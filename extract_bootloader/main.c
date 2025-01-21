#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "LzmaDecode.h"
#include "cpp_wrapper.h"

static uint8_t* readFlashBlob(char* blobPath) {
	// File pointer
	FILE *file;

	// Open the binary file in read-binary mode
	file = fopen(blobPath, "rb");
	if (file == NULL) {
		perror("Error opening file");
		return NULL;
	}

	// Seek to the end of the file to determine its size
	if (fseek(file, 0, SEEK_END) != 0) {
		perror("Error seeking end of file");
		fclose(file);
		return NULL;
	}
	long file_size = ftell(file);
	if (file_size < 0) {
		perror("Error getting file size");
		fclose(file);
		return NULL;
	}

	// Reset file pointer to the beginning of the file
	rewind(file);

	// Allocate memory to hold the file contents
	unsigned char *buffer = (unsigned char *)malloc(file_size);
	if (buffer == NULL) {
		perror("Memory allocation failed");
		fclose(file);
		return NULL;
	}

	// Read the file into the buffer
	size_t bytes_read = fread(buffer, 1, file_size, file);
	fclose(file);
	if (bytes_read != file_size) {
		perror("Error reading file");
		free(buffer);
		return NULL;
	}

	// Successfully loaded the file into memory
	printf("File loaded into memory. Size: %ld bytes\n", file_size);
	return buffer;
}

int writeBinFile(const char *filename, const uint8_t *buffer, size_t length) {
    if (filename == NULL || buffer == NULL) {
        return -1; // Invalid arguments
    }

    // Open the file for writing (overwrites if the file exists)
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    // Write the buffer to the file
    size_t written = fwrite(buffer, 1, length, file);
    if (written != length) {
        perror("Failed to write data to file");
        fclose(file);
        return -1;
    }

    // Close the file
    if (fclose(file) != 0) {
        perror("Failed to close file");
        return -1;
    }

    return 0; // Success
}

uint32_t l2b(uint32_t little_endian_value) {
	return ((little_endian_value & 0x000000FF) << 24) | // Move byte 0 to byte 3
		   ((little_endian_value & 0x0000FF00) << 8)  | // Move byte 1 to byte 2
		   ((little_endian_value & 0x00FF0000) >> 8)  | // Move byte 2 to byte 1
		   ((little_endian_value & 0xFF000000) >> 24);  // Move byte 3 to byte 0
}

void dumpAsBE(uint32_t* mem, int count) {
	for (int i=0; i<count; i++) {
		printf("0x%04x: 0x%08X\n", i<<2, l2b(mem[i]));
	}
}

int main() {
	uint8_t* flashBlob = readFlashBlob("../cg3100d_stock.bin");
	unsigned int lzmaBlobOffset = 0x1F78 + 0x5c;
	unsigned int lzmaBlobSize = 0x42AD;
	CLzmaDecoderState lzmaState;
	int ret = LzmaDecodeProperties(&lzmaState.Properties, flashBlob + lzmaBlobOffset, LZMA_PROPERTIES_SIZE);
	printf("LzmaDecodeProperties(0x%08X) returns %d\n", lzmaBlobOffset, ret);
	printf("Properties (lc, lp, pb): %d, %d, %d\n", lzmaState.Properties.lc, lzmaState.Properties.lp, lzmaState.Properties.pb);

	printf("%d\n", (1536 << ((lzmaState.Properties.lc) + (lzmaState.Properties.lp))) + 3692);

	// A higher value does not work, the reason is unknown.
	uint32_t blSize = 0xB697;
	uint8_t* bl = malloc(blSize);
	if (decompress_lzma(flashBlob + lzmaBlobOffset, lzmaBlobSize - LZMA_PROPERTIES_SIZE, bl, blSize)) {
		printf("0x%08X\n", blSize);
		writeBinFile("bl.bin", bl, blSize);
		return 0;
	} else {
		return -1;
	}
}