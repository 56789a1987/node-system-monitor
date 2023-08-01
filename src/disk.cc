#include <sys/statvfs.h>
#include "disk.h"

napi_value GetDiskUsage(const napi_env env, const napi_callback_info info)
{
	char file[256];
	struct statvfs fsData;

	napi_status status;
	size_t argc = 1;
	napi_value args[1], nBlockSize, nBlocks, nBlocksFree, nBlocksAvail;
	napi_valuetype valueType;

	// make sure we have one argument
	NAPI_ASSERT(napi_get_cb_info(env, info, &argc, args, NULL, NULL));
	if (argc < 1)
	{
		napi_throw_error(env, NULL, "Must provide a string argument 'file'");
		return NULL;
	}

	// make sure the argument is a string
	NAPI_ASSERT(napi_typeof(env, args[0], &valueType));
	if (valueType != napi_string)
	{
		napi_throw_error(env, NULL, "Must provide a string argument 'file'");
		return NULL;
	}

	// get the string from arguments
	NAPI_ASSERT(napi_get_value_string_utf8(env, args[0], file, sizeof(file), &argc));
	if (statvfs(file, &fsData) != 0)
	{
		napi_throw_error(env, NULL, "Error getting disk stats");
		return NULL;
	}

	// create values for return object
	NAPI_ASSERT(napi_create_double(env, fsData.f_bsize, &nBlockSize));
	NAPI_ASSERT(napi_create_double(env, fsData.f_blocks, &nBlocks));
	NAPI_ASSERT(napi_create_double(env, fsData.f_bfree, &nBlocksFree));
	NAPI_ASSERT(napi_create_double(env, fsData.f_bavail, &nBlocksAvail));

	// create return object
	NewObject(object);
	napi_property_descriptor properties[] = {
		DECLARE_NAPI_VALUE("blockSize", nBlockSize),
		DECLARE_NAPI_VALUE("blocks", nBlocks),
		DECLARE_NAPI_VALUE("blocksFree", nBlocksFree),
		DECLARE_NAPI_VALUE("blocksAvail", nBlocksAvail),
	};
	NAPI_ASSERT(napi_define_properties(env, object, 4, properties));

	return object;
}
