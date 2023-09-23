#include <sys/statvfs.h>
#include "disk.h"

void GetDiskUsage(const FunctionCallbackInfo<Value> &args)
{
	// native value fields
	char file[256];
	struct statvfs fsData;

	// node value fields
	Isolate *isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();

	// make sure we have one argument
	if (args.Length() < 1 || !args[0]->IsString())
	{
		THROW_TYPE_ERROR(isolate, "Must provide a string argument 'file'");
		return;
	}

	// make sure the argument is a string
	args[0].As<String>()->WriteUtf8(isolate, file, sizeof(file));

	// get the string from arguments
	if (statvfs(file, &fsData) != 0)
	{
		THROW_ERROR(isolate, "Error getting disk stats");
		return;
	}

	// create return object
	Local<Object> returnValue = Object::New(isolate);
	returnValue->Set(context, V8_STRING("blockSize"), V8_NUMBER(fsData.f_bsize)).Check();
	returnValue->Set(context, V8_STRING("blocks"), V8_NUMBER(fsData.f_blocks)).Check();
	returnValue->Set(context, V8_STRING("blocksFree"), V8_NUMBER(fsData.f_bfree)).Check();
	returnValue->Set(context, V8_STRING("blocksAvail"), V8_NUMBER(fsData.f_bavail)).Check();

	args.GetReturnValue().Set(returnValue);
}
