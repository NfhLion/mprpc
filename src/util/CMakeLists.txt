set(SRC_LIST config.cc
             FileUtil.cc
             LogFile.cc
             asynclogging.cc
             logger.cc
             thread.cc
             threadpool.cc
             )

add_library(hrpc_util ${SRC_LIST})