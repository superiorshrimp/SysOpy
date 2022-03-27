v1:
	rm -f dest*.txt pomiar_zad_1_sys.txt 
	gcc main_sys.c -o main_sys -Wall
	./main_sys ./src1.txt ./dest1.txt
	./main_sys ./src2.txt ./dest2.txt
	rm -f *.o

v2:
	rm -f dest*.txt pomiar_zad_1_cl.txt 
	gcc main_cl.c -o main_cl -Wall
	./main_cl ./src1.txt ./dest1.txt
	./main_cl ./src2.txt ./dest2.txt
	rm -f *.o

clean:
	rm -f dest*.txt main_cl main_sys pomiar_zad_1_sys.txt pomiar_zad_1_cl.txt