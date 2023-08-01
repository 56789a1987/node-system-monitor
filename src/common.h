#ifndef MONITOR_COMMON_H
#define MONITOR_COMMON_H

#include <assert.h>
#include <node_api.h>

#define DECLARE_NAPI_VALUE(name, value)                            \
	{                                                              \
		name, NULL, NULL, NULL, NULL, value, napi_enumerable, NULL \
	}
#define DECLARE_NAPI_METHOD(name, function)                           \
	{                                                                 \
		name, NULL, function, NULL, NULL, NULL, napi_enumerable, NULL \
	}

#define NAPI_ASSERT(function) \
	status = function;        \
	assert(status == napi_ok);

#define NewObject(name)                      \
	napi_value name;                         \
	status = napi_create_object(env, &name); \
	assert(status == napi_ok);

#define NewArray(name)                      \
	napi_value name;                        \
	status = napi_create_array(env, &name); \
	assert(status == napi_ok);

#endif
