all:
	g++ -g -o out/hdr main.cpp -L/usr/X11R6/lib -lm -lpthread -lX11
