main:
	rm -f client_v server_v client_x server_x
	gcc server_v.c -o server_v -Wall
	gcc client_v.c -o client_v -Wall
	gcc server_x.c -o server_x -Wall -lrt
	gcc client_x.c -o client_x -Wall -lrt
	./server_v
	#./server_x
	rm -f client_v server_v client_x server_x

clean:
	rm -f client_v server_v client_x server_x