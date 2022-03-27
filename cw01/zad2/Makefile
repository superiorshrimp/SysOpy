testing: ./main
	#clear results.txt file
	echo time measurments: > ./results.txt
	#counting
	echo counting parameters of 1 small file
	echo counting parameters of 1 small file: >> ./results.txt
	./main 1 0 1 ./data/sf1.txt
	echo counting parameters of 5 small files
	echo counting parameters of 5 small files: >> ./results.txt
	./main 5 0 5 ./data/sf1.txt ./data/sf2.txt ./data/sf3.txt ./data/sf4.txt ./data/sf5.txt
	echo counting parameters of 10 small files
	echo counting parameters of 10 small files: >> ./results.txt
	./main 10 0 10 ./data/sf1.txt ./data/sf2.txt ./data/sf3.txt ./data/sf4.txt ./data/sf5.txt ./data/sf6.txt ./data/sf7.txt ./data/sf8.txt ./data/sf9.txt ./data/sf10.txt
	echo counting parameters of 1 medium file
	echo counting parameters of 1 medium file: >> ./results.txt
	./main 1 0 1 ./data/mf1.txt
	echo counting parameters of 5 medium files
	echo counting parameters of 5 medium files: >> ./results.txt
	./main 5 0 5 ./data/mf1.txt ./data/mf2.txt ./data/mf3.txt ./data/mf4.txt ./data/mf5.txt
	echo counting parameters of 10 medium files
	echo counting parameters of 10 medium files: >> ./results.txt
	./main 10 0 10 ./data/mf1.txt ./data/mf2.txt ./data/mf3.txt ./data/mf4.txt ./data/mf5.txt ./data/mf6.txt ./data/mf7.txt ./data/mf8.txt ./data/mf9.txt ./data/mf10.txt
	echo counting parameters of 1 big file
	echo counting parameters of 1 big file: >> ./results.txt
	./main 1 0 1 ./data/bf1.txt
	echo counting parameters of 5 big files
	echo counting parameters of 5 big files: >> ./results.txt
	./main 5 0 5 ./data/bf1.txt ./data/bf2.txt ./data/bf3.txt ./data/bf4.txt ./data/bf5.txt
	echo counting parameters of 10 big files
	echo counting parameters of 10 big files: >> ./results.txt
	./main 10 0 10 ./data/bf1.txt ./data/bf2.txt ./data/bf3.txt ./data/bf4.txt ./data/bf5.txt ./data/bf6.txt ./data/bf7.txt ./data/bf8.txt ./data/bf9.txt ./data/bf10.txt
	#reading and copying
	echo copying 1 small file
	echo copying 1 small file: >> ./results.txt
	./main 1 1 1 ./data/sf1.txt
	echo copying 5 small files
	echo copying 5 small files: >> ./results.txt
	./main 5 1 5 ./data/sf1.txt ./data/sf2.txt ./data/sf3.txt ./data/sf4.txt ./data/sf5.txt
	echo copying 10 small files
	echo copying 10 small files: >> ./results.txt
	./main 10 1 10 ./data/sf1.txt ./data/sf2.txt ./data/sf3.txt ./data/sf4.txt ./data/sf5.txt ./data/sf6.txt ./data/sf7.txt ./data/sf8.txt ./data/sf9.txt ./data/sf10.txt
	echo copying 1 medium file
	echo copying 1 medium file: >> ./results.txt
	./main 1 1 1 ./data/mf1.txt
	echo copying 5 medium files
	echo copying 5 medium files: >> ./results.txt
	./main 5 1 5 ./data/mf1.txt ./data/mf2.txt ./data/mf3.txt ./data/mf4.txt ./data/mf5.txt
	echo copying 10 medium files
	echo copying 10 medium files: >> ./results.txt
	./main 10 1 10 ./data/mf1.txt ./data/mf2.txt ./data/mf3.txt ./data/mf4.txt ./data/mf5.txt ./data/mf6.txt ./data/mf7.txt ./data/mf8.txt ./data/mf9.txt ./data/mf10.txt
	echo copying 1 big file
	echo copying 1 big file: >> ./results.txt
	./main 1 1 1 ./data/bf1.txt
	echo copying 5 big files
	echo copying 5 big files: >> ./results.txt
	./main 5 1 5 ./data/bf1.txt ./data/bf2.txt ./data/bf3.txt ./data/bf4.txt ./data/bf5.txt
	echo copying 10 big files
	echo copying 10 big files: >> ./results.txt
	./main 10 1 10 ./data/bf1.txt ./data/bf2.txt ./data/bf3.txt ./data/bf4.txt ./data/bf5.txt ./data/bf6.txt ./data/bf7.txt ./data/bf8.txt ./data/bf9.txt ./data/bf10.txt
	#deleting allocated blocks
	echo deleting 1 small block
	echo deleting 1 small block: >> ./results.txt
	./main 1 2 1 ./data/sf1.txt 3 1 0
	echo deleting 5 small blocks
	echo deleting 5 small blocks: >> ./results.txt
	./main 5 2 5 ./data/sf1.txt ./data/sf2.txt ./data/sf3.txt ./data/sf4.txt ./data/sf5.txt 3 5 0 1 2 3 4
	echo deleting 10 small blocks
	echo deleting 10 small blocks: >> ./results.txt
	./main 10 2 10 ./data/sf1.txt ./data/sf2.txt ./data/sf3.txt ./data/sf4.txt ./data/sf5.txt ./data/sf6.txt ./data/sf7.txt ./data/sf8.txt ./data/sf9.txt ./data/sf10.txt 3 10 0 1 2 3 4 5 6 7 8 9
	echo deleting 1 medium block
	echo deleting 1 medium block: >> ./results.txt
	./main 1 2 1 ./data/mf1.txt 3 1 0
	echo deleting 5 medium blocks
	echo deleting 5 medium blocks: >> ./results.txt
	./main 5 2 5 ./data/mf1.txt ./data/mf2.txt ./data/mf3.txt ./data/mf4.txt ./data/mf5.txt 3 5 0 1 2 3 4
	echo deleting 10 medium blocks
	echo deleting 10 medium blocks: >> ./results.txt
	./main 10 2 10 ./data/mf1.txt ./data/mf2.txt ./data/mf3.txt ./data/mf4.txt ./data/mf5.txt ./data/mf6.txt ./data/mf7.txt ./data/mf8.txt ./data/mf9.txt ./data/mf10.txt 3 10 0 1 2 3 4 5 6 7 8 9
	echo deleting 1 big block
	echo deleting 1 big block: >> ./results.txt
	./main 1 2 1 ./data/bf1.txt 3 1 0
	echo deleting 5 big blocks
	echo deleting 5 big blocks: >> ./results.txt
	./main 5 2 5 ./data/bf1.txt ./data/bf2.txt ./data/bf3.txt ./data/bf4.txt ./data/bf5.txt 3 5 0 1 2 3 4
	echo deleting 10 big blocks
	echo deleting 10 big blocks: >> ./results.txt
	./main 10 2 10 ./data/bf1.txt ./data/bf2.txt ./data/bf3.txt ./data/bf4.txt ./data/bf5.txt ./data/bf6.txt ./data/bf7.txt ./data/bf8.txt ./data/bf9.txt ./data/bf10.txt 3 10 0 1 2 3 4 5 6 7 8 9
	#adding and deleting blocks
	echo adding and deleting 10 big blocks
	echo adding and deleting 10 big block: >> ./results.txt
	./main 1 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 
	echo adding and deleting 50 big blocks
	echo adding and deleting 50 big block: >> ./results.txt
	./main 1 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0
	echo adding and deleting 100 big blocks
	echo adding and deleting 100 big block: >> ./results.txt
	./main 1 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0 2 1 ./data/bf1.txt 3 1 0 2 1 ./data/bf2.txt 3 1 0 2 1 ./data/bf3.txt 3 1 0 2 1 ./data/bf4.txt 3 1 0 2 1 ./data/bf5.txt 3 1 0

main: ../zad1/memory_blocks.c main.c
	gcc -o main ../zad1/memory_blocks.c main.c -I. -Wall

clean:
	rm -f *.a *.so *.txt main