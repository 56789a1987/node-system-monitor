#include <stdio.h>
#include <vector>
#include "cpu.h"

static CPUTimeEntry *lastGeneralCPU = NULL;
static std::vector<CPUTimeEntry *> lastCPUs;

void GetCPUUsage(const FunctionCallbackInfo<Value> &args)
{
	// native value fields
	CPUTimeEntry *pInfo;
	char line[160], id[64];
	unsigned long long user, nice, system, idle, ioWait, irq, softIrq;
	size_t n = 0;

	// node value fields
	Isolate *isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();

	// make sure we have opened the file successfully
	FILE *file = fopen("/proc/stat", "r");
	if (file == NULL)
	{
		THROW_ERROR(isolate, "Error getting CPU stats");
		return;
	}

	Local<Array> array = Array::New(isolate);
	Local<Object> entry;
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
					pInfo = new CPUTimeEntry;
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
					pInfo = new CPUTimeEntry;
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

			// create CPU entry object and set properties
			entry = Object::New(isolate);
			entry->Set(context, V8_STRING("user"), V8_NUMBER(user)).Check();
			entry->Set(context, V8_STRING("nice"), V8_NUMBER(nice)).Check();
			entry->Set(context, V8_STRING("system"), V8_NUMBER(system)).Check();
			entry->Set(context, V8_STRING("idle"), V8_NUMBER(idle)).Check();
			entry->Set(context, V8_STRING("ioWait"), V8_NUMBER(ioWait)).Check();
			entry->Set(context, V8_STRING("irq"), V8_NUMBER(irq)).Check();
			entry->Set(context, V8_STRING("softIrq"), V8_NUMBER(softIrq)).Check();

			// add the entry to array
			if (isGeneral)
			{
				array->Set(context, V8_STRING("general"), entry).Check();
			}
			else
			{
				array->Set(context, n, entry).Check();
				n++;
			}
		}
	}

	fclose(file);
	args.GetReturnValue().Set(array);
}
