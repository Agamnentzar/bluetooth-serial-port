project(easybluetooth)
cmake_minimum_required(VERSION 2.8)

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()

if(WIN32)
	set(srcs src/windows/BluetoothHelpers.cc  src/windows/BTSerialPortBinding.cc  src/windows/DeviceINQ.cc)
	include_directories(windows)
	set(libs ws2_32)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin") # MacOSX
	set(srcs src/osx/BluetoothDeviceResources.mm  src/osx/BluetoothWorker.mm  src/osx/BTSerialPortBinding.mm  src/osx/DeviceINQ.mm  src/osx/pipe.c)
	set(libs "-framework Foundation -framework IOBluetooth -fobjc-arc")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-gc")
	include_directories(osx)
else()
	set(srcs src/linux/BTSerialPortBinding.cc  src/linux/DeviceINQ.cc)
	set(libs bluetooth)
	include_directories(linux)
endif()

include_directories(./src)

if (MSVC)
	add_library(easybluetooth STATIC ${srcs})
else()
	add_library(easybluetooth SHARED ${srcs})
endif()

add_executable(inquiretest EXCLUDE_FROM_ALL test/main.cpp)

target_link_libraries(easybluetooth ${libs})
target_link_libraries(inquiretest easybluetooth)