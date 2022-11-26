all: linked_list.o str_linked_list.o
	clang linked_list.o str_linked_list.o

linked_list.o: linked_list.h linked_list.c
	clang -c linked_list.c

str_linked_list.o: str_linked_list.c
	clang -c str_linked_list.c

clean:
	rm *.o
	rm a.out

