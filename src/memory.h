#ifndef MONITOR_MEMORY_H
#define MONITOR_MEMORY_H

#include "common.h"

napi_value GetMemoryUsage(const napi_env env, const napi_callback_info info);

#endif
