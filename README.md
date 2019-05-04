# weebware
A project started late 2016 / early 2017 by [nex](https://github.com/nexzoz) and released during the summer of, 2017. This repository contains the cheat files, loader files, and all frontend and backend web files. This is a dying, yet ongoing project developed by [nex](https://github.com/nexzoz) & [kite](https://github.com/qtKite).

## Compiling
The following libraries are required:  
   - Directx 9 SDK https://www.microsoft.com/en-au/download/details.aspx?id=6812  
   - LibCURL https://curl.haxx.se/download.html  
   - Polyhook https://github.com/stevemk14ebr/PolyHook_2_0  
   - Capstone https://github.com/aquynh/capstone    

## Injecting
   Any injection method may be used - preferably manual map. 
   Load the x86 .dll built into csgo.exe using any injector of your choice.

## Hooking
  The hooking can be found under hook_funcs.cpp and hook_funcs.h
  A new instance of a VEH hook class is required for each function (refer to polyhook documentation)
  
## Dependencies
   The guns.txt and skins.txt must be installed in C://weebware/dependencies/ failure to do so results in the skinchanger tab not working
   All custom models need to be installed in order to work unless checks removed
   These dependencies are installed in the weebware loader.


![legitbot](https://i.imgur.com/SQJdkom.jpg)
![visuals](https://i.imgur.com/KWl49fX.jpg)
![skins](https://i.imgur.com/i3qnaMm.jpg)
![misc](https://i.imgur.com/8KyHHv0.jpg)
![configs](https://i.imgur.com/oI9sgza.jpg)
