# Default rule

LZMA_FILES := LzmaDecode.h LzmaDecode.c LzmaTypes.h
URL := https://github.com/openwrt/openwrt/raw/refs/heads/main/target/linux/bmips/image/lzma-loader/src

CC		:= $(CROSS_COMPILE)gcc
LD		:= $(CROSS_COMPILE)ld

CFLAGS		= -O0 -ggdb
CFLAGS		+= -ggdb \
		-D_LZMA_PROB32 -D_LZMA_NO_SYSTEM_SIZE_T -D_LZMA_PROB32 

LDFLAGS		=  -lc -dynamic-linker /lib64/ld-linux-x86-64.so.2 \
		/usr/lib/x86_64-linux-gnu/crt1.o \
		/usr/lib/x86_64-linux-gnu/crti.o \
		/usr/lib/x86_64-linux-gnu/crtn.o

.PHONY: clean

all: check_and_download_lzma main.elf 
	@echo "Final FLASH_BLOB is set to: $(FLASH_BLOB)"

check_and_download_lzma: $(LZMA_FILES)

$(LZMA_FILES):
	@echo "$@ is missing. Downloading..."
	@curl -fsSL -o "$@" "$(URL)/$@"

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.elf: main.o LzmaDecode.o
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f main.elf


