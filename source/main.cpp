#include <iostream>
#include <ios>
#include <cstdlib>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
//#include <asndlib.h>
//#include <mp3player.h>
#include <aesndlib.h>
#include <gcmodplay.h>
#include "../include/SETTINGS.hpp"
#include "../include/JPEG.hpp"
#include "../include/MACROS.hpp"
#include "../include/PORTS.hpp"
#include "../include/DRAW.hpp"
//#include "../build/sample_mp3.h"
#include "../build/technique_mod.h"
#include "../build/no_jpg.h"
#include "../build/yes_jpg.h"


void initialise();
void initialise_fat();
void load_settings(const std::string& sFilePath = Settings::SCsDefaultPath);
void die(const char* pcMsg);


static void* xfb[2];					// Pointer to the XFB region
static GXRModeObj *rmode = nullptr;		// Rendermode object holding the rendering parameters
static MODPlay play;					// Playmode object for playing sounds
static Settings settings{};				// Global configuration object
static vs8 HWButton = -1;


//-----------------------------------  ISR -----------------------------------------
void ISR_PowerButton() { HWButton = SYS_POWEROFF; }
void ISR_WiimotePowerButton(s32 iChan) { HWButton = SYS_POWEROFF; }
void ISR_ResetButton(u32 iIRQ, void* pContext) { HWButton = SYS_HOTRESET; }


//----------------------------------- MAIN ----------------------------------------
int main(int argc, char** argv)
{
	initialise();
	initialise_fat();

	u8 iXFB = 0;
	ir_t ir;
	JPEG imageNo(no_jpg, no_jpg_size);
	JPEG imageYes(yes_jpg, yes_jpg_size);
	load_settings(Settings::SCsDefaultPath);

	MODPlay_SetMOD(&play, technique_mod);
	MODPlay_SetVolume(&play, 63, 63);

	while(true) 
	{
		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();
		
		// IR Movement
		WPAD_IR(WPAD_CHAN_0, &ir);

		// Initialise the console, required for printf
		console_init(xfb[iXFB], 20, 20, rmode->fbWidth, rmode->xfbHeight, 
			rmode->fbWidth * VI_DISPLAY_PIX_SZ);
		//VIDEO_ClearFrameBuffer(rmode, xfb[iXFB], COLOR_BLACK);	// Clears the screen completely
		if (!settings.getBackgroundMusic())		// Music off - show a "no" button
		{
			imageNo.display(xfb[iXFB], rmode, rmode->fbWidth, rmode->xfbHeight, 
				(rmode->fbWidth - imageNo.getWidth()) >> 1, 
				(rmode->xfbHeight - imageNo.getHeight()) >> 1);

			//if (MP3Player_IsPlaying()) MP3Player_Stop();
			if (play.playing && !play.paused) MODPlay_Pause(&play, true);
		}
		else	// Music on - show a "yes" button
		{
			imageYes.display(xfb[iXFB], rmode, rmode->fbWidth, rmode->xfbHeight,
				(rmode->fbWidth - imageYes.getWidth()) >> 1, 
				(rmode->xfbHeight - imageYes.getHeight()) >> 1);

			//if (!MP3Player_IsPlaying()) MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, nullptr);
			if (!play.playing) MODPlay_Start(&play);
			else if (play.paused) MODPlay_Pause(&play, false);
		}
		// Draw the cursor
		if (ir.valid) 
			DRAW_box(xfb[iXFB], rmode, rmode->fbWidth, rmode->xfbHeight, ir.x - 5, ir.y - 5, 
				ir.x + 5, ir.y + 5, COLOR_WHITE);
		
		// Rumble on-off
		if (!settings.getRumble()) WPAD_Rumble(WPAD_CHAN_0, 0);
		else WPAD_Rumble(WPAD_CHAN_0, 1);
		
		std::cout << "\x1b[2;0Hx = " << ir.x << " y = " << ir.y << std::endl <<
		"Background music = " << settings.getBackgroundMusic() << " Rumble = " << settings.getRumble();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 iButtonsDown = WPAD_ButtonsDown(WPAD_CHAN_0);

		if (iButtonsDown)
		{
			// We return to the launcher application via exit
			if (iButtonsDown & WPAD_BUTTON_HOME) 
			{
				//if (MP3Player_IsPlaying()) MP3Player_Stop();
				if (play.playing) MODPlay_Stop(&play);
				fatUnmount(0);
				exit(0);
			}
			else if (iButtonsDown & WPAD_BUTTON_1) TOGGLE(HW_GPIOB_OUT, SLOT_LED);
			else if (iButtonsDown & WPAD_BUTTON_2) TOGGLE(HW_GPIOB_OUT, DO_EJECT);
			else if (iButtonsDown & WPAD_BUTTON_A && ir.valid)
			{
				// Change the settings and save them on disk when clicking the button
				if ((settings.getBackgroundMusic() && ir.x >= imageYes.getPosX() && 
					ir.x < (imageYes.getPosX() + imageYes.getWidth()) && ir.y >= imageYes.getPosY() && 
					ir.y < (imageYes.getPosY() + imageYes.getHeight())) ||
					(!settings.getBackgroundMusic() && ir.x >= imageNo.getPosX() && 
					ir.x < (imageNo.getPosX() + imageNo.getWidth()) && ir.y >= imageNo.getPosY() && 
					ir.y < (imageNo.getPosY() + imageNo.getHeight())))
				{
					settings.setBackgroundMusic(!settings.getBackgroundMusic());
					settings.setRumble(!settings.getRumble());
					settings.save(Settings::SCsDefaultPath);
				}
			}
		}

		if (HWButton != -1)
		{
			if (play.playing) MODPlay_Stop(&play);
			fatUnmount(0);
			SYS_ResetSystem(HWButton, 0, 0);
		}
		
		VIDEO_SetNextFramebuffer(xfb[iXFB]);
		iXFB ^= 1;
		VIDEO_Flush();

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}


/**
 * @brief Initialise essential subsystems
 * 
 */
void initialise()
{
	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Initialise the audio subsystem
	//ASND_Init();
	//MP3Player_Init();
	AESND_Init();
	MODPlay_Init(&play);

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(nullptr);

	// Allocate memory for the display in the uncached region
	xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb[0]);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	std::cout << "\x1b[2;0H";

	WPAD_SetVRes(WPAD_CHAN_ALL, rmode->fbWidth, rmode->xfbHeight);
	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);

	SYS_SetPowerCallback(ISR_PowerButton);
	SYS_SetResetCallback(ISR_ResetButton);
	WPAD_SetPowerButtonCallback(ISR_WiimotePowerButton);
}


/**
 * @brief Initialise FAT subsystem
 * 
 */
void initialise_fat()
{
	if (!fatInitDefault()) die("fatInitDefault failure: terminating\n");

	DIR* pDir = opendir("/");
	if (!pDir) die("opendir() failure; terminating\n");

	if (chdir("/")) die("Could not change to root directory, exiting.\n");

	closedir(pDir);
}


/**
 * @brief Loads global configuration
 * 
 * @param sFilePath the path to the configuration in the filesystem
 */
void load_settings(const std::string& sFilePath)
{
	// Loads
	try { settings = Settings(sFilePath); }
	catch (std::ios_base::failure& iof) { settings.save(sFilePath); }
}


/**
 * @brief Early exits the application
 * 
 * @param pcMsg error message to print on the screen
 */
void die(const char* pcMsg)
{
	perror(pcMsg);
	sleep(5);
	fatUnmount(0);
	exit(1);
}