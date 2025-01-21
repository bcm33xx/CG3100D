A custom code to extract the second-stage bootloader from the SPI Flash dump of Netgear CG3100D.
1. According to disassembly, the first-stage bootloader allocates 0x80000 bytes for the second bootloader.
2. The [BcmProgramHeader](https://github.com/Broadcom/aeolus/blob/master/ProgramStore/ProgramStore.h) of the second bootloader image (lzma compressed) locates at offset 0x1F78.
3. The LZMA part starts at offset 0x1FD4.
4. The (LZMA loader in OpenWrt](https://github.com/openwrt/openwrt/tree/main/target/linux/bmips/image/lzma-loader/src) does not work for this blob.
5. The 7z decompressor in [aeolus](https://github.com/Broadcom/aeolus) works, only if the output size is set to no more than 0xB697 bytes.

# Usage (For reference only, should prefer aeolus)
```sh
make download
make all
./main.elf
```
