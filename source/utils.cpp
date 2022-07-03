#include <iostream>
#include <iomanip>
#include <ogc/color.h>
#include <ogc/gx_struct.h>
#include <wiiuse/wpad.h>
#include "../include/utils.hpp"
#include "../include/DRAW.hpp"

void print_wiimote_data(void* pXfb, const GXRModeObj* pRmode, WPADData* pWPADData)
{
	std::cout << "Data->Err: " << pWPADData->err << std::endl << std::endl;
	
	/*std::cout << "IR Dots: " << std::endl;
	for(s32 i = 0; i < 4; i++) 
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
	
	//ir.valid - TRUE is the wiimote is pointing at the screen, else it is false
	if(pWPADData->ir.valid) 
	{
		//float theta = pWPADData->ir.angle / 180.0 * M_PI;
	
		//ir.x/ir.y - The x/y coordinates that the wiimote is pointing to, relative to the screen.
		//ir.angle - how far (in degrees) the wiimote is twisted (based on ir)
		std::cout << "Cursor: " << std::fixed << std::setprecision(2) << pWPADData->ir.x << ", " << 
			pWPADData->ir.y << std::endl;
		std::cout << pWPADData->ir.angle << " deg" << std::endl << std::endl;
		
		DRAW_dot(pXfb, pRmode, pRmode->fbWidth, pRmode->xfbHeight, pWPADData->ir.x - 2, 
            pWPADData->ir.y - 2, pWPADData->ir.x + 2, pWPADData->ir.y + 2, COLOR_RED);
		/*DRAW_dot(xfb, rmode, rmode->fbWidth, rmode->xfbHeight, pWPADData->ir.x - 2 + 10 * sinf(theta), 
			pWPADData->ir.y - 2 - 10 * cosf(theta), pWPADData->ir.x + 2 + 10 * sinf(theta), 
			pWPADData->ir.y + 2 - 10 * cosf(theta), COLOR_BLUE);*/
	} 
	else std::cout << "No Cursor" << std::endl << std::endl;
	
	if(pWPADData->ir.raw_valid) 
	{
		//ir.z - How far away the wiimote is from the screen in meters
		std::cout << "Distance: " << pWPADData->ir.z << "m" << std::endl;
		//orient.yaw - The left/right angle of the wiimote to the screen 
		std::cout << "Yaw: " << pWPADData->orient.yaw << " deg" << std::endl << std::endl;

		/*for(s32 i = 0; i < 2; i++) 
		{
			std::cout << pWPADData->ir.sensorbar.rot_dots[i].x << ", " << 
				pWPADData->ir.sensorbar.rot_dots[i].y << std::endl;
			DRAW_dot(xfb, rmode, 4, 4, pWPADData->ir.sensorbar.rot_dots[i].x + 2, 
				pWPADData->ir.sensorbar.rot_dots[i].y + 2, pWPADData->ir.sensorbar.rot_dots[i].x + 2, 
				pWPADData->ir.sensorbar.rot_dots[i].y + 2, COLOR_GREEN);
		}*/
	} 
	else std::cout << std::endl << std::endl;

	std::cout << "Acceleration:" << std::endl;
	//accel.x/accel.y/accel.z - analog values for the accelleration of the wiimote
	//(Note: Gravity pulls downwards, so even if the wiimote is not moving, 
	//one(or more) axis will have a reading as if it is moving "upwards")
	std::cout << "XYZ: " << std::setw(3) << pWPADData->accel.x << ", " << pWPADData->accel.y << ", " << 
		pWPADData->accel.z << std::endl << std::endl;
	//orient.pitch - how far the wiimote is "tilted" in degrees
	std::cout << "Pitch: " << pWPADData->orient.pitch << " deg" << std::endl;
	//orient.roll - how far the wiimote is "twisted" in degrees (uses accelerometer)
	std::cout << "Roll: " << pWPADData->orient.roll << " deg" << std::endl;
}