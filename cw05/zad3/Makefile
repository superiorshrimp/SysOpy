main:
	rm -f main kons prod fifo result.txt
	gcc main.c -o main -Wall
	gcc konsument.c -o kons -Wall
	gcc producent.c -o prod -Wall
	./main
	rm -f main kons prod fifo

clean:
	rm -f main kons prod fifo result.txt