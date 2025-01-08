cmake_minimum_required(VERSION 3.25)
include(FetchContent)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0074 NEW)

# WPILib
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
    set(WITH_PROTOBUF OFF)
    set(WITH_SIMULATION_MODULES OFF)

    set(NO_WERROR ON)

    FetchContent_Declare(
            ntcore
            GIT_REPOSITORY https://github.com/wpilibsuite/allwpilib.git
            GIT_TAG        17a03514bad6de195639634b3d57d5ac411d601e # 2025.1.1
            OVERRIDE_FIND_PACKAGE
    )
    FetchContent_MakeAvailable(ntcore)
endif()
