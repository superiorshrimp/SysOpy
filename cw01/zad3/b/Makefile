static:
	gcc -c ../../zad1/memory_blocks.c -Wall
	ar rcs libmemory_blocks.a memory_blocks.o
	gcc -o main_static ../../zad2/main.c -L. -lmemory_blocks -Wall -O
	rm -f *.o
	make tests_static
	rm -f *.a main_static

	gcc -c ../../zad1/memory_blocks.c -Wall
	ar rcs libmemory_blocks.a memory_blocks.o
	gcc -o main_static ../../zad2/main.c -L. -lmemory_blocks -Wall -O1
	rm -f *.o
	make tests_static
	rm -f *.a main_static

	gcc -c ../../zad1/memory_blocks.c -Wall
	ar rcs libmemory_blocks.a memory_blocks.o
	gcc -o main_static ../../zad2/main.c -L. -lmemory_blocks -Wall -O2
	rm -f *.o
	make tests_static
	rm -f *.a main_static

	gcc -c ../../zad1/memory_blocks.c -Wall
	ar rcs libmemory_blocks.a memory_blocks.o
	gcc -o main_static ../../zad2/main.c -L. -lmemory_blocks -Wall -O3
	rm -f *.o
	make tests_static
	rm -f *.a main_static

	gcc -c ../../zad1/memory_blocks.c -Wall
	ar rcs libmemory_blocks.a memory_blocks.o
	gcc -o main_static ../../zad2/main.c -L. -lmemory_blocks -Wall -O0
	rm -f *.o
	make tests_static
	rm -f *.a main_static

	gcc -c ../../zad1/memory_blocks.c -Wall
	ar rcs libmemory_blocks.a memory_blocks.o
	gcc -o main_static ../../zad2/main.c -L. -lmemory_blocks -Wall -Os
	rm -f *.o
	make tests_static
	rm -f *.a main_static

shared:
	gcc -fPIC -c ../../zad1/memory_blocks.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_shared ../../zad2/main.c -lmemory_blocks -L. -Wl,-rpath=. -Wall -O
	rm -f *.o
	make tests_shared
	rm -f *.a main_shared
	
	gcc -fPIC -c ../../zad1/memory_blocks.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_shared ../../zad2/main.c -lmemory_blocks -L. -Wl,-rpath=. -Wall -O1
	rm -f *.o
	make tests_shared
	rm -f *.a main_shared

	gcc -fPIC -c ../../zad1/memory_blocks.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_shared ../../zad2/main.c -lmemory_blocks -L. -Wl,-rpath=. -Wall -O2
	rm -f *.o
	make tests_shared
	rm -f *.a main_shared

	gcc -fPIC -c ../../zad1/memory_blocks.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_shared ../../zad2/main.c -lmemory_blocks -L. -Wl,-rpath=. -Wall -O3
	rm -f *.o
	make tests_shared
	rm -f *.a main_shared

	gcc -fPIC -c ../../zad1/memory_blocks.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_shared ../../zad2/main.c -lmemory_blocks -L. -Wl,-rpath=. -Wall -O0
	rm -f *.o
	make tests_shared
	rm -f *.a main_shared

	gcc -fPIC -c ../../zad1/memory_blocks.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_shared ../../zad2/main.c -lmemory_blocks -L. -Wl,-rpath=. -Wall -Os
	rm -f *.o
	make tests_shared
	rm -f *.a main_shared
	
dynamic:
	gcc -c ../../zad1/memory_blocks.c -Wall
	gcc -c ../a/main_dynamic.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_dynamic main_dynamic.o memory_blocks.o -ldl -L. -Wall -O
	rm -f *.o
	make tests_dynamic
	rm -f *.so main_dynamic

	gcc -c ../../zad1/memory_blocks.c -Wall
	gcc -c ../a/main_dynamic.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_dynamic main_dynamic.o memory_blocks.o -ldl -L. -Wall -O1
	rm -f *.o
	make tests_dynamic
	rm -f *.so main_dynamic

	gcc -c ../../zad1/memory_blocks.c -Wall
	gcc -c ../a/main_dynamic.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_dynamic main_dynamic.o memory_blocks.o -ldl -L. -Wall -O2
	rm -f *.o
	make tests_dynamic
	rm -f *.a main_dynamic

	gcc -c ../../zad1/memory_blocks.c -Wall
	gcc -c ../a/main_dynamic.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_dynamic main_dynamic.o memory_blocks.o -ldl -L. -Wall -O3
	rm -f *.o
	make tests_dynamic
	rm -f *.so main_dynamic

	gcc -c ../../zad1/memory_blocks.c -Wall
	gcc -c ../a/main_dynamic.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_dynamic main_dynamic.o memory_blocks.o -ldl -L. -Wall -O0
	rm -f *.o
	make tests_dynamic
	rm -f *.so main_dynamic

	gcc -c ../../zad1/memory_blocks.c -Wall
	gcc -c ../a/main_dynamic.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall
	gcc -o main_dynamic main_dynamic.o memory_blocks.o -ldl -L. -Wall -Os
	rm -f *.o
	make tests_dynamic
	rm -f *.so main_dynamic

#running only a few tests for more clarity in results
tests_static:
	echo static tests:
	echo adding and deleting 10 big blocks
	echo adding and deleting 10 big blocks: >> ./results.txt
	./main_static 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 
	echo adding and deleting 50 big blocks
	echo adding and deleting 50 big blocks: >> ./results.txt
	./main_static 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0
	echo adding and deleting 100 big blocks
	echo adding and deleting 100 big blocks: >> ./results.txt
	./main_static 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0

tests_shared:
	echo shared tests: 
	echo adding 1 small block
	echo adding 1 small block: >> ./results.txt
	./main_shared 1 0 1 ../../zad2/data/sf1.txt
	echo adding and deleting 10 big blocks
	echo adding and deleting 10 big blocks: >> ./results.txt
	./main_shared 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 
	echo adding and deleting 50 big blocks
	echo adding and deleting 50 big blocks: >> ./results.txt
	./main_shared 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0
	echo adding and deleting 100 big blocks
	echo adding and deleting 100 big blocks: >> ./results.txt
	./main_shared 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0

tests_dynamic:
	echo dynamic tests:
	echo adding and deleting 10 big blocks
	echo adding and deleting 10 big blocks: >> ./results.txt
	./main_dynamic 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 
	echo adding and deleting 50 big blocks
	echo adding and deleting 50 big blocks: >> ./results.txt
	./main_dynamic 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0
	echo adding and deleting 100 big blocks
	echo adding and deleting 100 big blocks: >> ./results.txt
	./main_dynamic 1 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0 2 1 ../../zad2/data/bf1.txt 3 1 0 2 1 ../../zad2/data/bf2.txt 3 1 0 2 1 ../../zad2/data/bf3.txt 3 1 0 2 1 ../../zad2/data/bf4.txt 3 1 0 2 1 ../../zad2/data/bf5.txt 3 1 0

clean:
	rm -f *.a *.so *.txt main_dynamic main_static main_shared