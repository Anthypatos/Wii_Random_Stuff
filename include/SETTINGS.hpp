/**
	@file		SETTINGS.hpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		29/06/2022
    @brief		Settings class
    @par		Description
                Class for managing application settings
				
*/

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_


#include <string>


/**
 * @brief Class for managing application settings
 */
class Settings
{
public:
    static const std::string SCsDefaultPath;   /**< Default path for storing the application's settings */

    /* Getters and setters */
    bool getBackgroundMusic() const noexcept;
    void setBackgroundMusic(bool bBackgroundMusic) noexcept;
    bool getRumble() const noexcept;
    void setRumble(bool bRumble) noexcept;

    /**
     * @brief Creates an object with the default settings
     */
    Settings() noexcept;

    /**
     * @brief Constructs a new object by reading a settings file
     * @param CsFilePath the path to the JSON file holding the settings
     */
    explicit Settings(const std::string& CsFilePath);

    /**
     * @brief Saves the settings on disk
     * @param CsPath the path where the settings are to be stored
     */
    void save(const std::string& CsPath) const;

private:
    bool _bBackgroundMusic;     /**< Toggles background music */
    bool _bRumble;              /**< Toggles rumble */
    
};


inline bool Settings::getBackgroundMusic() const noexcept { return _bBackgroundMusic; }
inline void Settings::setBackgroundMusic(bool bBackgroundMusic) noexcept { _bBackgroundMusic = bBackgroundMusic; }
inline bool Settings::getRumble() const noexcept { return _bRumble; }
inline void Settings::setRumble(bool bRumble) noexcept { _bRumble = bRumble; }


#endif