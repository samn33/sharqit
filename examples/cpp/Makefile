CC = g++
CFLAG = -Wall -O4 -std=c++17

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

check_random: check_random.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

sample: sample.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

sample1: sample1.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

sample2: sample2.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

sample3: sample3.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lshrqt

clean:
	rm -f sample sample1 sample2 sample3 sample4 sample5 check_random
