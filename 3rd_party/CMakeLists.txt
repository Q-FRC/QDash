cmake_minimum_required(VERSION 3.28)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)

# protobuf
if (NOT USE_SYSTEM_PROTOBUF)
    set(protobuf_ALLOW_CCACHE ON)
    set(protobuf_MSVC_STATIC_RUNTIME OFF)
    set(protobuf_BUILD_SHARED_LIBS ON)
    set(protobuf_BUILD_TESTS OFF)
    set(ABSL_PROPAGATE_CXX_STD ON)

    add_subdirectory(protobuf)

    set(Protobuf_DIR ${CMAKE_CURRENT_BINARY_DIR}/protobuf/cmake/protobuf)
    set(Protobuf_ROOT ${CMAKE_CURRENT_BINARY_DIR}/protobuf)

    set(Protobuf_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/src)

    file(WRITE "${Protobuf_DIR}/protobuf-targets.cmake" "")
else()
    find_package(Protobuf)
endif()

# WPILIB
if (NOT USE_SYSTEM_NTCORE)
    set(USE_SYSTEM_EIGEN OFF)
    set(USE_SYSTEM_FMTLIB OFF)
    set(WITH_JAVA OFF)
    set(WITH_CSCORE OFF)
    set(WITH_NTCORE ON)
    set(WITH_WPIMATH OFF)
    set(WITH_WPILIB OFF)
    set(WITH_TESTS OFF)
    set(WITH_GUI OFF)
    set(WITH_SIMULATION_MODULES OFF)

    set(NO_WERROR ON)

    add_subdirectory(allwpilib)
endif()
