static:
	gcc -c memory_blocks.c -Wall
	ar rcs libmemory_blocks.a memory_blocks.o
	rm -f *.o

shared:
	gcc -fPIC -c memory_blocks.c -Wall
	gcc -shared -o libmemory_blocks.so memory_blocks.o -Wall -lmemory_blocks -L. -Wl,-rpath=.
	rm -f *.o

clean:
	rm -f *.a *.so *.txt