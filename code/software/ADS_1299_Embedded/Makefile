all: biosignal

clean:
	rm -rf main.o biosignal

biosignal: main.o
		gcc -g -o biosignal main.o
main.o: main.c
		gcc -c -g main.c
