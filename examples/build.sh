#!/bin/bash
g++ -o sample1 -O4 -std=c++17 -L ~/lib -I ~/include -I /usr/include/eigen3 sample1.cpp -lsharq
g++ -o sample2 -O4 -std=c++17 -L ~/lib -I ~/include -I /usr/include/eigen3 sample2.cpp -lsharq
g++ -o sample3 -O4 -std=c++17 -L ~/lib -I ~/include -I /usr/include/eigen3 sample3.cpp -lsharq
