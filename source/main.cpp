#include <iostream>
#include <stdexcept>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <dirent.h>
#include <unistd.h>
#include "../include/JPEG.hpp"
#include "../include/MACROS.hpp"
#include "../include/PORTS.hpp"
#include "../include/DRAW.hpp"
#include "../build/no_jpg.h"
#include "../build/yes_jpg.h"


enum State_t {STATE_NO, STATE_YES};

void initialise();
void initialise_fat();
void die(const char* pcMsg);

static void *xfb = nullptr;
static GXRModeObj *rmode = nullptr;

//---------------------------------------------------------------------------------
int main(int argc, char** argv)
//---------------------------------------------------------------------------------
{
	initialise();
	initialise_fat();

	ir_t irT;
	JPEG imageNo(no_jpg, no_jpg_size);
	JPEG imageYes(yes_jpg, yes_jpg_size);
	State_t stateCurrent = STATE_NO;

	while(1) 
	{
		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();
		
		// IR Movement
		WPAD_IR(WPAD_CHAN_0, &irT);
		
		VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);

		switch (stateCurrent)
		{
			case STATE_YES: imageYes.display(320, 240, xfb, rmode); break;
			case STATE_NO: imageNo.display(320, 240, xfb, rmode); break;
		}
		DRAW_box(irT.x - 5, irT.y - 5, irT.x + 5, irT.y + 5, COLOR_WHITE, xfb, rmode);
		
		std::cout << "\x1b[2;0Hx = " << irT.x << " y = " << irT.y;

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 iButtonsDown = WPAD_ButtonsDown(WPAD_CHAN_0);

		if (iButtonsDown)
		{
			// We return to the launcher application via exit
			if (iButtonsDown & WPAD_BUTTON_HOME) exit(0);
			else if (iButtonsDown & WPAD_BUTTON_1) TOGGLE(HW_GPIOB_OUT, SLOT_LED);
			else if (iButtonsDown & WPAD_BUTTON_2) TOGGLE(HW_GPIOB_OUT, DO_EJECT);
			else if (iButtonsDown & WPAD_BUTTON_A)
			{
				switch (stateCurrent)
				{
				case STATE_NO:
					if (irT.x >= imageNo.getPosX() && irT.x < imageNo.getPosX() + imageNo.getWidth() &&
						irT.y >= imageNo.getPosY() && irT.y < imageNo.getPosY() + imageNo.getHeight())
						stateCurrent = STATE_YES;
					break;
				
				case STATE_YES:
					if (irT.x >= imageYes.getPosX() && irT.x < imageYes.getPosX() + imageYes.getWidth() &&
						irT.y >= imageYes.getPosY() && irT.y < imageYes.getPosY() + imageNo.getHeight())
						stateCurrent = STATE_NO;
					break;
				}
			}
		}

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}


void initialise()
{
	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

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

	WPAD_SetVRes(WPAD_CHAN_0, rmode->viWidth, rmode->viHeight);
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
}

void initialise_fat()
{
	if (!fatInitDefault()) die("fatInitDefault failure: terminating\n");

	DIR* pDir = opendir("/");
	if (!pDir) die("opendir() failure; terminating\n");

	if (chdir("/")) die("Could not change to root directory, exiting.\n");

	closedir(pDir);
}


void die(const char* pcMsg)
{
	perror(pcMsg);
	sleep(5);
	exit(1);
}