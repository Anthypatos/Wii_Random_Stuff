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
#include "../include/utils.hpp"
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


static void* xfb[2];					// Pointer to the XFB region
static GXRModeObj *rmode = nullptr;		// Rendermode object holding the rendering parameters
static MODPlay play;					// Playmode object for playing sounds
static Settings settings{};				// Global configuration object
static vs8 yStopProgram = -1;


//-----------------------------------  ISR -----------------------------------------
void ISR_PowerButton() { yStopProgram = SYS_POWEROFF; }
void ISR_WiimotePowerButton(s32 iChan) { yStopProgram = SYS_POWEROFF; }
void ISR_ResetButton(u32 iIRQ, void* pContext) { yStopProgram = SYS_HOTRESET; }


//----------------------------------- MAIN ----------------------------------------
int main(int argc, char** argv)
{
	initialise();
	initialise_fat();

	load_settings(Settings::SCsDefaultPath);

	u8 yXFB = 0;
	JPEG imageNo(no_jpg, no_jpg_size);
	JPEG imageYes(yes_jpg, yes_jpg_size);

	WPADData* pWPADData1 = nullptr;
	u32 iExpansionType = WPAD_EXP_NONE;

	MODPlay_SetMOD(&play, technique_mod);
	MODPlay_SetVolume(&play, 63, 63);

	//-------------------------------- Superloop ---------------------------------
	while(true) 
	{
		// Initialise the console, required for printf
		console_init(xfb[yXFB], 20, 20, rmode->fbWidth, rmode->xfbHeight, 
			rmode->fbWidth * VI_DISPLAY_PIX_SZ);

		// The console understands VT terminal escape codes
		// This positions the cursor on row 2, column 0
		// we can use variables for this with format codes too
		// e.g. printf ("\x1b[%d;%dH", row, column );
		std::cout << "\x1b[2;0H";

		//VIDEO_ClearFrameBuffer(rmode, xfb[yXFB], COLOR_BLACK);	// Clears the screen completely
		if (!settings.getBackgroundMusic())		// Music off - show a "no" button
		{
			imageNo.display(xfb[yXFB], rmode, rmode->fbWidth, rmode->xfbHeight, 
				(rmode->fbWidth - imageNo.getWidth()) >> 1, 
				(rmode->xfbHeight - imageNo.getHeight()) >> 1);

			//if (MP3Player_IsPlaying()) MP3Player_Stop();
			if (play.playing && !play.paused) MODPlay_Pause(&play, true);
		}
		else	// Music on - show a "yes" button
		{
			imageYes.display(xfb[yXFB], rmode, rmode->fbWidth, rmode->xfbHeight,
				(rmode->fbWidth - imageYes.getWidth()) >> 1, 
				(rmode->xfbHeight - imageYes.getHeight()) >> 1);

			//if (!MP3Player_IsPlaying()) MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, nullptr);
			if (!play.playing) MODPlay_Start(&play);
			else if (play.paused) MODPlay_Pause(&play, false);
		}
		std::cout << "Background music = " << settings.getBackgroundMusic() << " Rumble = " << 
			settings.getRumble() << std::endl << std::endl;


		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		if (WPAD_Probe(WPAD_CHAN_0, &iExpansionType) == WPAD_ERR_NONE)
		{
			pWPADData1 = WPAD_Data(WPAD_CHAN_0);

			// Draw the cursor
			/*if (pWPADData1->ir.valid) 
				DRAW_box(xfb[yXFB], rmode, rmode->fbWidth, rmode->xfbHeight, pWPADData1->ir.x - 5, 
					pWPADData1->ir.y - 5, pWPADData1->ir.x + 5, pWPADData1->ir.y + 5, COLOR_WHITE);*/

			// Rumble on-off
			if (!settings.getRumble()) WPAD_Rumble(WPAD_CHAN_0, 0);
			else WPAD_Rumble(WPAD_CHAN_0, 1);

			if (pWPADData1->btns_d)
			{
				// We return to the launcher application via exit
				if (pWPADData1->btns_d & WPAD_BUTTON_HOME) 
				{
					prepare_exit();
					exit(0);
				}
				else if (pWPADData1->btns_d & WPAD_BUTTON_1) TOGGLE(HW_GPIOB_OUT, SLOT_LED);
				else if (pWPADData1->btns_d & WPAD_BUTTON_2) TOGGLE(HW_GPIOB_OUT, DO_EJECT);
				else if (pWPADData1->btns_d & WPAD_BUTTON_A && pWPADData1->ir.valid)
				{
					// Change the settings and save them on disk when clicking the button
					if ((settings.getBackgroundMusic() && pWPADData1->ir.x >= imageYes.getPosX() && 
						pWPADData1->ir.x < imageYes.getPosX() + imageYes.getWidth() && 
						pWPADData1->ir.y >= imageYes.getPosY() && 
						pWPADData1->ir.y < imageYes.getPosY() + imageYes.getHeight()) ||
						(!settings.getBackgroundMusic() && pWPADData1->ir.x >= imageNo.getPosX() && 
						pWPADData1->ir.x < imageNo.getPosX() + imageNo.getWidth() && 
						pWPADData1->ir.y >= imageNo.getPosY() && 
						pWPADData1->ir.y < imageNo.getPosY() + imageNo.getHeight()))
					{
						settings.setBackgroundMusic(!settings.getBackgroundMusic());
						settings.setRumble(!settings.getRumble());
						settings.save(Settings::SCsDefaultPath);
					}
				}
			}

			print_wiimote_data(xfb[yXFB], rmode, pWPADData1);
		}

		if (yStopProgram != -1)
		{
			prepare_exit();
			SYS_ResetSystem(yStopProgram, 0, 0);
		}
		
		VIDEO_SetNextFramebuffer(xfb[yXFB]);
		yXFB ^= 1;
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

	WPAD_SetIdleTimeout(300);
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
	try { settings = Settings(sFilePath); }
	catch (std::ios_base::failure& iof) { settings.save(sFilePath); }
}


void prepare_exit()
{
	//if (MP3Player_IsPlaying()) MP3Player_Stop();
	if (play.playing) MODPlay_Stop(&play);
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
	exit(1);
}