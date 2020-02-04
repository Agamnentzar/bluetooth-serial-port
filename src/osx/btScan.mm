//
// Scanning for Bluetooth devices requires the RunLoop of the main thread to be executed.
// This cannot be guaranteed in a library which may be used from a Unix-style console
// application.  Hence, BT scanning is implemented as this separate executable where
// we control the RunLoop.
//
#include <iostream>

#import <Foundation/NSObject.h>
#import <IOBluetooth/objc/IOBluetoothDevice.h>
#import <IOBluetooth/objc/IOBluetoothDeviceInquiry.h>

#include "json.hpp"

// The purpose of this delegate is to finish the RunLoop once the inquiry completes.
@interface DiscoveryFinisher : NSObject<IOBluetoothDeviceInquiryDelegate>
- (void) deviceInquiryComplete: (IOBluetoothDeviceInquiry *)sender error: (IOReturn)error aborted: (BOOL)aborted;
@end

@implementation DiscoveryFinisher
- (void)deviceInquiryComplete: (IOBluetoothDeviceInquiry *)sender error: (IOReturn)error aborted: (BOOL)aborted
{
    CFRunLoopStop(CFRunLoopGetCurrent());
}
@end

nlohmann::json jsonForDevice(IOBluetoothDevice *device)
{
    nlohmann::json j = {
        {"addressString", [[device addressString] UTF8String]},
        {"classOfDevice", [device classOfDevice]},
        {"deviceClassMajor", [device deviceClassMajor]},
        {"deviceClassMinor", [device deviceClassMinor]},
        {"nameOrAddress", [[device nameOrAddress] UTF8String]},
        {"rawRSSI", [device rawRSSI]},
        {"isConnected", (bool)[device isConnected]},
        {"isPaired", (bool)[device isPaired]},
        {"isFavorite", (bool)[device isFavorite]},
        {"lastSeen", [[device getLastInquiryUpdate] timeIntervalSince1970]},
        {"haveName", [device name] != nil}
    };
    return j;
}

int main(int argc, const char* argv[])
{
    @autoreleasepool {
        DiscoveryFinisher* df = [[DiscoveryFinisher alloc] init];
        IOBluetoothDeviceInquiry* bdi = [[IOBluetoothDeviceInquiry alloc] initWithDelegate: df];
        [bdi setUpdateNewDeviceNames:NO];
        if ([bdi start] != kIOReturnSuccess) {
            // Output empty JS array.
            printf("[]");
            return -1;
        }
        CFRunLoopRun();
        [bdi stop];

        NSArray *foundDevices = [bdi foundDevices];
        nlohmann::json j = nlohmann::json::array();
        NSEnumerator *enumerator = [foundDevices objectEnumerator];
        while (IOBluetoothDevice *dev = (IOBluetoothDevice *)[enumerator nextObject]) {
            j.push_back(jsonForDevice(dev));
        }
        printf("%s", j.dump(2).c_str());
    }

    return 0;
}
