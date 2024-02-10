cmake_minimum_required(VERSION 3.25)
include(FetchContent)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)

# protobuf
if (NOT USE_SYSTEM_PROTOBUF)
    set(protobuf_ALLOW_CCACHE ON)
    set(protobuf_MSVC_STATIC_RUNTIME OFF)
    set(protobuf_BUILD_SHARED_LIBS ON)
    set(protobuf_BUILD_TESTS OFF)
    set(ABSL_PROPAGATE_CXX_STD ON)

    FetchContent_Declare(
            protobuf
            GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
            GIT_TAG        d00db12f9497e93898b9850a8f4d67ac6cc20917 # 26.0
            OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(Protobuf)
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

    FetchContent_Declare(
            ntcore
            GIT_REPOSITORY https://github.com/wpilibsuite/allwpilib.git
            GIT_TAG        9206b47d6708cbae61506e962c633d722884a26f # 2024.2.1
            OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(ntcore)
endif()
