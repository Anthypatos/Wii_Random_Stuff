#include <iostream>
#include <iomanip>
#include <cstdlib>

#include <ogc/color.h>
#include <ogc/gx_struct.h>
#include <wiiuse/wpad.h>

#include <SDL_video.h>
#include <SDL_joystick.h>
#include <SDL_events.h>

#ifdef HW_RVL
	#include <ogc/gx_struct.h>
	#include <ogc/video_types.h>
	#include <ogc/consol.h>
#endif

#include "../include/util.hpp"
#include "../include/DRAW.hpp"


void print_wiimote_data(void* pXfb, const GXRModeObj* CpGXRmode, WPADData* pWPADData)
{
	std::cout << "Data->Err: " << pWPADData->err << std::endl << std::endl;
	
	/*std::cout << "IR Dots: " << std::endl;
	for(int32_t i = 0; i < 4; i++) 
	{
		if(pWPADData->ir.dot[i].visible) 
		{
			std::cout << std::setw(3) << pWPADData->ir.dot[i].rx << ", " << pWPADData->ir.dot[i].ry << 
				std::endl;
			DRAW_dot(xfb, rmode, 1024, 768, pWPADData->ir.dot[i].rx - 2, pWPADData->ir.dot[i].ry - 2, 
				pWPADData->ir.dot[i].rx + 2, pWPADData->ir.dot[i].ry + 2, COLOR_YELLOW);
		} 
		else std::cout << "None" << std::endl;
	}
	std::cout << std::endl;*/
	
	// ir.valid - TRUE is the wiimote is pointing at the screen, else it is false
	if(pWPADData->ir.valid) 
	{
		//float fTheta = pWPADData->ir.angle / 180.0 * M_PI;
	
		// ir.x/ir.y - The x/y coordinates that the wiimote is pointing to, relative to the screen.
		// ir.angle - how far (in degrees) the wiimote is twisted (based on ir)
		std::cout << "Cursor: " << std::fixed << std::setprecision(2) << pWPADData->ir.x << ", " << 
			pWPADData->ir.y << std::endl;
		std::cout << pWPADData->ir.angle << " deg" << std::endl << std::endl;
		
		DRAW_dot(pXfb, CpGXRmode, CpGXRmode->fbWidth, CpGXRmode->xfbHeight, pWPADData->ir.x - 2, 
            pWPADData->ir.y - 2, pWPADData->ir.x + 2, pWPADData->ir.y + 2, COLOR_RED);
		/*DRAW_dot(xfb, rmode, rmode->fbWidth, rmode->xfbHeight, pWPADData->ir.x - 2 + 10 * sinf(fTheta), 
			pWPADData->ir.y - 2 - 10 * cosf(fTheta), pWPADData->ir.x + 2 + 10 * sinf(fTheta), 
			pWPADData->ir.y + 2 - 10 * cosf(fTheta), COLOR_BLUE);*/
	} 
	else std::cout << "No Cursor" << std::endl << std::endl;
	
	if(pWPADData->ir.raw_valid) 
	{
		// ir.z - How far away the wiimote is from the screen in meters
		std::cout << "Distance: " << pWPADData->ir.z << "m" << std::endl;
		//orient.yaw - The left/right angle of the wiimote to the screen 
		std::cout << "Yaw: " << pWPADData->orient.yaw << " deg" << std::endl << std::endl;

		/*for(int32_t i = 0; i < 2; i++) 
		{
			std::cout << pWPADData->ir.sensorbar.rot_dots[i].x << ", " << 
				pWPADData->ir.sensorbar.rot_dots[i].y << std::endl;
			DRAW_dot(pXfb, CpGXRmode, 4, 4, pWPADData->ir.sensorbar.rot_dots[i].x + 2, 
				pWPADData->ir.sensorbar.rot_dots[i].y + 2, pWPADData->ir.sensorbar.rot_dots[i].x + 2, 
				pWPADData->ir.sensorbar.rot_dots[i].y + 2, COLOR_GREEN);
		}*/
	} 
	else std::cout << std::endl << std::endl;

	std::cout << "Acceleration:" << std::endl;
	// accel.x/accel.y/accel.z - analog values for the accelleration of the wiimote
	// (Note: Gravity pulls downwards, so even if the wiimote is not moving, 
	// one(or more) axis will have a reading as if it is moving "upwards")
	std::cout << "XYZ: " << std::setw(3) << pWPADData->accel.x << ", " << pWPADData->accel.y << ", " << 
		pWPADData->accel.z << std::endl << std::endl;
	// orient.pitch - how far the wiimote is "tilted" in degrees
	std::cout << "Pitch: " << pWPADData->orient.pitch << " deg" << std::endl;
	// orient.roll - how far the wiimote is "twisted" in degrees (uses accelerometer)
	std::cout << "Roll: " << pWPADData->orient.roll << " deg" << std::endl;
}


