main:
	rm -f server client server_logs.txt
	gcc server.c -o server -Wall -lrt
	gcc client.c -o client -Wall -lrt
	./server

clean:
	rm -f server client server_logs.txt