CC=g++
CFLAGS=-c -Wall -Wextra -std=c++11 -Wfatal-errors -g

.PHONY: clean all lib

all: main luks configuration

clean:
	rm *.o

main: lib main.o
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o main.o -o main

luks: lib luks.o
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o luks.o -o luks

configuration: lib configuration_test.o configuration.o
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o configuration.o configuration_test.o -o configuration

bruteforce: lib bruteforce_test.o bruteforce.o
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o bruteforce.o bruteforce_test.o -o bruteforce

lib: group.o permutation.o action.o unionfind.o coset.o ext.o

luks.o: luks.cc
	$(CC) $(CFLAGS) luks.cc

group.o: group.cc action.h
	$(CC) $(CFLAGS) group.cc

permutation.o: permutation.cc action.h
	$(CC) $(CFLAGS) permutation.cc

action.o: action.cc action.h
	$(CC) $(CFLAGS) action.cc

unionfind.o: unionfind.cc
	$(CC) $(CFLAGS) unionfind.cc

coset.o: coset.cc
	$(CC) $(CFLAGS) coset.cc

main.o: main.cc
	$(CC) $(CFLAGS) main.cc

ext.o: ext.cc
	$(CC) $(CFLAGS) ext.cc

bruteforce.o: bruteforce.cc
	$(CC) $(CFLAGS) bruteforce.cc

bruteforce_test.o: bruteforce_test.cc
	$(CC) $(CFLAGS) bruteforce_test.cc

configuration.o: configuration.cc
	$(CC) $(CFLAGS) configuration.cc

configuration_test.o: configuration_test.cc
	$(CC) $(CFLAGS) configuration_test.cc

cameron_test.o: cameron_test.cc
	$(CC) $(CFLAGS) cameron_test.cc

cameron_test: cameron_test.o lib
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o cameron_test.o -o cameron_test
	
