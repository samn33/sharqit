CC = g++
CFLAG = -pthread -std=c++17

PROG = test_sharq
PROG_SRC = test_fraction.cpp test_phase.cpp test_linear_map.cpp test_binary_matrix.cpp test_qgate.cpp test_qcirc.cpp \
	test_zxdiagram.cpp test_optimizer_zx.cpp test_optimizer_pp.cpp
PROG_OBJ = $(PROG_SRC:%.cpp=%.o)

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
