CC = gcc
LD = gcc
CFLAGS = -I. -std=gnu11
LDFLAGS =
DFLAGS = -g -DDEBUG
STATIC_LIB = libzcc.a
TEST_EXE = ztest

ifdef DEBUG
CFLAGS += $(DFLAGS)
endif

zvec_src = $(wildcard zvec/*.c)
test_src = $(wildcard test/*.c) $(wildcard test/zvec/*.c)
src = $(zvec_src) $(test_src)

zvec_obj = $(zvec_src:.c=.o)
test_obj = $(test_src:.c=.o)
obj = $(src:.c=.o)

all: $(STATIC_LIB) $(TEST_EXE)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(STATIC_LIB): $(zvec_obj)
	ar rcs $@ $^

$(TEST_EXE): $(test_obj) $(STATIC_LIB)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: debug
debug:
	make clean && make DEBUG=1

.PHONY: clean
clean:
	rm -f $(obj) $(STATIC_LIB) $(TEST_EXE)

.PHONY: clean_objs
clean_objs:
	rm -f $(obj)

