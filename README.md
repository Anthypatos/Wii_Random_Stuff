This test project shows tests of different features of the Nintendo Wii.
Currently it provides code for:
 - Video
 - Controller input, including motion controls, IR and rumble
 - Displaying pictures and working with the XFB
 - Playing non-3D audio
 - Using JSON to store and use settings
 - Some form of rudimentary user interface
 - Using GPIOs and interrupts
 - SDL-wii
 
This will (hopefully) be expanded in the future.

# Compiling this project
As of the date of writing this document, this project has been done using [devkitPPC r41](https://devkitpro.org/wiki/Getting_Started).
Additionally, you will need the following packages:
 - ppc-libjpeg-turbo
 - ppc-jansson
 - wii-sdl-libs
 
You can get those using [devkitPro pacman](https://devkitpro.org/wiki/devkitPro_pacman). Simply run:

    sudo (dkp-)pacman -S nameofthepackage1 nameofthepackage2 ...

Once that's done, just navigate to the project root and run 'make'.
