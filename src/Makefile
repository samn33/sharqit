VPATH = .:./zxdiagram

INC = sharq.h

LIB_SRC = fraction.cpp phase.cpp binary_matrix.cpp qgate.cpp qcirc.cpp zxnode.cpp \
	zxdiagram.cpp simplify.cpp extract_qcirc.cpp optimizer.cpp linear_map.cpp
LIB_OBJ = $(LIB_SRC:%.cpp=%.o)

CC = g++
CFLAG = -fPIC -Wall -O4 -std=c++17

PROG = sharq
PROG_SRC = sharq.cpp
PROG_OBJ = sharq.o
LIB_DYNAMIC = libsharq.so
LIB_STATIC = libsharq.a

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

INSTALL_INC = ~/include
INSTALL_LIB = ~/lib
INSTALL_BIN = ~/bin

#============================================================================
# Program
#============================================================================

ifdef OPTION

$(PROG): $(PROG_OBJ) $(LIB_STATIC)
	$(CC) -o $@ $(OPTION) $(CFLAG) $(LFLAG) $(IFLAG) $(PROG_OBJ) $(LIB_STATIC)

else

$(PROG): $(PROG_OBJ) $(LIB_DYNAMIC) $(LIB_STATIC)
	$(CC) -o $@ $(CFLAG) $(LFLAG) $(IFLAG) $(PROG_OBJ) $(LIB_STATIC)

endif

#============================================================================
# Library
#============================================================================

$(LIB_STATIC): $(LIB_OBJ) $(INC)
	ar rcs $(LIB_STATIC) $(LIB_OBJ)

$(LIB_DYNAMIC): $(LIB_OBJ) $(INC)
	$(CC) $(CFLAG) $(IFLAG) -shared $(LIB_OBJ) -o $@

$(LIB_OBJ): $(LIB_SRC)

$(PROG_OBJ): $(PROG_SRC)

#============================================================================

all:
	make

prof:
	make OPTION=-pg

debug:
	make OPTION=-g CFLAG="-fPIC -Wall -std=c++17"

install:
	cp $(LIB_DYNAMIC) $(LIB_STATIC) $(INSTALL_LIB); cp $(INC) $(INSTALL_INC); cp $(PROG) $(INSTALL_BIN)

tags:
	etags *.cpp *.h

clean:
	rm -f $(LIB_OBJ) $(PROG_OBJ) $(LIB) $(LIB_STATIC)

cleanall:
	rm -f $(LIB_OBJ) $(PROG_OBJ) $(PROG) $(LIB_DYNAMIC) $(LIB_STATIC)

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC) $(OPTION) $(CFLAG) $(IFLAG) -c $<
