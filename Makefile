CY-bibliotheque : fonction.o main.o
	gcc -o CY-bibliotheque fonction.o main.o
fonction.o : fonction.c
	gcc -o fonction.o -c fonction.c -W -Wall -ansi -pedantic
main.o : main.c
	gcc -o main.o -c main.c -W -Wall -ansi -pedantic
