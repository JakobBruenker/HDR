all:
	g++ -g -o bin/hdr main.cpp HDR.cpp -L/usr/X11R6/lib -lm -lpthread -lX11 -lHalf -pthread -I/usr/include/OpenEXR -I/usr/include/libdrm -lIlmImf 
