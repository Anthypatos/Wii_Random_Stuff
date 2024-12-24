#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <ogc/color.h>
#include <ogc/gx_struct.h>
#include <wiiuse/wpad.h>

#include <SDL_video.h>
#include <SDL_joystick.h>
#include <SDL_events.h>

#ifdef __wii__
	#include <ogc/gx_struct.h>
	#include <ogc/video_types.h>
	#include <ogc/consol.h>
#endif

#include "../include/util.hpp"
#include "../include/DRAW.hpp"


void PrintWiimoteData(void* pXfb, const GXRModeObj* CpGXRmode, WPADData* pWPADData)
{
	std::printf("Data->Err: %i\n\n", pWPADData->err);
	
	/*std::printf("IR Dots: \n");
	for(int32_t i = 0; i < 4; i++) 
	{
		if(pWPADData->ir.dot[i].visible) 
		{
			std::printf("%4d, %3d\n", pWPADData->ir.dot[i].rx, pWPADData->ir.dot[i].ry);
			DRAW_dot(xfb, rmode, 1024, 768, pWPADData->ir.dot[i].rx - 2, pWPADData->ir.dot[i].ry - 2, 
				pWPADData->ir.dot[i].rx + 2, pWPADData->ir.dot[i].ry + 2, COLOR_YELLOW);
		} 
		else std::printf("None\n");
	}
	std::printf("\n");*/
	
	// ir.valid - TRUE is the wiimote is pointing at the screen, else it is false
	if(pWPADData->ir.valid) 
	{
		//float fTheta = pWPADData->ir.angle / 180.0 * M_PI;
	
		// ir.x/ir.y - The x/y coordinates that the wiimote is pointing to, relative to the screen.
		// ir.angle - how far (in degrees) the wiimote is twisted (based on ir)
		std::printf("Cursor: %.02f,%.02f\n", pWPADData->ir.x, pWPADData->ir.y);
		std::printf("%.02f deg\n\n", pWPADData->ir.angle);
		
		DRAW_dot(pXfb, CpGXRmode, CpGXRmode->fbWidth, CpGXRmode->xfbHeight, pWPADData->ir.x - 2, 
            pWPADData->ir.y - 2, pWPADData->ir.x + 2, pWPADData->ir.y + 2, COLOR_RED);
		/*DRAW_dot(xfb, rmode, rmode->fbWidth, rmode->xfbHeight, pWPADData->ir.x - 2 + 10 * sinf(fTheta), 
			pWPADData->ir.y - 2 - 10 * cosf(fTheta), pWPADData->ir.x + 2 + 10 * sinf(fTheta), 
			pWPADData->ir.y + 2 - 10 * cosf(fTheta), COLOR_BLUE);*/
	} 
	else std::printf("No Cursor\n\n");
	
	if(pWPADData->ir.raw_valid) 
	{
		// ir.z - How far away the wiimote is from the screen in meters
		std::printf("Distance: %.02fm\n", pWPADData->ir.z);
		//orient.yaw - The left/right angle of the wiimote to the screen 
		std::printf("Yaw: %.02f deg\n\n", pWPADData->orient.yaw);

		/*for(int32_t i = 0; i < 2; i++) 
		{
			std::printf("%i, %i\n", pWPADData->ir.sensorbar.rot_dots[i].x, 
				pWPADData->ir.sensorbar.rot_dots[i].y);
			DRAW_dot(pXfb, CpGXRmode, 4, 4, pWPADData->ir.sensorbar.rot_dots[i].x + 2, 
				pWPADData->ir.sensorbar.rot_dots[i].y + 2, pWPADData->ir.sensorbar.rot_dots[i].x + 2, 
				pWPADData->ir.sensorbar.rot_dots[i].y + 2, COLOR_GREEN);
		}*/
	} 
	else std::printf("\n\n");

	std::printf("Acceleration:\n");
	// accel.x/accel.y/accel.z - analog values for the accelleration of the wiimote
	// (Note: Gravity pulls downwards, so even if the wiimote is not moving, 
	// one(or more) axis will have a reading as if it is moving "upwards")
	std::printf("XYZ: %3d,%3d,%3d\n", pWPADData->accel.x, pWPADData->accel.y, pWPADData->accel.z);
	// orient.pitch - how far the wiimote is "tilted" in degrees
	std::printf("Pitch: %f deg\n", pWPADData->orient.pitch);
	// orient.roll - how far the wiimote is "twisted" in degrees (uses accelerometer)
	std::printf("Roll: %f deg\n", pWPADData->orient.roll);
}


