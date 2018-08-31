# WEEBWARE
Game enhancing software by Kite & Nex.  

## Compiling
The following libraries are required:  
   - Directx 9 SDK https://www.microsoft.com/en-au/download/details.aspx?id=6812  
   - LibCURL https://curl.haxx.se/download.html  
   - Polyhook https://github.com/stevemk14ebr/PolyHook_2_0  
   - Capstone https://github.com/aquynh/capstone    

## injecting
   Any injection method may be used - preferably manual map. 
   Simply load the .dll built into csgo.exe

## Hooking
  The hooking can be found under hook_funcs.cpp and hook_funcs.h
  A new instance of a VEH hook class is required for each function (refer to polyhook documentation)
  
