CC = g++
CFLAGS = -g -Wall -o
TARGET = journey

$(TARGET): $(TARGET).cpp
			$(CC) $(CFLAGS) $(TARGET) $(TARGET).cpp
clean:
		$(RM) $(TARGET)