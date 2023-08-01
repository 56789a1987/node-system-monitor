#include "common.h"
#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "network.h"
#include "sensors.h"

static napi_value Init(napi_env env, napi_value exports)
{
	napi_property_descriptor methods[] = {
		DECLARE_NAPI_METHOD("getCPUUsage", GetCPUUsage),
		DECLARE_NAPI_METHOD("getDiskUsage", GetDiskUsage),
		DECLARE_NAPI_METHOD("getMemoryUsage", GetMemoryUsage),
		DECLARE_NAPI_METHOD("getNetworkUsage", GetNetworkUsage),
		DECLARE_NAPI_METHOD("getSensors", GetSensors),
	};
	napi_status status = napi_define_properties(env, exports, 5, methods);
	assert(status == napi_ok);

	return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
