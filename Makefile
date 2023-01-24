CC := gcc
TESTS := $(wildcard test/*.c)
CFILES := $(wildcard src/*.c)

libalgo.so: $(CFILES)
	$(CC) $(CFLAGS) -O2 -Wall -Wextra -Werror -Wpedantic -std=c11 -shared -fPIC $(CFILES) -o libalgo.so -g

clean:
	find . -type f -name "*.out" -delete
	find . -type f -name "vgcore.*" -delete
	rm libalgo.so || true

tests: libalgo.so
	@echo $(TESTS)
	@for test in $(TESTS); do \
		$(CC) $(CFLAGS) -O2 -Iinclude -L. -lalgo -Wall -Wextra -Werror -Wpedantic -std=c11 $$test -o $$test.out; \
		LD_LIBRARY_PATH="." ./$$test.out; \
		rm $$test.out; \
	done
	@find . -type f -name "vgcore.*" -delete
