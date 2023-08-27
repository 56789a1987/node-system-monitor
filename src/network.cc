#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "network.h"

static std::vector<NetworkEntry *> lastNetworks;

napi_value GetNetworkUsage(const napi_env env, const napi_callback_info info)
{
	NetworkEntry *pInfo;
	char line[160], name[64];
	unsigned long long temp, recvBytes, recvPackets, transBytes, transPackets;
	size_t n = 0;

	napi_status status;
	napi_value nName, nRecvBytes, nRecvPackets, nTransBytes, nTransPackets, nTotalRecvBytes, nTotalRecvPackets, nTotalTransBytes, nTotalTransPackets;

	FILE *file = fopen("/proc/net/dev", "r");
	if (file == NULL)
	{
		napi_throw_error(env, NULL, "Error getting network devices");
		return NULL;
	}

	// skip table header - first two lines
	fgets(line, sizeof(line), file);
	fgets(line, sizeof(line), file);

	NewArray(array);
	while (fgets(line, sizeof(line), file) != NULL)
	{
		sscanf(
			line, "%s %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu", name,
			&recvBytes, &recvPackets, &temp, &temp, &temp, &temp, &temp, &temp,
			&transBytes, &transPackets, &temp, &temp, &temp, &temp, &temp, &temp);
		name[strlen(name) - 1] = 0; // remove ':'
		NetworkEntry info = {recvBytes, recvPackets, transBytes, transPackets};

		if (lastNetworks.size() <= n)
		{
			// initialize last values object
			// the result of the first call should be discarded if possible
			// because the difference is the total value
			pInfo = (NetworkEntry *)malloc(sizeof(NetworkEntry));
			memcpy(pInfo, &info, sizeof(NetworkEntry));
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
			recvBytes = info.recvBytes - pInfo->recvBytes;
			recvPackets = info.recvPackets - pInfo->recvPackets;
			transBytes = info.transBytes - pInfo->transBytes;
			transPackets = info.transPackets - pInfo->transPackets;
			memcpy(pInfo, &info, sizeof(NetworkEntry));
		}

		// create values for network entry object
		NAPI_ASSERT(napi_create_string_utf8(env, name, NAPI_AUTO_LENGTH, &nName));
		NAPI_ASSERT(napi_create_double(env, recvBytes, &nRecvBytes));
		NAPI_ASSERT(napi_create_double(env, recvPackets, &nRecvPackets));
		NAPI_ASSERT(napi_create_double(env, transBytes, &nTransBytes));
		NAPI_ASSERT(napi_create_double(env, transPackets, &nTransPackets));
		NAPI_ASSERT(napi_create_double(env, info.recvBytes, &nTotalRecvBytes));
		NAPI_ASSERT(napi_create_double(env, info.recvPackets, &nTotalRecvPackets));
		NAPI_ASSERT(napi_create_double(env, info.transBytes, &nTotalTransBytes));
		NAPI_ASSERT(napi_create_double(env, info.transPackets, &nTotalTransPackets));

		// create network entry object and set properties
		NewObject(item);
		napi_property_descriptor properties[] = {
			DECLARE_NAPI_VALUE("name", nName),
			DECLARE_NAPI_VALUE("recvBytes", nRecvBytes),
			DECLARE_NAPI_VALUE("recvPackets", nRecvPackets),
			DECLARE_NAPI_VALUE("transBytes", nTransBytes),
			DECLARE_NAPI_VALUE("transPackets", nTransPackets),
			DECLARE_NAPI_VALUE("totalRecvBytes", nTotalRecvBytes),
			DECLARE_NAPI_VALUE("totalRecvPackets", nTotalRecvPackets),
			DECLARE_NAPI_VALUE("totalTransBytes", nTotalTransBytes),
			DECLARE_NAPI_VALUE("totalTransPackets", nTotalTransPackets),
		};
		NAPI_ASSERT(napi_define_properties(env, item, 9, properties));

		// add the entry to array
		NAPI_ASSERT(napi_set_element(env, array, n, item));
		n++;
	}

	fclose(file);
	return array;
}
