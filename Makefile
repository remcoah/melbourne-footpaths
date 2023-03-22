# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# Define libraries to be linked (for example -lm)
LDLIBS1 = 
LDLIBS2 = 
LDLIBS3 = 
LDLIBS4 = 

# Define sets of source files and object files
SRC = driver.c quadtree.c array.c rectangle.c data.c

# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)
 
# Define the executable name(s)
EXE1 = dict1
EXE2 = dict2
EXE3 = dict3
EXE4 = dict4

$(EXE1): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE1) $(OBJ) $(LDLIBS1)

$(EXE2): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE2) $(OBJ) $(LDLIBS2)

$(EXE3): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE3) $(OBJ) $(LDLIBS3)

$(EXE4): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE4) $(OBJ) $(LDLIBS4)


driver.o: driver.c data.h quadtree.h

data.o: data.c data.h

rectangle.o: rectangle.c rectangle.h

quadtree.o: quadtree.c quadtree.h array.h

array.o: array.c array.h data.h

clean:
	rm -f $(OBJ) $(EXE1) $(EXE2) $(EXE3) $(EXE4)