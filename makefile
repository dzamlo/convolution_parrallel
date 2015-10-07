CC = gcc
CPP = cpp
CFLAGS = -Wall -Wextra -pedantic --std=gnu99 -g
LDFLAGS = -lpthread -lm -lrt
 
# CC = clang
# CFLAGS = -Wall -Wextra -pedantic --std=gnu99 -g -O0 -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined
# LDFLAGS = -fsanitize=address -fsanitize=leak -fsanitize=undefined -lpthread -lm

# CFLAGS = -Wall -Wextra -pedantic --std=gnu99 -g -O0 -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=thread
# LDFLAGS = -fsanitize=undefined -fsanitize=thread -lpthread -lm

OUTS = convolution
OBJS_convolution = convolution.o image.o main.o ppm.o 
OBJS = $(OBJS_convolution)

DEPS = $(OBJS:.o=.d) 

.PHONY: all clean format

all: $(OUTS) 

-include $(DEPS)

convolution: $(OBJS_convolution)
	$(CC) $(LDFLAGS) $^ -o $@

%.d: %.c
	$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

clean:
	rm -f $(OBJS) $(OUTS) $(DEPS)

format:
	clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4}" -i *.c *.h

