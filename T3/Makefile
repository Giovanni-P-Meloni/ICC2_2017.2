all: JSON_Parser.o main.c
	gcc -Wall JSON_Parser.o main.c -o all
JSON_Parser.o: JSON_Parser.h JSON_Parser.c
	gcc -Wall -c JSON_Parser.c
run: all
	./all
clean:
	rm -f *.o all