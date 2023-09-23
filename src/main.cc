#include "common.h"
#include "cpu.h"
#include "disk.h"
#include "memory.h"
#include "network.h"
#include "sensors.h"

static void Initialize(Local<Object> exports)
{
	NODE_SET_METHOD(exports, "getCPUUsage", GetCPUUsage);
	NODE_SET_METHOD(exports, "getDiskUsage", GetDiskUsage);
	NODE_SET_METHOD(exports, "getMemoryUsage", GetMemoryUsage);
	NODE_SET_METHOD(exports, "getNetworkUsage", GetNetworkUsage);
	NODE_SET_METHOD(exports, "getSensors", GetSensors);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
