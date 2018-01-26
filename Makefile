all: main

main: main.c
	gcc main.c window.c -o siletto `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-3.0`

run:
	./siletto
