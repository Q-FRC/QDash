set(CMAKE_AUTOMOC ON)

# Build Configuraton

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/BuildConfig.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/BuildConfig.cpp")

add_library(BuildConfig STATIC
    BuildConfig.h
    ${CMAKE_CURRENT_BINARY_DIR}/BuildConfig.cpp
)

target_link_libraries(BuildConfig Qt6::Core Qt6::Quick)
target_include_directories(BuildConfig PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
