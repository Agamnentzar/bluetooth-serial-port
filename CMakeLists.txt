project(bluetoothserialport)
cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)

add_library(bluetoothserialport)

if(WIN32)
	target_sources(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/windows/BluetoothHelpers.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/windows/BTSerialPortBinding.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/windows/DeviceINQ.cc
	)
	target_include_directories(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/windows
	)
	target_link_libraries(bluetoothserialport ws2_32 bthprops)

elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin") # MacOSX
	target_sources(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BluetoothDeviceResources.mm
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BluetoothWorker.mm
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BTSerialPortBinding.mm
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/DeviceINQ.mm src/osx/pipe.c
	)
	target_include_directories(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx
	)
	target_link_libraries(bluetoothserialport "-framework Foundation -framework IOBluetooth -fobjc-arc")
else()
	target_sources(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/linux/BTSerialPortBinding.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/linux/DeviceINQ.cc
	)
	target_link_libraries(bluetoothserialport bluetooth)
	target_include_directories(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/linux
	)
endif()

target_include_directories(bluetoothserialport PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(inquiretest EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp)

set_target_properties(bluetoothserialport
PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(inquiretest bluetoothserialport)
