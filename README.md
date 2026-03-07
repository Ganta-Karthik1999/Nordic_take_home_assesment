# TCP Odd/Even Counter Server

## Overview

This project implements a multi-threaded TCP server that receives JSON messages containing random values from a client (such as Fluent Bit).  
The server counts how many values are **odd** and **even** during a **60-second window**.

Every 60 seconds, the server sends an HTTP **POST request** to:

https://paste.c-net.org/

The request contains the number of odd and even values received during the last interval.

Example POST body:

```
odd=42 even=22
```

The response returned by the server is printed to standard output.

---

## Architecture

The server is designed using a **multi-threaded architecture**.

```
                +----------------------+
                |   TCP Client        |
                |   (Fluent Bit)      |
                +----------+----------+
                           |
                           v
                  +----------------+
                  |   TCP Server   |
                  |   (main thread)|
                  +--------+-------+
                           |
            +--------------+--------------+
            |                             |
            v                             v
   Client Worker Threads           Timer Thread
      handle_client()             timer_thread_func()
            |                             |
            v                             v
    Parse JSON messages           Trigger every 60 sec
    Update odd/even counts        Send HTTP POST request
                                  and Reset odd and even 
                                  to 0
```

---

## Features

- Multi-threaded TCP server
- Thread-safe counter updates using **pthread mutex**
- JSON parsing using **json-c**
- HTTP POST requests using **libcurl**
- Periodic reporting every **60 seconds**
- Graceful shutdown using **SIGINT (Ctrl+C)**

---

## JSON Input Format

The server expects JSON messages in the following format:

```json
{
  "rand_value": 183726423746721374(uint64_t) value
}
```

The value is parsed and classified as **odd** or **even**.

---

## Project Structure

```
project/
│
├── inc/
│   ├── common.h
│   ├── socket_utils.h
│   └── timer.h
│
├── src/
│   ├── main.c
│   ├── socket_utils.c
│   ├── timer.c
│   ├── json.c
│   ├── counter.c
│   └── post.c
│
├── Makefile
└── README.md
```

---

## Dependencies

Install required libraries.

### Ubuntu / Debian

```
sudo apt install libjson-c-dev libcurl4-openssl-dev
```

---

## Build Instructions

Compile using the Makefile:

```
make
```

Or compile manually:

```
gcc -o server \
src/main.c \
src/socket_utils.c \
src/timer.c \
src/json.c \
src/post.c \
src/counter.c \
-I inc \
-ljson-c -lcurl -pthread
```

---

## Running the Server

Start the server:

```
./server
```

The server listens on:

```
127.0.0.1:4242
```

---

## Example Client

Using Fluent Bit:

```
fluent-bit -i random -o tcp://127.0.0.1:4242 -p format=json_lines
```

Example output from the server:

```
Response: https://paste.c-net.org/WedlockEatin
Response: https://paste.c-net.org/ParadeRoutine
```

---

## Thread Safety

Multiple client threads update shared counters simultaneously.  
A **pthread mutex** protects access to shared variables.

Example:

```c
pthread_mutex_lock(&count_mutex);
even++;
pthread_mutex_unlock(&count_mutex);
```

---

## Timer Design

A dedicated timer thread runs independently of the networking logic.

```
while (running) {
    sleep(60);
    send_post_request(odd, even);
    reset counters
}
```

This ensures the server sends POST requests every **60 seconds regardless of client activity**.

---

## Graceful Shutdown

The server handles the `SIGINT` signal.

Press:

```
Ctrl + C
```

The server will:

- Stop accepting new connections
- Stop the timer thread
- Cleanup CURL resources
- Close the server socket

---

## Possible Improvements

Future improvements could include:

- Using `timerfd` for Linux-native timers
- Supporting persistent client connections
- Adding structured logging
- Making server configuration configurable
- Running the server as a system daemon

---

## Author

Karthik Ganta  
Embedded Systems / Embedded Linux Developer