void state_in_game(SDL_Surface* pSdlSurfaceScreen, SDL_Joystick** apJoysticks)
{
	SDL_FillRect(pSdlSurfaceScreen, nullptr, SDL_MapRGB(pSdlSurfaceScreen->format, 35, 75, 0));

	#ifdef HW_RVL
		CON_Init(pSdlSurfaceScreen->pixels, 20, 20, pSdlSurfaceScreen->w, pSdlSurfaceScreen->h, 
			pSdlSurfaceScreen->w * VI_DISPLAY_PIX_SZ);
		std::cout << "\x1b[2;0H";
	#endif
	
	// Joysticks' properties can be fetched
	for (uint8_t i = 0; i < SDL_NumJoysticks(); i++)
		std::cout << "JOYPAD " << i << std::endl << 
			"HAS " << SDL_JoystickNumAxes(apJoysticks[i]) << " AXES" << std::endl << 
			"AND " << SDL_JoystickNumButtons(apJoysticks[i]) << " BUTTONS" << std::endl << 
			"AND " << SDL_JoystickNumBalls(apJoysticks[i]) << " BALLS" << std::endl << 
			"AND " << SDL_JoystickNumHats(apJoysticks[i]) << " HATS";
			
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
					case 0:	std::cout << "A "; 		break;
					case 1:	std::cout << "B "; 		break;
					case 2:	std::cout << "1 "; 		break;
					case 3:	std::cout << "2 "; 		break;
					case 4:	std::cout << "MINUS "; 	break;
					case 5:	std::cout << "PLUS "; 	break;
					case 6:	std::cout << "HOME "; 	break;
					case 7:	std::cout << "Z "; 		break;
					case 8:	std::cout << "C "; 		break;
					default: 						break;
				}
				std::cout << (sdlEvent.type == SDL_JOYBUTTONDOWN ? "DOWN" : "UP") << " AND " << 
					(sdlEvent.jbutton.state == SDL_PRESSED ? "PRESSED" : "RELEASED");

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
	
	std::cout << "IR X " << urMouseX << " IR Y " << urMouseY << std::endl <<
		"JOYAXISMOTION OF PAD " << uyAxisWhich << std::endl << 
		"AXIS " << uyAxisIndex << std::endl << 
		"VALUE " << rAxisValue << std::endl;
	
	// The hat reports the directional PAD's status 
	for (uint8_t i = 0; i < SDL_NumJoysticks(); i++)
	{
		uint8_t uyHats = SDL_JoystickNumHats(apJoysticks[i]);
		for (uint8_t j = 0; j < uyHats; j++)
		{
			std::cout << "HAT " << j << " ";

			uint8_t uyJoystate = SDL_JoystickGetHat(apJoysticks[i], j);
			switch (uyJoystate)
			{
				case SDL_HAT_CENTERED: 		std::cout << "CENTERED "; 	break;
				case SDL_HAT_UP: 			std::cout << "UP "; 		break;
				case SDL_HAT_RIGHT: 		std::cout << "RIGHT "; 		break;
				case SDL_HAT_DOWN: 			std::cout << "DOWN "; 		break;
				case SDL_HAT_LEFT: 			std::cout << "LEFT "; 		break;
				case SDL_HAT_RIGHTUP: 		std::cout << "RIGHTUP "; 	break;
				case SDL_HAT_RIGHTDOWN: 	std::cout << "RIGHTDOWN ";	break;
				case SDL_HAT_LEFTUP: 		std::cout << "LEFTUP "; 	break;
				case SDL_HAT_LEFTDOWN: 		std::cout << "LEFTDOWN "; 	break;
			}
		}
		
		// Axis events can be fetched with the SDL_JoystickGetAxis function
		std::cout << "JOYSTICKGETAXIS" << std::endl 
			<< "X MOVE " << SDL_JoystickGetAxis(apJoysticks[i], 0) << " Y MOVE " << 
			SDL_JoystickGetAxis(apJoysticks[i], 1) << std::endl;
	}
}
