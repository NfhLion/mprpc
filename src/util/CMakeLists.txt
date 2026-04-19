set(HRPC_UTIL_SOURCES
    config.cc
    FileUtil.cc
    LogFile.cc
    asynclogging.cc
    logger.cc
    thread.cc
    threadpool.cc
)

add_library(hrpc_util ${HRPC_LIBRARY_TYPE} ${HRPC_UTIL_SOURCES})

target_include_directories(hrpc_util
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/util>
)

set_target_properties(hrpc_util PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
