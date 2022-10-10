main: main.c regle.o ens_regles.o
	cc -Wall -Wextra -o mymake main.c regle.o ens_regles.o

debug: main.c regle.o ens_regles.o
	cc -Wall -Wextra -g -o mymake main.c regle.o ens_regles.o

regle.o: regle.c regle.h
	cc -c regle.c

ens_regles.o: ens_regles.c ens_regles.h
	cc -c ens_regles.c

clean:
	-rm *.o mymake

