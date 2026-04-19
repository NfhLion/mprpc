cmake_minimum_required(VERSION 3.16)
project(hrpc VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

option(HRPC_BUILD_SHARED "Build hrpc libraries as shared objects" OFF)
option(HRPC_BUILD_EXAMPLES "Build hrpc example programs" ON)
option(HRPC_BUILD_TESTS "Build hrpc tests" OFF)

if (HRPC_BUILD_SHARED)
    set(HRPC_LIBRARY_TYPE SHARED)
else()
    set(HRPC_LIBRARY_TYPE STATIC)
endif()

find_package(Protobuf REQUIRED)

add_subdirectory(src)

if (HRPC_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (HRPC_BUILD_TESTS)
    add_subdirectory(tests/hrpc_net)
    add_subdirectory(tests/hrpc_util)
endif()

add_custom_target(copy_sdk_headers ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/lib/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/lib/include
    COMMENT "Copying public SDK headers into lib/include")

add_custom_target(copy_runtime_config ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_SOURCE_DIR}/bin/config
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/config
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/log
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/config ${PROJECT_SOURCE_DIR}/bin/config
    COMMENT "Copying runtime config into bin/config")

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS hrpc hrpc_net hrpc_util
    EXPORT hrpcTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT hrpcTargets
    FILE hrpcTargets.cmake
    NAMESPACE hrpc::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hrpc)
