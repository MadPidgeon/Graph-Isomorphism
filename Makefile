CC=g++-5
CFLAGS=-c -Wall -Wextra -std=c++1y -Wfatal-errors -DDEBUG -g

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

johnson.o: johnson.cc
	$(CC) $(CFLAGS) johnson.cc

configuration_test.o: configuration_test.cc
	$(CC) $(CFLAGS) configuration_test.cc

cameron_test.o: cameron_test.cc
	$(CC) $(CFLAGS) cameron_test.cc

cameron_test: cameron_test.o johnson.o lib
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o johnson.o cameron_test.o -o cameron_test -lpthread

conf2.o: conf2.cc
	$(CC) $(CFLAGS) conf2.cc

chap7.o: chap7.cc
	$(CC) $(CFLAGS) chap7.cc

chap7: chap7.o configuration.o ext.o conf2.o lib
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o chap7.o conf2.o -o chap7 

validate_nauty.o: validate_nauty.cc
	$(CC) $(CFLAGS) validate_nauty.cc

validate_nauty: validate_nauty.o ext.o configuration.o lib
	$(CC) validate_nauty.o configuration.o ext.o unionfind.o -o validate_nauty

connectivity.o: connectivity.cc
	$(CC) $(CFLAGS) connectivity.cc

connectivity: connectivity.o unionfind.o
	$(CC) connectivity.o unionfind.o -o connectivity

block_test.o: block_test.cc
	$(CC) $(CFLAGS) block_test.cc

align.o: align.cc
	$(CC) $(CFLAGS) align.cc

new_conf.o: new_conf.cc
	$(CC) $(CFLAGS) new_conf.cc

new_conf: new_conf.o lib conf2.o
	$(CC) permutation.o group.o action.o unionfind.o coset.o ext.o conf2.o new_conf.o -o new_conf

block_test: block_test.o lib johnson.o configuration.o align.o
	$(CC) permutation.o group.o action.o ext.o unionfind.o coset.o johnson.o configuration.o align.o block_test.o -o block_test

everything.o: everything.cc
	$(CC) $(CFLAGS) everything.cc

everything: everything.o group.o coset.o permutation.o ext.o conf2.o unionfind.o action.o fhl.o
	$(CC) permutation.o fhl.o group.o action.o ext.o unionfind.o coset.o  everything.o -o everything -lpthread

fhl.o: fhl.cc
	$(CC) $(CFLAGS) fhl.cc

fhl_test.o: fhl_test.cc
	$(CC) $(CFLAGS) fhl_test.cc

fhl_test: fhl_test.o group.o permutation.o coset.o fhl.o action.o ext.o unionfind.o
	$(CC) permutation.o group.o fhl_test.o coset.o fhl.o ext.o action.o unionfind.o -o fhl_test

iterator_example.o: iterator_example.cc
	$(CC) $(CFLAGS) iterator_example.cc

iterator_example: iterator_example.o ext.o
	$(CC) iterator_example.o ext.o -o iterator_example
