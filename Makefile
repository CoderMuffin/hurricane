all: main

CC = gcc
override CFLAGS += -g -Ihurricane -O0 -Wno-cpp -L/home/runner/hurricane/hurricane/X11_so -lX11 -lxcb -lXau -lXdmcp -lbsd -Wall -Werror -Wno-stringop-truncation -Wno-stringop-overflow -pthread -lm

main:
	$(CC) $(CFLAGS) main.c -o "$@"

clean:
	rm -f main main-debug
