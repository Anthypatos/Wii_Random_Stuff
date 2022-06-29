#include <ios>
#include <jansson.h>
#include "../include/SETTINGS.hpp"


Settings::Settings(const char* pcfilePath)
{
    json_t* jsonRoot = nullptr;
	json_t* jsonSettings = nullptr;
	json_t* jsonField = nullptr;
    json_error_t error;

	jsonRoot = json_load_file(pcfilePath, 0, &error);
	if(!jsonRoot) throw std::ios_base::failure("Error opening file");

	if(!json_is_object(jsonRoot))
	{
		json_decref(jsonRoot);
		throw std::ios_base::failure("Error: Root is not an object");
	}

	jsonSettings = json_object_get(jsonRoot, "settings");
	if(!json_is_object(jsonSettings))
	{
		json_decref(jsonRoot);
		throw std::ios_base::failure("Error: settings is not an object");
	}
	
	jsonField = json_object_get(jsonSettings, "background_music");
	if(!json_is_boolean(jsonField))
	{
		json_decref(jsonRoot);
		throw std::ios_base::failure("Error: background_music is not boolean");
	}
	_bBackgroundMusic = json_boolean_value(jsonField);

	jsonField = json_object_get(jsonSettings, "rumble");
	if(!json_is_boolean(jsonField))
	{
		json_decref(jsonRoot);
		throw std::ios_base::failure("Error: rumble is not boolean");
	}
	_bRumble = json_boolean_value(jsonField);

	json_decref(jsonRoot);
}