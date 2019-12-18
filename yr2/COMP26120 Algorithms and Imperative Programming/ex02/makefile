CC=gcc
CFLAGS=-g -std=c99 -Wall
#LDFLAGS=-lm

test2a:
	./part2a "only one parameter"
	./part2a "biggest parameter" "at" "start"
	./part2a "biggest" "parameter" "at" "end" "very very very big parameter"
	./part2a "answer" "somewhere" "in" "the" "middle"
	./part2a "two" "strings" "the" "same" "length" "ha ha !"
	./part2a #no  parameters

test2b:
	-./part2b #no  parameters
	-./part2b -x 34
	-./part2b -c -123aaa
	-./part2b -c -3000.0
	-./part2b -f -8000.0
	./part2b -c -27.0
	./part2b -f -16.6
	
