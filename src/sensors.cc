#include <sensors/sensors.h>
#include "sensors.h"

napi_value GetSensors(const napi_env env, const napi_callback_info info)
{
	double value;

	napi_status status;
	napi_value nValue, nPrefix, nPath, nName, nNumber, nType;

	static bool sensorsLoaded = false;
	if (!sensorsLoaded)
	{
		if (sensors_init(NULL) != 0)
		{
			napi_throw_error(env, NULL, "Error initializing sensors");
			return NULL;
		}
		sensorsLoaded = true;
	}

	NewArray(array);
	int nr1 = 0, i1 = 0;
	for (;;)
	{
		const sensors_chip_name *chipName = sensors_get_detected_chips(0, &nr1);
		if (chipName == NULL)
			break;

		// create values for chip name object
		NewArray(featuresArray);
		NAPI_ASSERT(napi_create_string_utf8(env, chipName->prefix, NAPI_AUTO_LENGTH, &nPrefix));
		NAPI_ASSERT(napi_create_string_utf8(env, chipName->path, NAPI_AUTO_LENGTH, &nPath));
		napi_property_descriptor properties[] = {
			DECLARE_NAPI_VALUE("prefix", nPrefix),
			DECLARE_NAPI_VALUE("path", nPath),
			DECLARE_NAPI_VALUE("features", featuresArray),
		};

		// create chip name object and add it to the general array
		NewObject(chipNameObject);
		NAPI_ASSERT(napi_define_properties(env, chipNameObject, 3, properties));
		NAPI_ASSERT(napi_set_element(env, array, i1, chipNameObject));
		i1++;

		int nr2 = 0, i2 = 0;
		for (;;)
		{
			const sensors_feature *feature = sensors_get_features(chipName, &nr2);
			if (feature == NULL)
				break;

			// create values for feature object
			NewArray(subfeaturesArray);
			NAPI_ASSERT(napi_create_string_utf8(env, feature->name, NAPI_AUTO_LENGTH, &nName));
			NAPI_ASSERT(napi_create_double(env, feature->number, &nNumber));
			NAPI_ASSERT(napi_create_double(env, feature->type, &nType));
			napi_property_descriptor properties[] = {
				DECLARE_NAPI_VALUE("name", nName),
				DECLARE_NAPI_VALUE("number", nNumber),
				DECLARE_NAPI_VALUE("type", nType),
				DECLARE_NAPI_VALUE("subfeatures", subfeaturesArray),
			};

			// create feature object and add it to the features array
			NewObject(featureObject);
			NAPI_ASSERT(napi_define_properties(env, featureObject, 4, properties));
			NAPI_ASSERT(napi_set_element(env, featuresArray, i2, featureObject));
			i2++;

			int nr3 = 0, i3 = 0;
			for (;;)
			{
				const sensors_subfeature *subfeature = sensors_get_all_subfeatures(chipName, feature, &nr3);
				if (subfeature == NULL)
					break;

				// create values for subfeature object
				NAPI_ASSERT(napi_create_string_utf8(env, subfeature->name, NAPI_AUTO_LENGTH, &nName));
				NAPI_ASSERT(napi_create_double(env, subfeature->number, &nNumber));
				NAPI_ASSERT(napi_create_double(env, subfeature->type, &nType));
				napi_property_descriptor properties[] = {
					DECLARE_NAPI_VALUE("name", nName),
					DECLARE_NAPI_VALUE("number", nNumber),
					DECLARE_NAPI_VALUE("type", nType),
				};

				// create subfeature object and add it to the subfeatures array
				NewObject(subfeatureObject);
				NAPI_ASSERT(napi_define_properties(env, subfeatureObject, 3, properties));
				NAPI_ASSERT(napi_set_element(env, subfeaturesArray, i3, subfeatureObject));
				i3++;

				// set value of subfeature
				if (subfeature->flags & SENSORS_MODE_R)
				{
					int rc = sensors_get_value(chipName, subfeature->number, &value);
					if (rc >= 0)
					{
						NAPI_ASSERT(napi_create_double(env, value, &nValue));
						NAPI_ASSERT(napi_set_named_property(env, subfeatureObject, "value", nValue));
					}
				}
			}
		}
	}

	return array;
}
