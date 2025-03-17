compile: build run

build:
	gcc \
	./*.c \
	-o play.out \
	`pkg-config --libs --cflags sdl3 sdl3-image`

run:
	./play.out
