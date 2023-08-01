#include <stdio.h>
#include <string.h>
#include "memory.h"

napi_value GetMemoryUsage(const napi_env env, const napi_callback_info info)
{
	char line[128], name[64];
	unsigned long long value;

	napi_status status;
	napi_value nValue;

	FILE *file = fopen("/proc/meminfo", "r");
	if (file == NULL)
	{
		napi_throw_error(env, NULL, "Error getting memory info");
		return NULL;
	}

	NewObject(object);
	while (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(line, "%s %llu", name, &value);
		name[strlen(name) - 1] = 0; // remove ':'

		// we only need several values here instead of the full list
		if (
			strcmp(name, "MemTotal") == 0 ||
			strcmp(name, "MemFree") == 0 ||
			strcmp(name, "MemAvailable") == 0 ||
			strcmp(name, "SwapTotal") == 0 ||
			strcmp(name, "SwapFree") == 0)
		{
			NAPI_ASSERT(napi_create_double(env, value, &nValue));
			NAPI_ASSERT(napi_set_named_property(env, object, name, nValue));
		}
	}

	fclose(file);
	return object;
}
