main:
	rm -f main_v main_x
	gcc main_v.c -o main_v -Wall
	gcc main_x.c -o main_x -Wall -lpthread -lrt
	#./main_v
	./main_x
	rm -f main_v main_x

clean:
	rm -f main_v main_x