CC = g++
CFLAG = -fPIC -Wall -O4 -fopenmp -std=c++17

#PROG = eval_sharq
#PROG_OBJ = eval_sharq.o
#PROG_SRC = eval_sharq.cpp

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

all:
	make eval_sharq_zx; make eval_sharq_pp

eval_sharq_zx: eval_sharq_zx.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

eval_sharq_pp: eval_sharq_pp.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

cleanall:
	rm -f eval_sharq_zx eval_sharq_pp
