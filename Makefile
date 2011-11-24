CC=g++
CFLAGS=-c -Wall -g
LDFLAGS=
SOURCES=main.cpp Pole.cpp SolutionStep.cpp Item.cpp Stack.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mi-par.sh

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@