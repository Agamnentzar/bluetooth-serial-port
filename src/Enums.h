#pragma once

#include <string>

enum DeviceClass
{
	DC_Miscellaneous = 0,
	// Major class: Computer (desktop,notebook, PDA, organizers, .... ).
	DC_Computer = 256,
	DC_DesktopComputer = 260,
	DC_ServerComputer = 264,
	DC_LaptopComputer = 268,
	DC_HandheldComputer = 272,
	DC_PdaComputer = 276,
	DC_WearableComputer = 280,
	// Major class: Phone (cellular, cordless, payphone, modem, ...).
	DC_Phone = 512,
	DC_CellPhone = 516,
	DC_CordlessPhone = 520,
	DC_SmartPhone = 524,
	DC_WiredPhone = 528,
	DC_IsdnAccess = 532,
	// Major class: LAN/Network Access point.
	DC_AccessPointAvailable = 768,
	DC_AccessPoint1To17 = 800,
	DC_AccessPoint17To33 = 832,
	DC_AccessPoint33To50 = 864,
	DC_AccessPoint50To67 = 896,
	DC_AccessPoint67To83 = 928,
	DC_AccessPoint83To99 = 960,
	DC_AccessPointNoService = 992,
	// Major class: Audio/Video (headset,speaker,stereo, video display, vcr.....
	DC_AudioVideoUnclassified = 1024,
	DC_AudioVideoHeadset = 1028,
	DC_AudioVideoHandsFree = 1032,
	DC_AudioVideoMicrophone = 1040,
	DC_AudioVideoLoudSpeaker = 1044,
	DC_AudioVideoHeadphones = 1048,
	DC_AudioVideoPortable = 1052,
	DC_AudioVideoCar = 1056,
	DC_AudioVideoSetTopBox = 1060,
	DC_AudioVideoHiFi = 1064,
	DC_AudioVideoVcr = 1068,
	DC_AudioVideoVideoCamera = 1072,
	DC_AudioVideoCamcorder = 1076,
	DC_AudioVideoMonitor = 1080,
	DC_AudioVideoDisplayLoudSpeaker = 1084,
	DC_AudioVideoVideoConferencing = 1088,
	DC_AudioVideoGaming = 1096,
	// Major class: Peripheral (mouse, joystick, keyboards, ..... ).
	DC_Peripheral = 1280,
	DC_PeripheralJoystick = 1284,
	DC_PeripheralGamepad = 1288,
	DC_PeripheralRemoteControl = 1292,
	DC_PeripheralSensingDevice = 1296,
	DC_PeripheralDigitizerTablet = 1300,
	DC_PeripheralCardReader = 1304,
	DC_PeripheralKeyboard = 1344,
	DC_PeripheralPointingDevice = 1408,
	DC_PeripheralCombinedKeyboardPointingDevice = 1472,
	// Major class: Imaging (printing, scanner, camera, display, ...).
	DC_Imaging = 1536,
	DC_ImagingDisplay = 1552,
	DC_ImagingCamera = 1568,
	DC_ImagingScanner = 1600,
	DC_ImagingPrinter = 1664,
	// Major class: Wearable.
	DC_Wearable = 1792,
	DC_WearableWristWatch = 1796,
	DC_WearablePager = 1800,
	DC_WearableJacket = 1804,
	DC_WearableHelmet = 1808,
	DC_WearableGlasses = 1812,
	// Major class: Toy.
	DC_Toy = 2048,
	DC_ToyRobot = 2052,
	DC_ToyVehicle = 2056,
	DC_ToyFigure = 2058,
	DC_ToyController = 2060,
	DC_ToyGame = 2064,
	// Major class: Medical.
	DC_Medical = 2304,
	DC_MedicalBloodPressureMonitor = 2308,
	DC_MedicalThermometer = 2312,
	DC_MedicalWeighingScale = 2314,
	DC_MedicalGlucoseMeter = 2316,
	DC_MedicalPulseOximeter = 2320,
	DC_MedicalHeartPulseRateMonitor = 2324,
	DC_MedicalDataDisplay = 2328,
	// Uncategorized, specific device code not specified
	DC_Uncategorized = 7936,
};

enum ServiceClass
{
	SC_None = 0,
	SC_LimitedDiscoverableMode = 1,
	SC_Positioning = 8,
	SC_Network = 16,
	SC_Rendering = 32,
	SC_Capturing = 64,
	SC_ObjectTransfer = 128,
	SC_Audio = 256,
	SC_Telephony = 512,
	SC_Information = 1024,
};

std::string GetDeviceClassString(DeviceClass deviceClass);
std::string GetServiceClassString(ServiceClass serviceClass);
