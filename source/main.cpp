#include <iostream>
#include <iomanip>
#include <ios>
#include <cstdlib>
#include <cmath>
#include <sys/dir.h>
#include <cstdio>
#include <unistd.h>

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
//#include <asndlib.h>
//#include <mp3player.h>
#include <aesndlib.h>
#include <gcmodplay.h>

#include "../include/util.hpp"
#include "../include/SETTINGS.hpp"
#include "../include/JPEG.hpp"
#include "../include/MACROS.hpp"
#include "../include/PORTS.hpp"
#include "../include/DRAW.hpp"
//#include "../build/sample_mp3.h"
#include "../build/technique_mod.h"
#include "../build/no_jpg.h"
#include "../build/yes_jpg.h"


void Initialise();
void FAT_Initialise();
void Settings_Initialise(const char* sFilePath = Settings::SCsDefaultPath);
void PrepareExit();
void Die(const char* pcMsg);


static void* SapXfb[2]{};					// Pointer to the XFB region
static GXRModeObj* SpGXRmode{nullptr};		// Rendermode object holding the rendering parameters
static MODPlay SMODPlay{};					// Playmode object for playing sounds
static Settings Ssettings{};				// Global configuration object


//-----------------------------------  ISR -----------------------------------------
void ISR_PowerButton() 
{
	PrepareExit();
	SYS_ResetSystem(SYS_POWEROFF, 0, 0);
}


void ISR_WiimotePowerButton(int32_t iChan)
{
	PrepareExit();
	SYS_ResetSystem(SYS_POWEROFF, 0, 0);
}


void ISR_ResetButton(uint32_t uiIRQ, void* pContext)
{
	PrepareExit();
	SYS_ResetSystem(SYS_HOTRESET, 0, 0);
}


//----------------------------------- MAIN ----------------------------------------
int main(int argc, char** argv)
{
	Initialise();
	FAT_Initialise();

	Settings_Initialise(Settings::SCsDefaultPath);

	uint8_t uyXFB{};
	JPEG imageNo{no_jpg, no_jpg_size};
	JPEG imageYes{yes_jpg, yes_jpg_size};

	WPADData* pWPADData1{nullptr};
	uint32_t uiExpansionType{WPAD_EXP_NONE};

	MODPlay_SetMOD(&SMODPlay, technique_mod);
	MODPlay_SetVolume(&SMODPlay, 63, 63);

	//-------------------------------- Superloop ---------------------------------
	while(true) 
	{
		// Initialise the console, required for printf
		CON_Init(SapXfb[uyXFB], 20, 20, SpGXRmode->fbWidth, SpGXRmode->xfbHeight, 
			SpGXRmode->fbWidth * VI_DISPLAY_PIX_SZ);

		// The console understands VT terminal escape codes
		// This positions the cursor on row 2, column 0
		// we can use variables for this with format codes too
		// e.g. printf ("\x1b[%d;%dH", row, column );
		std::printf("\x1b[2;0H");

		//VIDEO_ClearFrameBuffer(SpGXRmode, SapXfb[uyXFB], COLOR_BLACK);	// Clears the screen completely
		if (!Ssettings.getBackgroundMusic())		// Music off - show a "no" button
		{
			imageNo.Display(SapXfb[uyXFB], SpGXRmode, SpGXRmode->fbWidth, SpGXRmode->xfbHeight, 
				(SpGXRmode->fbWidth - imageNo.GetWidth()) >> 1, 
				(SpGXRmode->xfbHeight - imageNo.GetHeight()) >> 1);

			//if (MP3Player_IsPlaying()) MP3Player_Stop();
			if (SMODPlay.playing && !SMODPlay.paused) MODPlay_Pause(&SMODPlay, true);
		}
		else	// Music on - show a "yes" button
		{
			imageYes.Display(SapXfb[uyXFB], SpGXRmode, SpGXRmode->fbWidth, SpGXRmode->xfbHeight,
				(SpGXRmode->fbWidth - imageYes.GetWidth()) >> 1, 
				(SpGXRmode->xfbHeight - imageYes.GetHeight()) >> 1);

			//if (!MP3Player_IsPlaying()) MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, nullptr);
			if (!SMODPlay.playing) MODPlay_Start(&SMODPlay);
			else if (SMODPlay.paused) MODPlay_Pause(&SMODPlay, false);
		}
		std::printf("Background music = %i Rumble = %i\n\n", Ssettings.getBackgroundMusic(), 
			Ssettings.getRumble());


		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		if (WPAD_Probe(WPAD_CHAN_0, &uiExpansionType) == WPAD_ERR_NONE)
		{
			pWPADData1 = WPAD_Data(WPAD_CHAN_0);

			// Draw the cursor
			/*if (pWPADData1->ir.valid) 
				DRAW_box(SapXfb[uyXFB], SpGXRmode, SpGXRmode->fbWidth, SpGXRmode->xfbHeight, pWPADData1->ir.x - 5, 
					pWPADData1->ir.y - 5, pWPADData1->ir.x + 5, pWPADData1->ir.y + 5, COLOR_WHITE);*/

			// Rumble on-off
			if (!Ssettings.getRumble()) WPAD_Rumble(WPAD_CHAN_0, 0);
			else WPAD_Rumble(WPAD_CHAN_0, 1);

			if (pWPADData1->btns_d)
			{
				// We return to the launcher application via exit
				if (pWPADData1->btns_d & WPAD_BUTTON_HOME) 
				{
					PrepareExit();
					std::exit(EXIT_SUCCESS);
				}
				else if (pWPADData1->btns_d & WPAD_BUTTON_1) TOGGLE(HW_GPIOB_OUT, SLOT_LED);
				else if (pWPADData1->btns_d & WPAD_BUTTON_2) TOGGLE(HW_GPIOB_OUT, DO_EJECT);
				else if (pWPADData1->btns_d & WPAD_BUTTON_A && pWPADData1->ir.valid)
				{
					// Change the settings and save them on disk when clicking the button
					if ((Ssettings.getBackgroundMusic() && pWPADData1->ir.x >= imageYes.GetPosX() && 
						pWPADData1->ir.x < imageYes.GetPosX() + imageYes.GetWidth() && 
						pWPADData1->ir.y >= imageYes.GetPosY() && 
						pWPADData1->ir.y < imageYes.GetPosY() + imageYes.GetHeight()) ||
						(!Ssettings.getBackgroundMusic() && pWPADData1->ir.x >= imageNo.GetPosX() && 
						pWPADData1->ir.x < imageNo.GetPosX() + imageNo.GetWidth() && 
						pWPADData1->ir.y >= imageNo.GetPosY() && 
						pWPADData1->ir.y < imageNo.GetPosY() + imageNo.GetHeight()))
					{
						Ssettings.setBackgroundMusic(!Ssettings.getBackgroundMusic());
						Ssettings.setRumble(!Ssettings.getRumble());
						Ssettings.save(Settings::SCsDefaultPath);
					}
				}
			}

			PrintWiimoteData(SapXfb[uyXFB], SpGXRmode, pWPADData1);
		}
		
		VIDEO_SetNextFramebuffer(SapXfb[uyXFB]);
		uyXFB ^= 1;
		VIDEO_Flush();

		// Wait for the next frame
		VIDEO_WaitVSync();
	}
}


