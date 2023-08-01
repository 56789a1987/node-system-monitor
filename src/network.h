#ifndef MONITOR_NETWORK_H
#define MONITOR_NETWORK_H

#include "common.h"

typedef struct
{
	unsigned long long recvBytes;
	unsigned long long recvPackets;
	unsigned long long transBytes;
	unsigned long long transPackets;
} NetworkEntry;

napi_value GetNetworkUsage(const napi_env env, const napi_callback_info info);

#endif
