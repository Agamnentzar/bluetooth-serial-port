project(bluetoothserialport)
cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)

add_library(bluetoothserialport)

target_sources(bluetoothserialport
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/Enums.cc
)

if(WIN32) # windows
	set(PLATFORM windows)
	target_sources(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/windows/BluetoothHelpers.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/windows/BTSerialPortBinding.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/windows/DeviceINQ.cc
	)
	target_link_libraries(bluetoothserialport ws2_32 bthprops)

elseif(APPLE) # MacOSX
	set(PLATFORM osx)
	target_sources(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BluetoothDeviceResources.mm
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BluetoothWorker.mm
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BTSerialPortBinding.mm
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/DeviceINQ.mm
		${CMAKE_CURRENT_SOURCE_DIR}/src/osx/pipe.c
	)
	find_library(FOUNDATION Foundation)
	find_library(IOBLUETOOTH IOBluetooth)
	target_link_libraries(bluetoothserialport ${FOUNDATION} ${IOBLUETOOTH} -fobjc-arc)
	
	add_executable(btScan src/osx/btScan.mm)
	target_link_libraries(btScan ${FOUNDATION} ${IOBLUETOOTH})
else() # Linux
	set(PLATFORM linux)
	target_sources(bluetoothserialport
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/linux/BTSerialPortBinding.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/linux/DeviceINQ.cc
	)
	target_link_libraries(bluetoothserialport bluetooth)
endif()

# Platform-specific compile flags
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") # G++
    target_compile_options(bluetoothserialport PRIVATE -Wall -Wextra)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC") # MSVC
    target_compile_options(bluetoothserialport PRIVATE /EHsc /W2 /c)
endif()

target_include_directories(bluetoothserialport
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/${PLATFORM}
)
add_executable(inquiretest EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp)

set_target_properties(bluetoothserialport
PROPERTIES
	POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(inquiretest bluetoothserialport ${FOUNDATION} ${IOBLUETOOTH})