/**
 * @brief Initialise essential subsystems
 * 
 */
void Initialise()
{
	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Initialise the audio subsystem
	//ASND_Init();
	//MP3Player_Init();
	AESND_Init();
	MODPlay_Init(&SMODPlay);

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	SpGXRmode = VIDEO_GetPreferredMode(nullptr);

	// Allocate memory for the display in the uncached region
	SapXfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(SpGXRmode));
	SapXfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(SpGXRmode));

	// Set up the video registers with the chosen mode
	VIDEO_Configure(SpGXRmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(SapXfb[0]);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if (SpGXRmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

	WPAD_SetIdleTimeout(300);
	WPAD_SetVRes(WPAD_CHAN_ALL, SpGXRmode->fbWidth, SpGXRmode->xfbHeight);
	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);

	SYS_SetPowerCallback(ISR_PowerButton);
	SYS_SetResetCallback(ISR_ResetButton);
	WPAD_SetPowerButtonCallback(ISR_WiimotePowerButton);
}


/**
 * @brief Initialise FAT subsystem
 * 
 */
void FAT_Initialise()
{
	if (!fatInitDefault()) Die("fatInitDefault failure: terminating\n");

	DIR* pDir{opendir("/")};
	if (!pDir) Die("opendir() failure; terminating\n");

	if (chdir("/")) Die("Could not change to root directory, exiting.\n");

	closedir(pDir);
}


/**
 * @brief Loads global configuration
 * 
 * @param sFilePath the path to the configuration in the filesystem
 */
void Settings_Initialise(const char* sFilePath)
{
	try { Ssettings = Settings(sFilePath); }
	catch (std::ios_base::failure& iof) { Ssettings.save(sFilePath); }
}


void PrepareExit()
{
	//if (MP3Player_IsPlaying()) MP3Player_Stop();
	if (SMODPlay.playing) MODPlay_Stop(&SMODPlay);
	WPAD_Shutdown();
}


/**
 * @brief Early exits the application
 * 
 * @param pcMsg error message to print on the screen
 */
void Die(const char* pcMsg)
{
	std::perror(pcMsg);
	sleep(5);

	PrepareExit();
	std::exit(EXIT_FAILURE);
}