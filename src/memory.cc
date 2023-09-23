#include <stdio.h>
#include <string.h>
#include "memory.h"

void GetMemoryUsage(const FunctionCallbackInfo<Value> &args)
{
	// native value fields
	char line[160], name[64];
	unsigned long long value;

	// node value fields
	Isolate *isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();

	FILE *file = fopen("/proc/meminfo", "r");
	if (file == NULL)
	{
		THROW_ERROR(isolate, "Error getting memory info");
		return;
	}

	Local<Object> returnValue = Object::New(isolate);

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
			returnValue->Set(context, V8_STRING(name), V8_NUMBER(value)).Check();
		}
	}

	fclose(file);
	args.GetReturnValue().Set(returnValue);
}
