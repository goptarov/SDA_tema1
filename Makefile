all: tema1
	touch tema1.out;
tema1: main.c
	gcc -o tema1 main.c
clean:
	rm -f tema1
