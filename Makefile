CC      := gcc
LIBS    := json-c -lcurl -pthread
TARGET  := server
SRCS    := src/main.c src/socket_utils.c
INCLUDE := inc

all:
	$(CC) -o $(TARGET) $(SRCS) -I $(INCLUDE) -l $(LIBS)

run:
	./$(TARGET)

clean:
	sudo rm -rf $(TARGET)
