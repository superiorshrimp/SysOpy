main:
	rm -f main ../res1.pgm ../res2.pgm ../res3.pgm
	gcc main.c -o main -lpthread -Wall
	./main 5 0 ../baboon.ascii.pgm ../res1.pgm
	./main 5 0 ../feep.ascii.pgm ../res1.pgm
	./main 5 1 ../baboon.ascii.pgm ../res1.pgm
	./main 10 1 ../res1.pgm ../res2.pgm
	./main 3 1 ../res2.pgm ../res3.pgm
	diff ../res1.pgm ../res3.pgm
	rm -f main

clean:
	rm -f main ../res1.pgm ../res2.pgm ../res3.pgm