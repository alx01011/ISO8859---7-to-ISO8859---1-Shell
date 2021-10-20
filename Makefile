###################################################
#
# file: Makefile
#
# @Author:   Alexandros E. Antonakakiss
# @Version:  10-02-2021
# @email:    csd4802@csd.uoc.gr
#
# Makefile
#
####################################################

CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

.PHONY: translate

translate:
	$(CC) $(CFLAGS) src/*.c -o $@

clean:
	-rm -f translate


test:
	./translate < ./tests/test1.7 > test.1
	./translate < ./tests/test2.7 > test.2

