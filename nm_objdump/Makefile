CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c99
DEBUGFLAGS = -g

TARGET_0 = hnm
TARGET_1 =

SOURCE_0 = test2.c test3.c nm-main.c main.h
SOURCE_1 =


all: $(TARGET_0) $(TARGET_1)


debug: CFLAGS += $(DEBUGFLAGS)
debug: all


$(TARGET_0): $(SOURCE_0)
	$(CC) $(CFLAGS) -o $(TARGET_0) $(SOURCE_0)

$(TARGET_1): $(SOURCE_1)
	$(CC) $(CFLAGS) -o $(TARGET_1) $(SOURCE_1)


clean:
	rm -f $(TARGET)
	rm -f *.o
	rm -f *.out
	rm -f *~


.PHONY: all clean debug
