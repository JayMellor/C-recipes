all: word_list_linked.o spell_checker.o
	cc ../generic_linked_list/linked_list.o word_list_linked.o spell_checker.o

spell_checker.o: spell_checker.c
	cc -c spell_checker.c

word_list_array.o: word_list_array.c
	cc -c word_list_array.c

word_list_linked.o: word_list_linked.c
	cc -c word_list_linked.c
