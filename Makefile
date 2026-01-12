all:
	gcc -c -I ./include src/wordle.c -o obj/wordle.o
	gcc obj/wordle.o main.c -o main

clean:
	rm -rf obj/*o main
