CC=g++
CFLAGS=-c -Wall -g -D DEBUG
LDFLAGS=
SOURCES=main.cpp Step.cpp Item.cpp Stack.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=mi-par.sh

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@