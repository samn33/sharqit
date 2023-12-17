CC = g++
CFLAG = -fPIC -Wall -O4 -fopenmp -std=c++17

PROG = eval_sharq
PROG_OBJ = eval_sharq.o
PROG_SRC = eval_sharq.cpp

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

$(PROG): $(PROG_OBJ)
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $(PROG_OBJ) -lsharq

$(PROG_OBJ): $(PROG_SRC)

clean:
	rm -f $(PROG_OBJ)

cleanall:
	rm -f $(PROG_OBJ) $(PROG)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC) $(CFLAG) $(IFLAG) -c $<
