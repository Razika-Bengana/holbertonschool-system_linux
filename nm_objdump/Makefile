CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -g -std=c99
DEBUGFLAGS = -g

TARGET_0 = hnm
TARGET_1 = hobjdump

SOURCE_0 = hnm-64.c hnm-32.c hnm-main.c hnm.h
SOURCE_1 = hobjdump.c


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
