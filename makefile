OBJS	= diseaseMonitor.o hashtable.o rbt.o bf.o list.o
SOURCE	= diseaseMonitor.c hashtable.c rbt.c bf.c list.c
HEADER	= hashtable.h rbt.h bf.h list.h
OUT	= diseaseMonitor

CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

diseaseMonitor.o: diseaseMonitor.c
	$(CC) $(FLAGS) diseaseMonitor.c 

hashtable.o: hashtable.c
	$(CC) $(FLAGS) hashtable.c 

rbt.o: rbt.c
	$(CC) $(FLAGS) rbt.c

bf.o: bf.c
	$(CC) $(FLAGS) bf.c

list.o: list.c
	$(CC) $(FLAGS) list.c

clean:
	rm -f $(OBJS) $(OUT) *.cfs
