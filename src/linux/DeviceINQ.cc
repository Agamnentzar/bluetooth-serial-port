#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "BluetoothException.h"
#include "DeviceINQ.h"

extern "C"{
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <assert.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
}

using namespace std;

DeviceINQ *DeviceINQ::Create()
{
	return new DeviceINQ();
}

DeviceINQ::DeviceINQ()
{
}

DeviceINQ::~DeviceINQ()
{
}

vector<device> DeviceINQ::Inquire(int length)
{
	char addr[19] = { 0 };
	char name[248] = { 0 };
	int dev_id = hci_get_route(NULL);
	int sock = hci_open_dev(dev_id);

	if (dev_id < 0 || sock < 0)
		throw BluetoothException("error opening socket");

	int max_rsp = 255;
	inquiry_info *ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
	int num_rsp = hci_inquiry(dev_id, length, max_rsp, NULL, &ii, IREQ_CACHE_FLUSH);

	vector<device> devices;

	for (int i = 0; i < num_rsp; i++)
	{
		ba2str(&(ii + i)->bdaddr, addr);
		memset(name, 0, sizeof(name));

		hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof(name), name, 0);

		//int8_t rssi = 0;
		//hci_read_rssi(sock, uint16_t handle, &rssi, 0);

		unsigned char *p = (ii + i)->dev_class;
		uint32_t cod = (uint32_t)(p[0] + (0x100 * (p[1] + (0x100 * p[2]))));

		device dev;
		dev.address = string(addr);
		dev.name = string(name);
		dev.connected = false;
		dev.remembered = false;
		dev.authenticated = false;
		dev.lastSeen = 0;
		dev.lastUsed = 0;
		dev.deviceClass = (DeviceClass)(cod & 0x1ffc);
		dev.majorDeviceClass = (DeviceClass)(cod & DC_Uncategorized);
		dev.serviceClass = (ServiceClass)(cod >> 13);
		devices.push_back(dev);
	}

	free(ii);
	close(sock);
	return devices;
}

int DeviceINQ::SdpSearch(string address)
{
	int channelID = -1;
	uuid_t svc_uuid;
	bdaddr_t target;
	bdaddr_t source = { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };
	sdp_list_t *response_list = NULL, *search_list, *attrid_list;
	sdp_session_t *session = 0;

	str2ba(address.c_str(), &target);

	// connect to the SDP server running on the remote machine
	// session = sdp_connect(BDADDR_ANY, &target, SDP_RETRY_IF_BUSY);
	session = sdp_connect(&source, &target, SDP_RETRY_IF_BUSY);

	if (!session)
		throw BluetoothException("no session resurned from sdp_connect");

	// specify the UUID of the application we're searching for
	sdp_uuid16_create(&svc_uuid, SERIAL_PORT_PROFILE_ID);
	search_list = sdp_list_append(NULL, &svc_uuid);

	// specify that we want a list of all the matching applications' attributes
	uint32_t range = 0x0000ffff;
	attrid_list = sdp_list_append(NULL, &range);

	// get a list of service records that have the serial port UUID
	sdp_service_search_attr_req(session, search_list, SDP_ATTR_REQ_RANGE, attrid_list, &response_list);
	sdp_list_t *r = response_list;

	// go through each of the service records
	for (; r; r = r->next)
	{
		sdp_record_t *rec = (sdp_record_t*)r->data;
		sdp_list_t *proto_list;

		// get a list of the protocol sequences
		if (sdp_get_access_protos(rec, &proto_list) == 0)
		{
			sdp_list_t *p = proto_list;

			// go through each protocol sequence
			for (; p; p = p->next)
			{
				sdp_list_t *pds = (sdp_list_t*)p->data;

				// go through each protocol list of the protocol sequence
				for (; pds; pds = pds->next)
				{

					// check the protocol attributes
					sdp_data_t *d = (sdp_data_t*)pds->data;
					int proto = 0;

					for (; d; d = d->next)
					{
						switch (d->dtd)
						{
						case SDP_UUID16:
						case SDP_UUID32:
						case SDP_UUID128:
							proto = sdp_uuid_to_proto(&d->val.uuid);
							break;
						case SDP_UINT8:
							if (proto == RFCOMM_UUID)
							{
								channelID = d->val.int8;
								// TODO: cleanup
								return channelID; // stop if channel is found
							}
							break;
						}
					}
				}

				sdp_list_free((sdp_list_t*)p->data, 0);
			}

			sdp_list_free(proto_list, 0);
		}

		sdp_record_free(rec);
	}
	sdp_close(session);
	return channelID;
}
