CC = gcc

all: clock

clock: config.o arduino.o clock.c
	$(CC) -o $@ $^

arduino.o: arduino.c
	$(CC) -c $<

config.o: config.c
	$(CC) -c $<
