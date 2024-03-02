CC = g++
CFLAG = -fPIC -Wall -O4 -fopenmp -std=c++17

#PROG = eval_sharqit
#PROG_OBJ = eval_sharqit.o
#PROG_SRC = eval_sharqit.cpp

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

all:
	make eval_sharqit_zx; make eval_sharqit_pp

eval_sharqit_zx: eval_sharqit_zx.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

eval_sharqit_pp: eval_sharqit_pp.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

cleanall:
	rm -f eval_sharqit_zx eval_sharqit_pp
