clean:
	rm bin/hdr
bin/hdr:
	g++ -g -o bin/hdr main.cpp HDR.cpp -L/usr/X11R6/lib -lm -lpthread -lX11 -lHalf -lIlmImf 
run: bin/hdr
	bin/hdr
