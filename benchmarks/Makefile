CC = g++
CFLAG = -fPIC -Wall -O4 -fopenmp -std=c++17

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

all:
	make eval_sharqit_zx; make eval_sharqit_pp

eval_sharqit_zx: eval_sharqit_zx.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

eval_sharqit_pp: eval_sharqit_pp.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

clean:
	rm -f eval_sharqit_zx eval_sharqit_pp
