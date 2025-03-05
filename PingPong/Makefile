CC = g++
CFLAGS = -Wall -pthread
TARGET = ping_pong

all: $(TARGET)

$(TARGET): ping_pong.o
	$(CC) $(CFLAGS) -o $(TARGET) ping_pong.o

ping_pong.o: ping_pong.cpp
	$(CC) $(CFLAGS) -c ping_pong.cpp

clean:
	rm -f $(TARGET) ping_pong.o

