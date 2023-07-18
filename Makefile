mymake : main.c regle.o ens_regles.o lecture.o util.o 
	cc -Wall -Wextra -o mymake main.c regle.o ens_regles.o lecture.o util.o

debug: main.c regle_dbg.o ens_regles_dbg.o lecture_dbg.o util_dbg.o
	cc -Wall -Wextra -g -o dbg_mymake -Og main.c regle_dbg.o ens_regles_dbg.o lecture_dbg.o util_dbg.o

regle.o: regle.c regle.h
	cc -c regle.c

regle_dbg.o: regle.c regle.h
	cc -g -c regle.c -o regle_dbg.o

ens_regles.o: ens_regles.c ens_regles.h
	cc -c ens_regles.c

ens_regles_dbg.o: ens_regles.c ens_regles.h
	cc -g -c ens_regles.c -o ens_regles_dbg.o

lecture.o: lecture.c lecture.h
	cc -c lecture.c

lecture_dbg.o: lecture.c lecture.h
	cc -g -c lecture.c -o lecture_dbg.o

util.o: util.c util.h
	cc -c util.c

util_dbg.o: util.c util.h
	cc -g -c util.c -o util_dbg.o

clean:
	rm *.o

