all:
	gcc -c -Wall -Werror -std=c99 -pedantic tests.c
	gcc -o tests tests.o
