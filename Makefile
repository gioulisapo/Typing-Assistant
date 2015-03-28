#
# In order to execute this "Makefile" just type "make"
#	Apostolos Gioulis
#

OBJASSISTANT 	= main.o HelpFunct.o TrieStruct.o
SOURCE    	= main.c HelpFunct.c TrieStruct.c
HEADER          = HelpFunct.h TrieStruct.h
OUTASSISTANT  	= typing-assistant
CC	= gcc
FLAGS   = -g -c -std=c99
# -g option enables debugging mode 
# -c flag generates object code for separate files

typining-assistant: $(OBJASSISTANT)
	$(CC) -g -std=c99 $(OBJASSISTANT) -o $(OUTASSISTANT)

main.o: main.c
	$(CC) $(FLAGS) main.c

HelpFunct.o: HelpFunct.c
	$(CC) $(FLAGS) HelpFunct.c

TrieStruct.o: TrieStruct.c
	$(CC) $(FLAGS) TrieStruct.c

# clean house
clean:
	rm -f *.o
