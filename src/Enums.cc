#include "Enums.h"
#include <vector>
#include <string>
#include <numeric>
#include <iostream>

using namespace std;

string GetDeviceClassString(DeviceClass deviceClass)
{
	switch (deviceClass)
	{
	case DC_Miscellaneous:
		return "Miscellaneous";
	case DC_Computer:
		return "Computer";
	case DC_DesktopComputer:
		return "DesktopComputer";
	case DC_ServerComputer:
		return "ServerComputer";
	case DC_LaptopComputer:
		return "LaptopComputer";
	case DC_HandheldComputer:
		return "HandheldComputer";
	case DC_PdaComputer:
		return "PdaComputer";
	case DC_WearableComputer:
		return "WearableComputer";
	case DC_Phone:
		return "Phone";
	case DC_CellPhone:
		return "CellPhone";
	case DC_CordlessPhone:
		return "CordlessPhone";
	case DC_SmartPhone:
		return "SmartPhone";
	case DC_WiredPhone:
		return "WiredPhone";
	case DC_IsdnAccess:
		return "IsdnAccess";
	case DC_AccessPointAvailable:
		return "AccessPointAvailable";
	case DC_AccessPoint1To17:
		return "AccessPoint1To17";
	case DC_AccessPoint17To33:
		return "AccessPoint17To33";
	case DC_AccessPoint33To50:
		return "AccessPoint33To50";
	case DC_AccessPoint50To67:
		return "AccessPoint50To67";
	case DC_AccessPoint67To83:
		return "AccessPoint67To83";
	case DC_AccessPoint83To99:
		return "AccessPoint83To99";
	case DC_AccessPointNoService:
		return "AccessPointNoService";
	case DC_AudioVideoUnclassified:
		return "AudioVideoUnclassified";
	case DC_AudioVideoHeadset:
		return "AudioVideoHeadset";
	case DC_AudioVideoHandsFree:
		return "AudioVideoHandsFree";
	case DC_AudioVideoMicrophone:
		return "AudioVideoMicrophone";
	case DC_AudioVideoLoudSpeaker:
		return "AudioVideoLoudSpeaker";
	case DC_AudioVideoHeadphones:
		return "AudioVideoHeadphones";
	case DC_AudioVideoPortable:
		return "AudioVideoPortable";
	case DC_AudioVideoCar:
		return "AudioVideoCar";
	case DC_AudioVideoSetTopBox:
		return "AudioVideoSetTopBox";
	case DC_AudioVideoHiFi:
		return "AudioVideoHiFi";
	case DC_AudioVideoVcr:
		return "AudioVideoVcr";
	case DC_AudioVideoVideoCamera:
		return "AudioVideoVideoCamera";
	case DC_AudioVideoCamcorder:
		return "AudioVideoCamcorder";
	case DC_AudioVideoMonitor:
		return "AudioVideoMonitor";
	case DC_AudioVideoDisplayLoudSpeaker:
		return "AudioVideoDisplayLoudSpeaker";
	case DC_AudioVideoVideoConferencing:
		return "AudioVideoVideoConferencing";
	case DC_AudioVideoGaming:
		return "AudioVideoGaming";
	case DC_Peripheral:
		return "Peripheral";
	case DC_PeripheralJoystick:
		return "PeripheralJoystick";
	case DC_PeripheralGamepad:
		return "PeripheralGamepad";
	case DC_PeripheralRemoteControl:
		return "PeripheralRemoteControl";
	case DC_PeripheralSensingDevice:
		return "PeripheralSensingDevice";
	case DC_PeripheralDigitizerTablet:
		return "PeripheralDigitizerTablet";
	case DC_PeripheralCardReader:
		return "PeripheralCardReader";
	case DC_PeripheralKeyboard:
		return "PeripheralKeyboard";
	case DC_PeripheralPointingDevice:
		return "PeripheralPointingDevice";
	case DC_PeripheralCombinedKeyboardPointingDevice:
		return "PeripheralCombinedKeyboardPointingDevice";
	case DC_Imaging:
		return "Imaging";
	case DC_ImagingDisplay:
		return "ImagingDisplay";
	case DC_ImagingCamera:
		return "ImagingCamera";
	case DC_ImagingScanner:
		return "ImagingScanner";
	case DC_ImagingPrinter:
		return "ImagingPrinter";
	case DC_Wearable:
		return "Wearable";
	case DC_WearableWristWatch:
		return "WearableWristWatch";
	case DC_WearablePager:
		return "WearablePager";
	case DC_WearableJacket:
		return "WearableJacket";
	case DC_WearableHelmet:
		return "WearableHelmet";
	case DC_WearableGlasses:
		return "WearableGlasses";
	case DC_Toy:
		return "Toy";
	case DC_ToyRobot:
		return "ToyRobot";
	case DC_ToyVehicle:
		return "ToyVehicle";
	case DC_ToyFigure:
		return "ToyFigure";
	case DC_ToyController:
		return "ToyController";
	case DC_ToyGame:
		return "ToyGame";
	case DC_Medical:
		return "Medical";
	case DC_MedicalBloodPressureMonitor:
		return "MedicalBloodPressureMonitor";
	case DC_MedicalThermometer:
		return "MedicalThermometer";
	case DC_MedicalWeighingScale:
		return "MedicalWeighingScale";
	case DC_MedicalGlucoseMeter:
		return "MedicalGlucoseMeter";
	case DC_MedicalPulseOximeter:
		return "MedicalPulseOximeter";
	case DC_MedicalHeartPulseRateMonitor:
		return "MedicalHeartPulseRateMonitor";
	case DC_MedicalDataDisplay:
		return "MedicalDataDisplay";
	case DC_Uncategorized:
		return "Uncategorized";
	default:
		return "";
	}
}

string GetServiceClassString(ServiceClass serviceClass)
{
	if (serviceClass == SC_None)
		return "None";

	vector<string> list;

	if ((serviceClass & SC_LimitedDiscoverableMode) != 0)
		list.push_back("LimitedDiscoverableMode");
	if ((serviceClass & SC_Positioning) != 0)
		list.push_back("Positioning");
	if ((serviceClass & SC_Network) != 0)
		list.push_back("Network");
	if ((serviceClass & SC_Rendering) != 0)
		list.push_back("Rendering");
	if ((serviceClass & SC_Capturing) != 0)
		list.push_back("Capturing");
	if ((serviceClass & SC_ObjectTransfer) != 0)
		list.push_back("ObjectTransfer");
	if ((serviceClass & SC_Audio) != 0)
		list.push_back("Audio");
	if ((serviceClass & SC_Telephony) != 0)
		list.push_back("Telephony");
	if ((serviceClass & SC_Information) != 0)
		list.push_back("Information");

	return accumulate(list.begin(), list.end(), string(),
		[](const string& a, const string& b) -> string {
		return a + (a.length() > 0 ? "," : "") + b;
	});
}
