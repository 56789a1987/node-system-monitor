#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "cpu.h"

static CPUTimeEntry *lastGeneralCPU = NULL;
static std::vector<CPUTimeEntry *> lastCPUs;

napi_value GetCPUUsage(const napi_env env, const napi_callback_info info)
{
	CPUTimeEntry *pInfo;
	char line[160], id[64];
	unsigned long long user, nice, system, idle, ioWait, irq, softIrq;
	size_t n = 0;

	napi_status status;
	napi_value nUser, nNice, nSystem, nIdle, nIoWait, nIrq, nSoftIrq;

	// make sure we have opened the file successfully
	FILE *file = fopen("/proc/stat", "r");
	if (file == NULL)
	{
		napi_throw_error(env, NULL, "Error getting CPU stats");
		return NULL;
	}

	NewArray(array);
	while (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%s %llu %llu %llu %llu %llu %llu %llu", id, &user, &nice, &system, &idle, &ioWait, &irq, &softIrq);
		if (strncmp(id, "cpu", 3) == 0)
		{
			bool isGeneral = strcmp(id, "cpu") == 0;
			CPUTimeEntry info = {user, nice, system, idle, ioWait, irq, softIrq};

			if (isGeneral)
			{
				if (lastGeneralCPU == NULL)
				{
					// initialize last values object
					// the result of the first call should be discarded if possible
					// because the difference is the total value
					pInfo = (CPUTimeEntry *)malloc(sizeof(CPUTimeEntry));
					memcpy(pInfo, &info, sizeof(CPUTimeEntry));
					lastGeneralCPU = pInfo;
					pInfo = NULL;
				}
				else
				{
					pInfo = lastGeneralCPU;
				}
			}
			else
			{
				if (lastCPUs.size() <= n)
				{
					// initialize last values object
					// the result of the first call should be discarded if possible
					// because the difference is the total value
					pInfo = (CPUTimeEntry *)malloc(sizeof(CPUTimeEntry));
					memcpy(pInfo, &info, sizeof(CPUTimeEntry));
					lastCPUs.push_back(pInfo);
					pInfo = NULL;
				}
				else
				{
					pInfo = lastCPUs.at(n);
				}
			}

			// get the difference from the last values
			if (pInfo != NULL)
			{
				user = info.user - pInfo->user;
				nice = info.nice - pInfo->nice;
				system = info.system - pInfo->system;
				idle = info.idle - pInfo->idle;
				ioWait = info.ioWait - pInfo->ioWait;
				irq = info.irq - pInfo->irq;
				softIrq = info.softIrq - pInfo->softIrq;
				// copy current values to last values
				memcpy(pInfo, &info, sizeof(CPUTimeEntry));
			}

			// create values for CPU entry object
			NAPI_ASSERT(napi_create_double(env, user, &nUser));
			NAPI_ASSERT(napi_create_double(env, nice, &nNice));
			NAPI_ASSERT(napi_create_double(env, system, &nSystem));
			NAPI_ASSERT(napi_create_double(env, idle, &nIdle));
			NAPI_ASSERT(napi_create_double(env, ioWait, &nIoWait));
			NAPI_ASSERT(napi_create_double(env, irq, &nIrq));
			NAPI_ASSERT(napi_create_double(env, softIrq, &nSoftIrq));

			// create CPU entry object and set properties
			NewObject(item);
			napi_property_descriptor properties[] = {
				DECLARE_NAPI_VALUE("user", nUser),
				DECLARE_NAPI_VALUE("nice", nNice),
				DECLARE_NAPI_VALUE("system", nSystem),
				DECLARE_NAPI_VALUE("idle", nIdle),
				DECLARE_NAPI_VALUE("ioWait", nIoWait),
				DECLARE_NAPI_VALUE("irq", nIrq),
				DECLARE_NAPI_VALUE("softIrq", nSoftIrq),
			};
			NAPI_ASSERT(napi_define_properties(env, item, 7, properties));

			// add the entry to array
			if (isGeneral)
			{
				status = napi_set_named_property(env, array, "general", item);
			}
			else
			{
				status = napi_set_element(env, array, n, item);
				n++;
			}
			assert(status == napi_ok);
		}
	}

	fclose(file);
	return array;
}
