#include <iostream>
#include <iomanip>
#include <ios>
#include <cstdlib>
#include <string>
#include <cmath>
#include <dirent.h>
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


void initialise();
void initialise_fat();
void load_settings(const std::string& sFilePath = Settings::SCsDefaultPath);
void prepare_exit();
void die(const char* pcMsg);


static void* SapXfb[2];					// Pointer to the XFB region
static GXRModeObj* SpGXRmode = nullptr;		// Rendermode object holding the rendering parameters
static MODPlay SMODPlay;					// Playmode object for playing sounds
static Settings Ssettings{};				// Global configuration object


//-----------------------------------  ISR -----------------------------------------
void ISR_PowerButton() 
{
	prepare_exit();
	SYS_ResetSystem(SYS_POWEROFF, 0, 0);
}


void ISR_WiimotePowerButton(int32_t iChan)
{
	prepare_exit();
	SYS_ResetSystem(SYS_POWEROFF, 0, 0);
}


void ISR_ResetButton(uint32_t iIRQ, void* pContext)
{
	prepare_exit();
	SYS_ResetSystem(SYS_HOTRESET, 0, 0);
}


//----------------------------------- MAIN ----------------------------------------
int main(int argc, char** argv)
{
	initialise();
	initialise_fat();

	load_settings(Settings::SCsDefaultPath);

	uint8_t yXFB = 0;
	JPEG imageNo(no_jpg, no_jpg_size);
	JPEG imageYes(yes_jpg, yes_jpg_size);

	WPADData* pWPADData1 = nullptr;
	uint32_t iExpansionType = WPAD_EXP_NONE;

	MODPlay_SetMOD(&SMODPlay, technique_mod);
	MODPlay_SetVolume(&SMODPlay, 63, 63);

	//-------------------------------- Superloop ---------------------------------
	while(true) 
	{
		// Initialise the console, required for printf
		CON_Init(SapXfb[yXFB], 20, 20, SpGXRmode->fbWidth, SpGXRmode->xfbHeight, 
			SpGXRmode->fbWidth * VI_DISPLAY_PIX_SZ);

		// The console understands VT terminal escape codes
		// This positions the cursor on row 2, column 0
		// we can use variables for this with format codes too
		// e.g. printf ("\x1b[%d;%dH", row, column );
		std::cout << "\x1b[2;0H";

		//VIDEO_ClearFrameBuffer(SpGXRmode, SapXfb[yXFB], COLOR_BLACK);	// Clears the screen completely
		if (!Ssettings.getBackgroundMusic())		// Music off - show a "no" button
		{
			imageNo.display(SapXfb[yXFB], SpGXRmode, SpGXRmode->fbWidth, SpGXRmode->xfbHeight, 
				(SpGXRmode->fbWidth - imageNo.getWidth()) >> 1, 
				(SpGXRmode->xfbHeight - imageNo.getHeight()) >> 1);

			//if (MP3Player_IsPlaying()) MP3Player_Stop();
			if (SMODPlay.playing && !SMODPlay.paused) MODPlay_Pause(&SMODPlay, true);
		}
		else	// Music on - show a "yes" button
		{
			imageYes.display(SapXfb[yXFB], SpGXRmode, SpGXRmode->fbWidth, SpGXRmode->xfbHeight,
				(SpGXRmode->fbWidth - imageYes.getWidth()) >> 1, 
				(SpGXRmode->xfbHeight - imageYes.getHeight()) >> 1);

			//if (!MP3Player_IsPlaying()) MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, nullptr);
			if (!SMODPlay.playing) MODPlay_Start(&SMODPlay);
			else if (SMODPlay.paused) MODPlay_Pause(&SMODPlay, false);
		}
		std::cout << "Background music = " << Ssettings.getBackgroundMusic() << " Rumble = " << 
			Ssettings.getRumble() << std::endl << std::endl;


		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		if (WPAD_Probe(WPAD_CHAN_0, &iExpansionType) == WPAD_ERR_NONE)
		{
			pWPADData1 = WPAD_Data(WPAD_CHAN_0);

			// Draw the cursor
			/*if (pWPADData1->ir.valid) 
				DRAW_box(SapXfb[yXFB], SpGXRmode, SpGXRmode->fbWidth, SpGXRmode->xfbHeight, pWPADData1->ir.x - 5, 
					pWPADData1->ir.y - 5, pWPADData1->ir.x + 5, pWPADData1->ir.y + 5, COLOR_WHITE);*/

			// Rumble on-off
			if (!Ssettings.getRumble()) WPAD_Rumble(WPAD_CHAN_0, 0);
			else WPAD_Rumble(WPAD_CHAN_0, 1);

			if (pWPADData1->btns_d)
			{
				// We return to the launcher application via exit
				if (pWPADData1->btns_d & WPAD_BUTTON_HOME) 
				{
					prepare_exit();
					exit(EXIT_SUCCESS);
				}
				else if (pWPADData1->btns_d & WPAD_BUTTON_1) TOGGLE(HW_GPIOB_OUT, SLOT_LED);
				else if (pWPADData1->btns_d & WPAD_BUTTON_2) TOGGLE(HW_GPIOB_OUT, DO_EJECT);
				else if (pWPADData1->btns_d & WPAD_BUTTON_A && pWPADData1->ir.valid)
				{
					// Change the settings and save them on disk when clicking the button
					if ((Ssettings.getBackgroundMusic() && pWPADData1->ir.x >= imageYes.getPosX() && 
						pWPADData1->ir.x < imageYes.getPosX() + imageYes.getWidth() && 
						pWPADData1->ir.y >= imageYes.getPosY() && 
						pWPADData1->ir.y < imageYes.getPosY() + imageYes.getHeight()) ||
						(!Ssettings.getBackgroundMusic() && pWPADData1->ir.x >= imageNo.getPosX() && 
						pWPADData1->ir.x < imageNo.getPosX() + imageNo.getWidth() && 
						pWPADData1->ir.y >= imageNo.getPosY() && 
						pWPADData1->ir.y < imageNo.getPosY() + imageNo.getHeight()))
					{
						Ssettings.setBackgroundMusic(!Ssettings.getBackgroundMusic());
						Ssettings.setRumble(!Ssettings.getRumble());
						Ssettings.save(Settings::SCsDefaultPath);
					}
				}
			}

			print_wiimote_data(SapXfb[yXFB], SpGXRmode, pWPADData1);
		}
		
		VIDEO_SetNextFramebuffer(SapXfb[yXFB]);
		yXFB ^= 1;
		VIDEO_Flush();

		// Wait for the next frame
		VIDEO_WaitVSync();
	}
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
	if(SpGXRmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

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
	try { Ssettings = Settings(sFilePath); }
	catch (std::ios_base::failure& iof) { Ssettings.save(sFilePath); }
}


void prepare_exit()
{
	//if (MP3Player_IsPlaying()) MP3Player_Stop();
	if (SMODPlay.playing) MODPlay_Stop(&SMODPlay);
	WPAD_Shutdown();
	fatUnmount(0);
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

	prepare_exit();
	exit(EXIT_FAILURE);
}