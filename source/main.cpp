#include <iostream>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <dirent.h>
#include <unistd.h>
#include <turbojpeg.h>
#include "display.hpp"
#include "MACROS.h"
#include "PORTS.h"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;


void initialise();
void initialise_fat();
void die(const char* pcMsg);

//---------------------------------------------------------------------------------
int main(int argc, char **argv)
//---------------------------------------------------------------------------------
{
	initialise();
	initialise_fat();

	ir_t irT;

	while(1) 
	{
		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();
		
		// IR Movement
		WPAD_IR(WPAD_CHAN_0, &irT);
		
		VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
		//drawBox(irT.x - 5, irT.y - 5, irT.x + 5, irT.y + 5, COLOR_WHITE);
		display_jpeg("/apps/test/resources/about.jpg", irT.x, irT.y, xfb, rmode);
		
		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 iPressed = WPAD_ButtonsDown(WPAD_CHAN_0);

		if (iPressed)
		{
			// We return to the launcher application via exit
			if (iPressed & WPAD_BUTTON_HOME) 
			{
				fatUnmount(0);
				exit(0);
			}
			if (iPressed & WPAD_BUTTON_1) TOGGLE(HW_GPIOB_OUT, SLOT_LED);
			else if (iPressed & WPAD_BUTTON_2) TOGGLE(HW_GPIOB_OUT, DO_EJECT);
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
	printf("\x1b[2;0H");

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
	fatUnmount(0);
	exit(1);
}