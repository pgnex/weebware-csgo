# WEEBWARE
Game enhancing software by Kite & Nex.  

![legitbot](https://cdn.discordapp.com/attachments/457750635916427284/484954439363592204/a7a48497fcadf4ed02808f06b8eb72d7.png)
![visuals](https://cdn.discordapp.com/attachments/457750635916427284/484954438218416128/34bd30f7b83594c862fa5f93275928bf.png)
![misc](https://cdn.discordapp.com/attachments/457750635916427284/484954437333417994/9b36ed86652b594c4e903154a1126501.png)
![skins](https://cdn.discordapp.com/attachments/457750635916427284/484954440147664896/afd2f10fb4d229ca1e52e4ce905c59d6.png)
![configs](https://cdn.discordapp.com/attachments/457750635916427284/484954441431384084/fd4f902d5986741c2e675e5f797ada36.png)

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
  
