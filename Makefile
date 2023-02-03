CC := gcc
NVCC := nvcc
TESTS := $(wildcard test/*.c)

CFILEFLAGS := -O2 -Wall -Wextra -Werror -Wpedantic -std=c2x -Isrc -lc -lm -lpthread -g -fPIC -Wl,--fatal-warnings
CLIBFLAGS := -O2 -Wall -Wextra -Werror -Wpedantic -std=c2x -Isrc -lc -lm -lpthread -shared -fPIC -g -Wl,--fatal-warnings
CTESTFLAGS := -O2 -Wall -Wextra -Werror -Wpedantic -std=c2x -Isrc -lc -lm -lpthread -Llib -lbst -lsort -lhashmap -g -Wl,--fatal-warnings

LIBDIR := lib

all: $(LIBDIR)/libbst.so $(LIBDIR)/libsort.so $(LIBDIR)/libhashmap.so $(LIBDIR)/libvector.so

%.o: %.c
	$(CC) $(CFILEFLAGS) -c $^ -o $@

$(LIBDIR)/libbst.so: src/bst/bst.c
	$(CC) $(CLIBFLAGS) $^ -o $@

$(LIBDIR)/libsort.so: src/sort/sort.o
	$(CC) $(CLIBFLAGS) $^ -o $@

$(LIBDIR)/libhashmap.so: src/hashmap/hashmap.c
	$(CC) $(CLIBFLAGS) $^ -o $@

$(LIBDIR)/libvector.so: src/vector/vector.c
	$(CC) $(CLIBFLAGS) $^ -o $@

clean:
	find . -type f -name "*.out" -delete
	find . -type f -name "vgcore.*" -delete
	find . -type f -name "*.log.core.*" -delete
	find . -type f -name "*.log" -delete
	find . -type f -name "*.so" -delete
	find . -type f -name "*.o" -delete

tests: $(LIBDIR)/libbst.so $(LIBDIR)/libsort.so $(LIBDIR)/libhashmap.so
	@for test in $(TESTS); do \
		$(CC) $(CTESTFLAGS) $$test -o $$test.out || exit $$?; \
	done

test: tests
	@echo
	@echo Tests: $(TESTS)
	@echo

	@for test in $(TESTS); do \
		LD_LIBRARY_PATH="lib" ./$$test.out || exit $$?; \
		echo; \
		echo; \
	done
	
	@for test in $(TESTS); do \
		LD_LIBRARY_PATH="lib" valgrind -s --log-file="$$test.log" --leak-check=full --show-leak-kinds=all --track-origins=yes ./$$test.out > /dev/null || exit $$?; \
		rm $$test.out; \
		echo "Valgrind log for $$test:"; \
		cat $$test.log; \
		echo; \
		echo; \
		rm $$test.log; \
	done

	@find . -type f -name "vgcore.*" -delete
