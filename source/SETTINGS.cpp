/**
	@file		SETTINGS.cpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		30/06/2022
    @brief		SETTINGS class
    @par		Description
                Implementation for the SETTINGS class
				
*/

#include <cstdio>
#include <string>
#include <ios>
#include <jansson.h>
#include "../include/SETTINGS.hpp"


/** Default path for storing the application's settings */
const std::string Settings::SCsDefaultPath = "/apps/test/settings.json";

/**
 * @brief Creates an object with the default settings
 */
Settings::Settings() noexcept : _bBackgroundMusic{true}, _bRumble{true} {} 


/**
 * @brief Constructs a new object by reading a settings file
 * 
 * @param sFilePath the path to the JSON file holding the settings
 */
Settings::Settings(const std::string& sFilePath)
{
    json_t* jsonRoot = nullptr;			// Root object of the JSON file
    json_error_t jsonerror;				// Error handler
    json_t* jsonSettings = nullptr;		// "Settings" JSON object
    json_t* jsonField = nullptr;		// Every JSON field inside the "Settings" object

	jsonRoot = json_load_file(sFilePath.c_str(), 0, &jsonerror);
	if(!jsonRoot) throw std::ios_base::failure("Error opening file");

	/* Retrieve the root and the "Settings" object first */
	if(!json_is_object(jsonRoot))
	{
		json_decref(jsonRoot);
        remove(sFilePath.c_str());
		throw std::ios_base::failure("Error: Root is not an object");
	}

	jsonSettings = json_object_get(jsonRoot, "settings");
	if(!json_is_object(jsonSettings))
	{
		json_decref(jsonRoot);
        remove(sFilePath.c_str());
		throw std::ios_base::failure("Error: settings is not an object");
	}
	
	/* New fields that are added to the class must be retrieved from here */
	jsonField = json_object_get(jsonSettings, "background_music");
	if(json_is_boolean(jsonField)) _bBackgroundMusic = json_boolean_value(jsonField);
	else _bBackgroundMusic = true;
	
	jsonField = json_object_get(jsonSettings, "rumble");
	if(json_is_boolean(jsonField)) _bRumble = json_boolean_value(jsonField);
	else _bRumble = true;

	// Free the objects from memory
    json_decref(jsonRoot);
}


/**
 * @brief Saves the settings on disk
 * 
 * @param sPath the path where the settings are to be stored
 */
void Settings::save(const std::string& sPath) const
{
    json_t* jsonRoot = json_object();		// Root object of the JSON file
    json_t* jsonSettings = json_object();	// "Settings" JSON object

	/* New field that are added to the class must be dumped from here */
    json_object_set_new(jsonSettings, "background_music", json_boolean(_bBackgroundMusic));
    json_object_set_new(jsonSettings, "rumble", json_boolean(_bRumble));
	
	// Attach the settings to the root
    json_object_set_new(jsonRoot, "settings", jsonSettings);

	// Store the JSON settings on disk
    if (json_dump_file(jsonRoot, sPath.c_str(), JSON_INDENT(4) | JSON_SORT_KEYS) == -1)
		throw std::ios_base::failure("I/O Error");

	// Free the objects from memory
    json_decref(jsonRoot);
}
