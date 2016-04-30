all:
	g++ -g -o bin/hdr main.cpp HDR.cpp -L/usr/X11R6/lib -lm -lpthread -lX11
