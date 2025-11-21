CC=gcc
CFLAGS=-I. -Wall -Wextra -lm -lcglm -O3 -ffast-math

%: %.c 
	$(CC) -o $@ $< $(CFLAGS)
	./$@
	ffmpeg -i $@-%04d.ppm -r 60 $@.mp4
	
.PHONY: clean

clean:
	rm -f *.o
	rm -f *.ppm
	rm -f *.mp4
