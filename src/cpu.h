#ifndef MONITOR_CPU_H
#define MONITOR_CPU_H

#include "common.h"

typedef struct
{
	unsigned long long user;
	unsigned long long nice;
	unsigned long long system;
	unsigned long long idle;
	unsigned long long ioWait;
	unsigned long long irq;
	unsigned long long softIrq;
} CPUTimeEntry;

napi_value GetCPUUsage(const napi_env env, const napi_callback_info info);

#endif
