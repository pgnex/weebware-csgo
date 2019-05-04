# weebware
Game enhancing software by Kite & Nex.  

![legitbot](https://i.imgur.com/SQJdkom.jpg)
![visuals](https://i.imgur.com/KWl49fX.jpg)
![skins](https://i.imgur.com/i3qnaMm.jpg)
![misc](https://i.imgur.com/8KyHHv0.jpg)
![configs](https://i.imgur.com/oI9sgza.jpg)


## Compiling
The following libraries are required:  
   - Directx 9 SDK https://www.microsoft.com/en-au/download/details.aspx?id=6812  
   - LibCURL https://curl.haxx.se/download.html  
   - Polyhook https://github.com/stevemk14ebr/PolyHook_2_0  
   - Capstone https://github.com/aquynh/capstone    

## Injecting
   Any injection method may be used - preferably manual map. 
   Simply load the .dll built into csgo.exe

## Hooking
  The hooking can be found under hook_funcs.cpp and hook_funcs.h
  A new instance of a VEH hook class is required for each function (refer to polyhook documentation)
  
## Dependencies
   The guns.txt and skins.txt must be installed in C://weebware/dependencies/ failure to do so results in the skinchanger tab not working  
   These dependencies are installed in the weebware loader.
