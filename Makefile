all: main

CC = gcc
override CFLAGS += -g -Wall -Werror -Wno-stringop-truncation -Wno-stringop-overflow -pthread -lm

main:
	$(CC) $(CFLAGS) main.c $(SRCS) -o "$@"

main-debug: $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -O0 main.c -o "$@"

clean:
	rm -f main main-debug