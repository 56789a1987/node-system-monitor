#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "network.h"

static std::vector<NetworkEntry *> lastNetworks;

void GetNetworkUsage(const FunctionCallbackInfo<Value> &args)
{
	// native value fields
	NetworkEntry *pInfo;
	char line[160], name[64];
	unsigned long long temp, recvBytes, recvPackets, transBytes, transPackets;
	size_t n = 0;

	// node value fields
	Isolate *isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();

	FILE *file = fopen("/proc/net/dev", "r");
	if (file == NULL)
	{
		THROW_ERROR(isolate, "Error getting network devices");
		return;
	}

	// skip table header - first two lines
	fgets(line, sizeof(line), file);
	fgets(line, sizeof(line), file);

	NetworkEntry entry;
	Local<Array> array = Array::New(isolate);
	while (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(
			line, "%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", name,
			&recvBytes, &recvPackets, &temp, &temp, &temp, &temp, &temp, &temp,
			&transBytes, &transPackets, &temp, &temp, &temp, &temp, &temp, &temp);
		name[strlen(name) - 1] = 0; // remove ':'
		entry = {recvBytes, recvPackets, transBytes, transPackets};

		if (lastNetworks.size() <= n)
		{
			// initialize last values object
			// the result of the first call should be discarded if possible
			// because the difference is the total value
			pInfo = (NetworkEntry *)malloc(sizeof(NetworkEntry));
			memcpy(pInfo, &entry, sizeof(NetworkEntry));
			lastNetworks.push_back(pInfo);
			pInfo = NULL;
		}
		else
		{
			pInfo = lastNetworks.at(n);
		}

		// get the difference from the last values
		if (pInfo != NULL)
		{
			recvBytes = entry.recvBytes - pInfo->recvBytes;
			recvPackets = entry.recvPackets - pInfo->recvPackets;
			transBytes = entry.transBytes - pInfo->transBytes;
			transPackets = entry.transPackets - pInfo->transPackets;
			memcpy(pInfo, &entry, sizeof(NetworkEntry));
		}

		// create network entry object and set properties
		Local<Object> arrayItem = Object::New(isolate);
		arrayItem->Set(context, V8_STRING("name"), V8_STRING(name)).Check();
		arrayItem->Set(context, V8_STRING("recvBytes"), V8_NUMBER(recvBytes)).Check();
		arrayItem->Set(context, V8_STRING("recvPackets"), V8_NUMBER(recvPackets)).Check();
		arrayItem->Set(context, V8_STRING("transBytes"), V8_NUMBER(transBytes)).Check();
		arrayItem->Set(context, V8_STRING("transPackets"), V8_NUMBER(transPackets)).Check();
		arrayItem->Set(context, V8_STRING("totalRecvBytes"), V8_NUMBER(entry.recvBytes)).Check();
		arrayItem->Set(context, V8_STRING("totalRecvPackets"), V8_NUMBER(entry.recvPackets)).Check();
		arrayItem->Set(context, V8_STRING("totalTransBytes"), V8_NUMBER(entry.transBytes)).Check();
		arrayItem->Set(context, V8_STRING("totalTransPackets"), V8_NUMBER(entry.transPackets)).Check();

		// add the entry to array
		array->Set(context, n, arrayItem).Check();
		n++;
	}

	fclose(file);
	args.GetReturnValue().Set(array);
}
