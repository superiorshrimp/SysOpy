main:
	gcc main.c -o main -Wall
	gcc main_exec.c -o main_exec -Wall
	./main ignore
	./main handler
	./main mask
	./main pending
	rm -f *.o main main_exec

clean:
	rm -f *.o main main_exec