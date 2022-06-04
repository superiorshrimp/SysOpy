main:
	rm -f server client
	gcc server.c -o server -lpthread -Wall
	gcc client.c -o client -Wall
	./server
	rm -f server client

clean:
	rm -f server client