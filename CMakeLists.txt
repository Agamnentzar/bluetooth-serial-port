cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10)

project(bluetoothserialport LANGUAGES CXX)

add_library(bluetoothserialport)

target_compile_features(bluetoothserialport PUBLIC cxx_std_11)

target_sources(bluetoothserialport PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Enums.cc
)

if (WIN32)
    set(PLATFORM windows)
    target_sources(bluetoothserialport PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/BluetoothHelpers.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/BTSerialPortBinding.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/DeviceINQ.cc
    )
    target_link_libraries(bluetoothserialport PRIVATE ws2_32 bthprops)

elseif (APPLE)
    set(PLATFORM osx)
    target_sources(bluetoothserialport PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BluetoothDeviceResources.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BluetoothWorker.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/osx/BTSerialPortBinding.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/osx/DeviceINQ.mm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/osx/pipe.c
    )
    find_library(FOUNDATION Foundation REQUIRED)
    find_library(IOBLUETOOTH IOBluetooth REQUIRED)

    target_compile_options(bluetoothserialport PRIVATE "-fobjc-arc")
    target_link_libraries(bluetoothserialport PRIVATE ${FOUNDATION} ${IOBLUETOOTH})

    add_executable(btScan src/osx/btScan.mm)
    target_link_libraries(btScan PRIVATE ${FOUNDATION} ${IOBLUETOOTH})

else()
    set(PLATFORM linux)
    target_sources(bluetoothserialport PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/BTSerialPortBinding.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/DeviceINQ.cc
    )
    target_link_libraries(bluetoothserialport PRIVATE bluetooth)
endif()

# Platform-specific compile flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(bluetoothserialport PRIVATE -Wall -Wextra)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(bluetoothserialport PRIVATE /EHsc /W2 /c)
endif()

target_include_directories(bluetoothserialport
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${PLATFORM}
)

set_target_properties(bluetoothserialport PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(inquiretest EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp)
target_link_libraries(inquiretest PRIVATE bluetoothserialport ${FOUNDATION} ${IOBLUETOOTH})

