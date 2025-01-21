# Default rule

LZMA_FILES := LzmaDecode.h LzmaDecode.c LzmaTypes.h
LZMA_URL := https://github.com/openwrt/openwrt/raw/refs/heads/main/target/linux/bmips/image/lzma-loader/src

7Z_FILES := 7zlzma.cpp InByte.cpp BinTree3.h WindowOut.cpp \
        InByte.h LZMA.h IInOutStreams.h WindowIn.h BinTree3Main.h \
        BinTreeMain.h LiteralCoder.h BinTree4b.h BinTree2Main.h \
        OutByte.cpp AriBitCoder.cpp BitTreeCoder.h LenCoder.h \
        LZMADecoder.h OutByte.h BinTree3ZMain.h CRC.h \
        LZMAEncoder.cpp BinTree3Z.h LZMA.cpp AriPrice.h \
        LiteralCoder.cpp LZMADecoder.cpp WindowIn.cpp RCDefs.h \
        BinTreeMF.h README RangeCoder.h Const.h BinTreeMFMain.h \
        WindowOut.h BinTree4Main.h IInOutStreams.cpp AriConst.h \
        BinTree.h 7z.h AriBitCoder.h BinTree4.h BinTree4bMain.h \
        Portable.h BinTree2.h LenCoder.cpp LZMAEncoder.h
7Z_URL := https://github.com/Broadcom/aeolus/raw/refs/heads/master/ProgramStore/7z

C_OBJ := main.o LzmaDecode.o
CXX_OBJS = cpp_wrapper.o \
        7zlzma.o        \
        AriBitCoder.o   \
        IInOutStreams.o \
        InByte.o        \
        LenCoder.o      \
        LiteralCoder.o  \
        LZMA.o          \
        LZMADecoder.o   \
        LZMAEncoder.o   \
        OutByte.o       \
        WindowIn.o      \
        WindowOut.o
VPATH += 7z

CC		:= $(CROSS_COMPILE)gcc
LD		:= $(CROSS_COMPILE)ld

CFLAGS		= -O0 -ggdb -m32
CFLAGS		+= -D_LZMA_PROB32 -D_LZMA_NO_SYSTEM_SIZE_T -D_LZMA_PROB32 

CPPFLAGS = -O0 -ggdb -m32 -DGNU

.PHONY: clean

all: main.elf 

download: check_and_download_lzma check_and_download_7z

check_and_download_lzma: $(LZMA_FILES)

check_and_download_7z: $(7Z_FILES)

$(LZMA_FILES):
	@echo "$@ is missing. Downloading..."
	@curl -fsSL -o "$@" "$(LZMA_URL)/$@"

$(7Z_FILES):
	@echo "$@ is missing. Downloading..."
	mkdir -p 7z
	@curl -fsSL -o "7z/$@" "$(7Z_URL)/$@"

main.elf: $(C_OBJ) $(CXX_OBJS)
	$(CXX) $(CFLAGS) $(C_OBJ) $(CXX_OBJS) -o $@

clean:
	rm -f *.o
	rm -f main.elf