void StateInGame(SDL_Surface* pSdlSurfaceScreen, SDL_Joystick** apJoysticks)
{
	SDL_FillRect(pSdlSurfaceScreen, nullptr, SDL_MapRGB(pSdlSurfaceScreen->format, 35, 75, 0));

	#ifdef __wii__
		CON_Init(pSdlSurfaceScreen->pixels, 20, 20, pSdlSurfaceScreen->w, pSdlSurfaceScreen->h, 
			pSdlSurfaceScreen->w * VI_DISPLAY_PIX_SZ);
		std::printf("\x1b[2;0H");
	#endif
	
	// Joysticks' properties can be fetched
	for (uint8_t i = 0; i < SDL_NumJoysticks(); i++)
		std::printf("JOYPAD %i\nHAS %i AXES\nAND %i BUTTONS\nAND %i BALLS\nAND %i HATS", i,
			SDL_JoystickNumAxes(apJoysticks[i]), SDL_JoystickNumButtons(apJoysticks[i]),
			SDL_JoystickNumBalls(apJoysticks[i]), SDL_JoystickNumHats(apJoysticks[i]));
			
	SDL_JoystickUpdate();	// Update joysticks' status

	SDL_Event sdlEvent;
	uint8_t uyAxisWhich = 0, uyAxisIndex = 0;
	int16_t rAxisValue = 0;
	uint16_t urMouseX = 0, urMouseY = 0;

	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
			// Checking button status is almost like handling keyboard events
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:

				switch (sdlEvent.jbutton.button)
				{
					case 0:	std::printf("A "); 		break;
					case 1:	std::printf("B "); 		break;
					case 2:	std::printf("1 "); 		break;
					case 3:	std::printf("2 "); 		break;
					case 4:	std::printf("MINUS "); 	break;
					case 5:	std::printf("PLUS "); 	break;
					case 6:	std::printf("HOME "); 	break;
					case 7:	std::printf("Z "); 		break;
					case 8:	std::printf("C "); 		break;
					default: 						break;
				}
				std::printf("%s AND %s", (sdlEvent.type == SDL_JOYBUTTONDOWN ? "DOWN" : "UP"),
					(sdlEvent.jbutton.state == SDL_PRESSED ? "PRESSED" : "RELEASED"));

				break;
			
			// An axis motion 
			case SDL_JOYAXISMOTION:  
				if (sdlEvent.jaxis.value < -3200 || sdlEvent.jaxis.value > 3200) 
				{
					uyAxisWhich = sdlEvent.jaxis.which;
					uyAxisIndex = sdlEvent.jaxis.axis;
					rAxisValue = sdlEvent.jaxis.value;

					if (uyAxisIndex == 0) 
					{
						/* Left and right movement (according to sdlEvent.jaxis.value) of the
							nunchuck stick or left analogic stick of the classic pad*/
					}
					else if (uyAxisIndex == 1) 
					{
						/* Up and Down movement (according to sdlEvent.jaxis.value) of the 
							nunchuck stick or left analogic stick of the classic pad */
					}
				}
				break;

			// Infra Red is mapped as a mouse
			case SDL_MOUSEMOTION:
				urMouseX = sdlEvent.motion.x; 
				urMouseY = sdlEvent.motion.y;
				break;

			case SDL_QUIT:
				SDL_FillRect(pSdlSurfaceScreen, 0, SDL_MapRGB(pSdlSurfaceScreen->format, 0, 0, 0));
				exit(EXIT_SUCCESS);
				break;
		}
	}	// End of event pool
	
	std::printf("IR X %i IR Y %i\nJOYAXISMOTION OF PAD %i\nAXIS %i\nVALUE %i\n", urMouseX, urMouseY,
		uyAxisWhich, uyAxisIndex, rAxisValue);
	
	// The hat reports the directional PAD's status 
	for (uint8_t i = 0; i < SDL_NumJoysticks(); i++)
	{
		uint8_t uyHats = SDL_JoystickNumHats(apJoysticks[i]);
		for (uint8_t j = 0; j < uyHats; j++)
		{
			std::printf("HAT %i ", j);

			uint8_t uyJoystate = SDL_JoystickGetHat(apJoysticks[i], j);
			switch (uyJoystate)
			{
				case SDL_HAT_CENTERED: 		std::printf("CENTERED "); 	break;
				case SDL_HAT_UP: 			std::printf("UP "); 		break;
				case SDL_HAT_RIGHT: 		std::printf("RIGHT "); 		break;
				case SDL_HAT_DOWN: 			std::printf("DOWN "); 		break;
				case SDL_HAT_LEFT: 			std::printf("LEFT "); 		break;
				case SDL_HAT_RIGHTUP: 		std::printf("RIGHTUP "); 	break;
				case SDL_HAT_RIGHTDOWN: 	std::printf("RIGHTDOWN ");	break;
				case SDL_HAT_LEFTUP: 		std::printf("LEFTUP "); 	break;
				case SDL_HAT_LEFTDOWN: 		std::printf("LEFTDOWN "); 	break;
			}
		}
		
		// Axis events can be fetched with the SDL_JoystickGetAxis function
		std::printf("JOYSTICKGETAXIS\nX MOVE %i Y MOVE %i\n", SDL_JoystickGetAxis(apJoysticks[i], 0),
			SDL_JoystickGetAxis(apJoysticks[i], 1));
	}
}
