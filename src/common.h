#ifndef MONITOR_COMMON_H
#define MONITOR_COMMON_H

#include <v8.h>
#include <node.h>

using namespace v8;

#define V8_STRING(string) String::NewFromUtf8(isolate, string, NewStringType::kInternalized).ToLocalChecked()
#define V8_NUMBER(value) Number::New(isolate, value)

inline void THROW_ERROR(Isolate *isolate, const char *message)
{
	isolate->ThrowException(Exception::Error(V8_STRING(message)));
}

inline void THROW_TYPE_ERROR(Isolate *isolate, const char *message)
{
	isolate->ThrowException(Exception::TypeError(V8_STRING(message)));
}

#endif
