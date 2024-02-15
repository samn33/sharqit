CC = g++
CFLAG = -Wall -O4 -std=c++17

IFLAG = -I . -I ~/include -I /usr/include/eigen3
LFLAG = -L . -L ~/lib

check_random: check_random.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample1: sample1.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample2: sample2.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample3: sample3.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample4: sample4.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

sample5: sample5.cpp
	$(CC) -o $@ $(CFLAG) $(IFLAG) $(LFLAG) $< -lsharq

cleanall:
	rm -f sample1 sample2 sample3 sample4 sample5 check_random
