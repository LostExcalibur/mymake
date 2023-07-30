# Ceci est un commentaire pour tester
# Il faudrait vraiment supporter les variables
TEST = abc
mymake: main.c regle.o ens_regles.o lecture.o variable.o util.o
	cc -O2 -o mymake main.c regle.o ens_regles.o lecture.o variable.o util.o

debug: main.c regle_dbg.o ens_regles_dbg.o lecture_dbg.o variable_dbg.o util_dbg.o
	cc -Og -Wall -Wextra -D_DEBUG -g -o dbg_mymake main.c regle_dbg.o ens_regles_dbg.o lecture_dbg.o variable_dbg.o util_dbg.o

profile: main.c regle_prof.o ens_regles_prof.o lecture_prof.o variable_prof.o util_prof.o
	cc -O2 -DNDEBUG -fno-omit-frame-pointer -fno-inline-functions-called-once -fno-inline-functions -fno-optimize-sibling-calls -g -o profile main.c regle_prof.o ens_regles_prof.o lecture_prof.o variable_prof.o util_prof.o -static

regle.o: regle.c regle.h
	cc -O2 -c regle.c

regle_dbg.o: regle.c regle.h
	cc -Og -Wall -Wextra -D_DEBUG -g -c regle.c -o regle_dbg.o

regle_prof.o: regle.c regle.h
	cc -O2 -DNDEBUG -fno-omit-frame-pointer -fno-inline-functions-called-once -fno-inline-functions -fno-optimize-sibling-calls -g -c regle.c -o regle_prof.o

ens_regles.o: ens_regles.c ens_regles.h
	cc -O2 -c ens_regles.c

ens_regles_dbg.o: ens_regles.c ens_regles.h
	cc -Og -Wall -Wextra -D_DEBUG -g -c ens_regles.c -o ens_regles_dbg.o

ens_regles_prof.o: ens_regles.c ens_regles.h
	cc -O2 -DNDEBUG -fno-omit-frame-pointer -fno-inline-functions-called-once -fno-inline-functions -fno-optimize-sibling-calls -g -c ens_regles.c -o ens_regles_prof.o

lecture.o: lecture.c lecture.h
	cc -O2 -c lecture.c

lecture_dbg.o: lecture.c lecture.h
	cc -Og -Wall -Wextra -D_DEBUG -g -c lecture.c -o lecture_dbg.o

lecture_prof.o: lecture.c lecture.h
	cc -O2 -DNDEBUG -fno-omit-frame-pointer -fno-inline-functions-called-once -fno-inline-functions -fno-optimize-sibling-calls -g -c lecture.c -o lecture_prof.o

variable.o: variable.c variable.h
	cc -O2 -c variable.c

variable_dbg.o: variable.c variable.h
	cc -Og -Wall -Wextra -D_DEBUG -g -c variable.c -o variable_dbg.o

variable_prof.o: variable.c variable.h
	cc -O2 -DNDEBUG -fno-omit-frame-pointer -fno-inline-functions-called-once -fno-inline-functions -fno-optimize-sibling-calls -g -c variable.c -o variable_prof.o

util.o: util.c util.h
	cc -O2 -c util.c

util_dbg.o: util.c util.h
	cc -Og -Wall -Wextra -D_DEBUG -g -c util.c -o util_dbg.o

util_prof.o: util.c util.h
	cc -O2 -DNDEBUG -fno-omit-frame-pointer -fno-inline-functions-called-once -fno-inline-functions -fno-optimize-sibling-calls -g -c util.c -o util_prof.o

clean:
	rm *.o
 
install: mymake
	cp mymake ~/.local/bin 
