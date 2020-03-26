CC = gcc
CFLAGS = -std=gnu99 -Wextra -Wall #-Wno-unused-parameter -Werror

TARGET = traceroute

SRCS = network.c check.c main.c
OBJS = network.o check.o main.o

.PHONY: all clean distclean

all = $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
		$(RM) $(OBJS) 

distclean:
		$(RM) $(OBJS) $(TARGET)