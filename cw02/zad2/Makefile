v1:
	rm -f dest*.txt pomiar_zad_2_sys.txt 
	gcc main_sys.c -o main_sys -Wall
	./main_sys c ./file1.txt
	./main_sys t ./file2.txt
	rm -f *.o

v2:
	rm -f pomiar_zad_2_cl.txt 
	gcc main_cl.c -o main_cl -Wall
	./main_cl c ./file1.txt
	./main_cl t ./file2.txt
	rm -f *.o

clean:
	rm -f main_cl main_sys pomiar_zad_2_cl.txt main_sys pomiar_zad_2_sys.txt 