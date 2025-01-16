# Netgear CG3100D

The Netgear CG3100D is a DOCSIS3.0 model that Optus(Australia) deploys to its end users before the NBN rolls out. NBN uses DOCSIS3.1, which is not supported by this device.

CG3100D has a BCM3380 SOC. It features 4 LAN ports, 2.4G Wifi, 64MB of DDR2 RAM, and 8MB of SPI Flash. It does not have WAN port. More hardware specs can be found [here](https://openwrt.org/inbox/toh/openwrt/netgear_cg3100d_v3#hardware_highlights).

The stock firmware runs eCos.

There is a [W.I.P OpenWrt support](https://github.com/rikka0w0/openwrt-fast3864op/tree/bcm3380-20241014) for this device.

## `cg3100d_stock.bin`
The content of the on-board SPI flash.

The flash image contains two firmware images, locating at 0x20000 and 0x400000.

Extract the individual image:
`dd if=cg3100d_stock.bin of=img2.bin bs=$((0x400000)) skip=1`

The firmware image has a Broadcom proprietary header and the main body is LZMA compressed. To extract the raw eCos binary, we need [aeolus](https://github.com/Broadcom/aeolus):
`aeolus -x -f img2.bin -o img2.ecos`

This utility is open-sourced by Broadcom. The W.I.P OpenWrt support will add this utility to the OpenWrt host tool. If you are on the BCM3380 target, this tool will be built as you build the OpenWrt firmware. The location is `openwrt/staging_dir/host/bin/aeolus_ProgramStore`.
