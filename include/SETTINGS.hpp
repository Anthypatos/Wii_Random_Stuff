#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_


class Settings
{
public:
    bool getBackgroundMusic() const noexcept;
    void setBackgroundMusic(bool bBackgroundMusic) noexcept;
    bool getRumble() const noexcept;
    void setRumble(bool bRumble) noexcept;

    explicit Settings(const char* pcfilePath);

private:
    bool _bBackgroundMusic;
    bool _bRumble;
};
inline bool Settings::getBackgroundMusic() const noexcept { return _bBackgroundMusic; }
inline void Settings::setBackgroundMusic(bool bBackgroundMusic) noexcept { _bBackgroundMusic = bBackgroundMusic; }
inline bool Settings::getRumble() const noexcept { return _bRumble; }
inline void Settings::setRumble(bool bRumble) noexcept { _bRumble = bRumble; }


#endif