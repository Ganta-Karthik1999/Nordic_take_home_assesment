all:
	gcc -o server src/main.c src/socket_utils.c -I inc -ljson-c -lcurl -pthread

run:
	./server

clean:
	sudo rm -rf server