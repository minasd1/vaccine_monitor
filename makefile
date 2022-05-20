OBJS = $(patsubst %.c, %.o, $(wildcard *.c)) 
HEADER = $(wildcard *.h)
TARGET = vaccineMonitor
CC = gcc
CFLAGS = -O3 -g -Wall 


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(TARGET): $(OBJS)
		$(CC) -g $(OBJS) -o $@ -lm


clean:
	-rm -f *.o 
	-rm -f $(TARGET)

