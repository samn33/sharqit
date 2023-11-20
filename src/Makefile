VPATH = .:./zxdiagram

INC = sharq.h

LIB_SRC = fraction.cpp phase.cpp binary_matrix.cpp qgate.cpp qcirc.cpp zxnode.cpp \
	zxdiagram.cpp simplify.cpp extract_qcirc.cpp optimizer.cpp linear_map.cpp
LIB_OBJ = $(LIB_SRC:%.cpp=%.o)

CC = g++
CFLAG = -fPIC -Wall -O4 -std=c++17

PROG = main
PROG_SRC = main.cpp
PROG_OBJ = main.o
LIB = libsharq.so

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

INSTALL_INC = ~/include
INSTALL_LIB = ~/lib

$(PROG): $(PROG_OBJ) $(LIB)
	$(CC) -o $@ $(CFLAG) $(LFLAG) $(IFLAG) $(PROG_OBJ) -lsharq

$(LIB): $(LIB_OBJ) $(LIB_EIGEN_OBJ) $(INC)
	$(CC) $(CFLAG) $(IFLAG) -shared $(LIB_OBJ) $(LIB_EIGEN_OBJ) -o $@

static: $(LIB_OBJ) $(LIB_EIGEN_OBJ) $(INC)
	ar rcs libsharq.a $(LIB_OBJ) $(LIB_EIGEN_OBJ)

$(LIB_OBJ): $(LIB_SRC)

$(PROG_OBJ): $(PROG_SRC)

install:
	cp $(LIB) $(INSTALL_LIB); cp $(INC) $(INSTALL_INC)

tags:
	etags *.cpp *.h

clean:
	rm -f $(LIB_OBJ) $(PROG_OBJ) $(LIB)

cleanall:
	rm -f $(LIB_OBJ) $(PROG_OBJ) $(PROG) $(LIB)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC) $(CFLAG) $(IFLAG) -c $<
