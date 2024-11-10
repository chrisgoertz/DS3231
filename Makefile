# Define the library name and version
LIB_NAME = DS3231
LIB_VERSION = 1.0

# Define the source files and object files
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

# Define the compiler and flags
CC = G:\libraries\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe
CFLAGS = -O2 -Wall -mmcu=atmega1284p
INCLUDES := -IG:\libraries\AVR\i2c_avr-master
INCLUDES += -IG:\libraries\avr8\avr8-gnu-toolchain\avr\include
INCLUDES += -IG:\libraries\avr8\avr8-gnu-toolchain\lib\gcc\avr\5.4.0\include

# Define the linker flags
# LDFLAGS := -L/usr/local/avr/lib -lavr-libc

# OS dependend RM Command
# For now i don't get, why the hell the rm command is not
# working in windows. Maybe it's related to the execution
# prevention or something similar.
# So i keep cleaning manual when working under M$ garbage OS...
ifeq ($(OS),Windows_NT)
RM = rm 
else
RM = rm -f
endif

# Rule to compile source files
$(OBJ_FILES): %.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

# Rule to create the library
lib$(LIB_NAME).a: $(OBJ_FILES)
	$(AR) rcs $@ $^

# Clean up
clean:

	$(RM) *.o