build:
	gcc -Wall -std=c99 ./src/*.c -g -lSDL2 -lm -o renderer

run:
	./renderer

clean:
	rm renderer
