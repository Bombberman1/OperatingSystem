linked_list: linked_list.o
	gcc linked_list.o -o linked_list

linked_list.o: linked_list.c
	gcc -c linked_list.c -o linked_list.o
	
my_malloc: my_malloc.o
	gcc my_malloc.o -o my_malloc

my_malloc.o: my_malloc.c
	gcc -c my_malloc.c -o my_malloc.o
	
clean:
	rm -f *.o *.a *.so linked_list my_malloc
