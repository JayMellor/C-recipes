all: spell_checker.o linked_list.o
	cc linked_list.o spell_checker.o

spell_checker.o: spell_checker.c
	cc -c spell_checker.c

linked_list.o: ../linked_list/linked_list.c ../linked_list/linked_list.h
	cc -c ../linked_list/linked_list.h
