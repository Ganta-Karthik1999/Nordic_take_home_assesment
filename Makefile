CC      := gcc
LIBS    := json-c -lcurl -pthread
TARGET  := server
SRCS    := src/main.c src/socket_utils.c src/timer.c src/json.c src/post.c src/counter.c
INCLUDE := inc

install:
	sudo apt install libjson-c-dev libcurl4-openssl-dev build-essential

all:
	$(CC) -o $(TARGET) $(SRCS) -I $(INCLUDE) -l $(LIBS)

run:
	./$(TARGET) 

clean:
	sudo rm -rf $(TARGET)
