/**
	@file		PORTS.hpp
	@author		Juan de la Cruz Caravaca Guerrero
	@date		18/04/2022
    @brief		Header file describing the name of the ports for this project.
    @par		Description 
				
*/


#ifndef PORTS_H_
#define PORTS_H_

#include <cstdint>
#include <ogc/system.h>

/// Registers - Broadway access
#define HW_GPIOB_DIR		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000c4)
#define HW_GPIOB_OUT    	*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000c0)
#define HW_GPIOB_IN     	*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000c8)
#define HW_GPIOB_INTLVL		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000cc)
#define HW_GPIOB_INTFLAG	*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000d0)
#define HW_GPIOB_INTMASK	*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000d4)
#define HW_GPIOB_STRAPS		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000d8)

// Registers - Starlet only - AHB required
#define HW_GPIO_ENABLE		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000dc) // Defaults to 0xFFFFFF
#define HW_GPIO_OWNER    	*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000dc) // Defaults to 0x00C3A0
#define HW_GPIO_DIR			*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000e4) // Defaults to 0xFF5F3E
#define HW_GPIO_OUT    		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000e0)
#define HW_GPIO_IN     		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000e4)
#define HW_GPIO_INTLVL		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000ec)
#define HW_GPIO_INTFLAG		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000f0)
#define HW_GPIO_INTMASK		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000f4)
#define HW_GPIO_STRAPS		*(volatile uint32_t *)MEM_PHYSICAL_TO_K1(0x0d8000f4)

// GPIO Output pins - Broadway access
#define SLOT_LED    5
#define SENSOR_BAR  8
#define DO_EJECT    9
#define AVE_SCL     14
#define AVE_SDA     15

//GPIO Input pins - Broadway access
#define SLOT_IN     7

// GPIO Output pins - Starlet only - AHB required
#define SHUTDOWN	1
#define FAN			2
#define DC_DC		3
#define DI_SPIN		4
#define EEP_CS		10
#define EEP_CLK		11
#define EEP_MOSI	12
#define DEBUG0		16
#define DEBUG1		17
#define DEBUG2		18
#define DEBUG3		19
#define DEBUG4		20
#define DEBUG5		21
#define DEBUG6		22
#define DEBUG7		23

// GPIO Input pins - Starlet only - AHB required
#define POWER		0
#define EJECT_BTN	6
#define EEP_MISO	13

#endif /* PORTS_H_ */