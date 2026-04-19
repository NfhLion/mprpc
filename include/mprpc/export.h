#ifndef HRPC_EXPORT_H
#define HRPC_EXPORT_H

#if defined(_WIN32) || defined(__CYGWIN__)
#  if defined(HRPC_SHARED)
#    if defined(HRPC_BUILDING_SHARED)
#      define HRPC_API __declspec(dllexport)
#    else
#      define HRPC_API __declspec(dllimport)
#    endif
#  else
#    define HRPC_API
#  endif
#else
#  if __GNUC__ >= 4
#    define HRPC_API __attribute__((visibility("default")))
#  else
#    define HRPC_API
#  endif
#endif

#endif // HRPC_EXPORT_H
