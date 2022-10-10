main: main.o regle.o
	cc -Wall -Wextra -o mymake main.o regle.o

main.o: main.c regle.h
	cc -c main.c

regle.o: regle.c regle.h
	cc -c regle.c

clean:
	-rm *.o mymake

