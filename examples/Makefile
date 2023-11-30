CC = g++
CFLAG = -Wall -O4 -std=c++17

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

sample1: sample1.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample2: sample2.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample3: sample3.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample4: sample4.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

clean:
	rm -f sample1 sample2 sample3 sample4 sample5
