CC = g++
CCFLAGS = -Wall

build: 
	$(CC) $(CCFLAGS) -o main ./src/main.cpp

clean:
	rm -f main