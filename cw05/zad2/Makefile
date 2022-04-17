main:
	rm -f main
	gcc main.c -o main -Wall
	echo sending message
	./main root test test_msg
	echo sort: data
	./main data
	echo sort: nadawca
	./main nadawca
	rm -f main

clean:
	rm -f main