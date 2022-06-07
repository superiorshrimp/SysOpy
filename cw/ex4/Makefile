main:
	rm -f server_tcp client_tcp server_udp client_udp lserver_tcp lclient_tcp lserver_udp lclient_udp
	gcc server_tcp.c -o server_tcp -lpthread -Wall
	gcc client_tcp.c -o client_tcp -Wall
	gcc server_udp.c -o server_udp -lpthread -Wall
	gcc client_udp.c -o client_udp -Wall
	#./server_tcp
	#./server_udp

	gcc lserver_tcp.c -o lserver_tcp -lpthread -Wall
	gcc lclient_tcp.c -o lclient_tcp -Wall
	gcc lserver_udp.c -o lserver_udp -lpthread -Wall
	gcc lclient_udp.c -o lclient_udp -Wall
	#./lserver_tcp
	./lserver_udp

	rm -f server_tcp client_tcp server_udp client_udp lserver_tcp lclient_tcp lserver_udp lclient_udp

clean:
	rm -f server_tcp client_tcp server_udp client_udp lserver_tcp lclient_tcp lserver_udp lclient_udp