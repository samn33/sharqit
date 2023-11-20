CC = g++
CFLAG = -pthread -std=c++17

PROG = test_sharq
PROG_OBJ = test_fraction.o test_phase.o test_linear_map.o test_binary_matrix.o test_qgate.o test_qcirc.o \
	test_zxdiagram.o test_optimizer.o
PROG_SRC = test_fraction.cpp test_phase.cpp test_linear_map.cpp test_binary_matrix.cpp test_qgate.cpp test_qcirc.cpp \
	test_zxdiagram.cpp test_optimizer.cpp

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

$(PROG): $(PROG_OBJ)
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $(PROG_OBJ) -lsharq -lgtest_main -lgtest

$(PROG_OBJ): $(PROG_SRC)

clean:
	rm -f $(PROG_OBJ)

cleanall:
	rm -f $(PROG_OBJ) $(PROG)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC) $(CFLAG) $(IFLAG) -c $<
