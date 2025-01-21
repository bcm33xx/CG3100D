# Netgear CG3100D

The Netgear CG3100D is a DOCSIS3.0 model that Optus(Australia) deploys to its end users before the NBN rolls out. NBN uses DOCSIS3.1, which is not supported by this device.

CG3100D has a BCM3380 SOC. It features 4 LAN ports, 2.4G Wifi, 64MB of DDR2 RAM, and 8MB of SPI Flash. It does not have WAN port. More hardware specs can be found [here](https://openwrt.org/inbox/toh/openwrt/netgear_cg3100d_v3#hardware_highlights).

The stock firmware runs eCos.

There is a [W.I.P OpenWrt support](https://github.com/rikka0w0/openwrt-fast3864op/tree/bcm3380-20241014) for this device.

## `cg3100d_stock.bin`
The content of the on-board SPI flash.

The flash image contains one second-stage bootloader images and two firmware images, locating at 0x1F78, 0x20000 and 0x400000, all LZMA-compressed.
The offsets of the firmware images are found in the serial console bootlog. The bootloader image offset is found though disassembly.

Extract the individual image:
`dd if=cg3100d_stock.bin of=img2.bin bs=$((0x400000)) skip=1`

The firmware image has a Broadcom proprietary header and the main body is LZMA compressed. To extract the raw eCos binary, we need [aeolus](https://github.com/Broadcom/aeolus):
`aeolus -x -f img2.bin -o img2.ecos`

This utility is open-sourced by Broadcom. The W.I.P OpenWrt support will add this utility to the OpenWrt host tool. If you are on the BCM3380 target, this tool will be built as you build the OpenWrt firmware. The location is `openwrt/staging_dir/host/bin/aeolus_ProgramStore`.

## The second-stage bootloader
When the SoC first powers on, the hardware maps the SPI Flash content to `0xBFC00000` and executes the instructions of the first-stage bootloader.
The first-stage bootloader calculates the system clock frequency, sets up SMP (to some extent), and initializes the DDR2 RAM.
Then, it sets up a temporary  stack and decompresses the second-stage bootloader into the memory (0x83F80000).
Finally, it transfers the control to the second-stage bootloader, and the boot process continues.

To extract the second-stage bootloader:
```sh
dd if=cg3100d_stock.bin of=bootloader_packed.bin bs=1 skip=$((0x1F78)) count=$((0x5C+0x42AD))
aeolus -x -f bootloader_packed.bin -o bootloader_padded.bin
```

The extracted `bootloader.bin` contains padding, as the decompressed size is not recorded anywhere (As far as I know). We see no more data after 0xA9D0. To remove the padding:
```sh
dd if=bootloader_padded.bin of=bootloader.bin bs=$((0xA9D0)) skip=0 count=1
```
