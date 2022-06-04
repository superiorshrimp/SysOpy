main:
	rm -f server client un
	gcc server.c -o server -lpthread -Wall
	gcc client.c -o client -Wall
	./server 0 0
	rm -f server client un

clean:
	rm -f server client un