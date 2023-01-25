CC := gcc
TESTS := $(wildcard test/*.c)

CLIBFLAGS := -O2 -Wall -Wextra -Werror -Wpedantic -std=c11 -Isrc -lc -lm -shared -fPIC -g
CTESTFLAGS := -O2 -Wall -Wextra -Werror -Wpedantic -std=c11 -Isrc -lc -lm -Llib -lbst -lsort -g

LIBDIR := lib

all: $(LIBDIR)/libbst.so $(LIBDIR)/libsort.so tests

$(LIBDIR)/libbst.so: src/bst/bst.c
	$(CC) $(CLIBFLAGS) $^ -o $@

$(LIBDIR)/libsort.so: src/sort/sort.c
	$(CC) $(CLIBFLAGS) $^ -o $@

clean:
	find . -type f -name "*.out" -delete
	find . -type f -name "vgcore.*" -delete
	find . -type f -name "*.log.core.*" -delete
	find . -type f -name "*.log" -delete
	find . -type f -name "*.so" -delete

tests: $(LIBDIR)/libbst.so $(LIBDIR)/libsort.so
	@for test in $(TESTS); do $(CC) $(CTESTFLAGS) $$test -o $$test.out; done

test: $(LIBDIR)/libbst.so $(LIBDIR)/libsort.so
	@echo $(TESTS)
	@for test in $(TESTS); do \
		$(CC) $(CTESTFLAGS) $$test -o $$test.out; \
		LD_LIBRARY_PATH="lib" ./$$test.out; echo; \
	done
	@for test in $(TESTS); do \
		LD_LIBRARY_PATH="lib" valgrind -s --log-file="$$test.log" --leak-check=full --show-leak-kinds=all --track-origins=yes ./$$test.out > /dev/null; \
		rm $$test.out; \
		echo "Valgrind log for $$test:"; \
		cat $$test.log; \
		rm $$test.log; \
	done
	@find . -type f -name "vgcore.*" -delete
