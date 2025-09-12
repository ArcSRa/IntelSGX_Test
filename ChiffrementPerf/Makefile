CFLAGS  = -Wall -Wextra
LDFLAGS  = -lcrypto

ifeq ($(DEBUG),1)
GRAMINE_LOG_LEVEL = debug
CFLAGS += -g
else
GRAMINE_LOG_LEVEL = error
CFLAGS += -O3
endif

.PHONY: all
all: chiff_bench_sans chiff_bench_sans.manifest
ifeq ($(SGX),1)
all: chiff_bench_sans.manifest.sgx chiff_bench_sans.sig
endif

chiff_bench_sans: chiff_bench_sans.o
	$(CC) $(CFLAGS) chiff_bench_sans.o -o chiff_bench_sans $(LDFLAGS)

chiff_bench_sans.o: chiff_bench.c
	$(CC) $(CFLAGS) -c chiff_bench.c -o chiff_bench_sans.o

chiff_bench_sans.manifest: chiff_bench.manifest.template chiff_bench_sans
	gramine-manifest \
		-Dlog_level=$(GRAMINE_LOG_LEVEL) \
		$< $@
	gramine-manifest-check $@

chiff_bench_sans.sig chiff_bench_sans.manifest.sgx: chiff_bench_sans.manifest
	gramine-sgx-sign \
		--manifest $< \
		--output $<.sgx
	gramine-manifest-check $<.sgx


ifeq ($(SGX),)
GRAMINE = gramine-direct
else
GRAMINE = gramine-sgx
endif

.PHONY: check
check: all
	$(GRAMINE) chiff_bench_sans > OUTPUT
	echo "test chiff sgx" | diff OUTPUT -
	@echo "[ Success ]"

.PHONY: clean
clean:
	$(RM) *.sig *.manifest.sgx *.manifest chiff_bench_sans.o chiff_bench_sans OUTPUT

.PHONY: distclean
distclean: clean
