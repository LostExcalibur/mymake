main: main.o regle.o ens_regles.o
	cc -Wall -Wextra -o mymake main.o regle.o ens_regles.o

main.o: main.c regle.h
	cc -c main.c

regle.o: regle.c regle.h
	cc -c regle.c

ens_regles.o: ens_regles.c ens_regles.h
	cc -c ens_regles.c

clean:
	-rm *.o mymake

