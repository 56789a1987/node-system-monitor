#include <sensors/sensors.h>
#include "sensors.h"

void GetSensors(const FunctionCallbackInfo<Value> &args)
{
	double value;

	// node value fields
	Isolate *isolate = args.GetIsolate();
	Local<Context> context = isolate->GetCurrentContext();

	static bool sensorsLoaded = false;
	if (!sensorsLoaded)
	{
		if (sensors_init(NULL) != 0)
		{
			THROW_ERROR(isolate, "Error initializing sensors");
			return;
		}
		sensorsLoaded = true;
	}

	Local<Array> array = Array::New(isolate);
	int nr1 = 0, i1 = 0;
	for (;;)
	{
		const sensors_chip_name *chipName = sensors_get_detected_chips(0, &nr1);
		if (chipName == NULL)
			break;

		// create a chip name object and add it to general array
		Local<Object> chipNameObject = Object::New(isolate);
		Local<Array> featuresArray = Array::New(isolate);
		chipNameObject->Set(context, V8_STRING("prefix"), V8_STRING(chipName->prefix)).Check();
		chipNameObject->Set(context, V8_STRING("path"), V8_STRING(chipName->path)).Check();
		chipNameObject->Set(context, V8_STRING("features"), featuresArray).Check();
		array->Set(context, i1, chipNameObject).Check();
		i1++;

		int nr2 = 0, i2 = 0;
		for (;;)
		{
			const sensors_feature *feature = sensors_get_features(chipName, &nr2);
			if (feature == NULL)
				break;

			// create a feature object and add it to features array
			Local<Object> featureObject = Object::New(isolate);
			Local<Array> subfeaturesArray = Array::New(isolate);
			featureObject->Set(context, V8_STRING("name"), V8_STRING(feature->name)).Check();
			featureObject->Set(context, V8_STRING("number"), V8_NUMBER(feature->number)).Check();
			featureObject->Set(context, V8_STRING("type"), V8_NUMBER(feature->type)).Check();
			featureObject->Set(context, V8_STRING("subfeatures"), subfeaturesArray).Check();
			featuresArray->Set(context, i2, featureObject).Check();
			i2++;

			int nr3 = 0, i3 = 0;
			for (;;)
			{
				const sensors_subfeature *subfeature = sensors_get_all_subfeatures(chipName, feature, &nr3);
				if (subfeature == NULL)
					break;

				// create a subfeature object and add it to subfeatures array
				Local<Object> subfeatureObject = Object::New(isolate);
				subfeatureObject->Set(context, V8_STRING("name"), V8_STRING(subfeature->name)).Check();
				subfeatureObject->Set(context, V8_STRING("number"), V8_NUMBER(subfeature->number)).Check();
				subfeatureObject->Set(context, V8_STRING("type"), V8_NUMBER(subfeature->type)).Check();
				subfeaturesArray->Set(context, i3, subfeatureObject).Check();
				i3++;

				// set value of subfeature
				if (subfeature->flags & SENSORS_MODE_R)
				{
					int rc = sensors_get_value(chipName, subfeature->number, &value);
					if (rc >= 0)
					{
						subfeatureObject->Set(context, V8_STRING("value"), V8_NUMBER(value)).Check();
					}
				}
			}
		}
	}

	args.GetReturnValue().Set(array);
}
