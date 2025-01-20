#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "LzmaDecode.h"

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

int main() {
	uint8_t* flashBlob = readFlashBlob("../cg3100d_stock.bin");
	unsigned int lzmaBlobOffset = 0x1F78 + 0x5c;
	unsigned int lzmaBlobSize = 0x42AD;
	CLzmaDecoderState lzmaState;
	int ret = LzmaDecodeProperties(&lzmaState.Properties, flashBlob + lzmaBlobOffset, LZMA_PROPERTIES_SIZE);
	printf("LzmaDecodeProperties(0x%08X) returns %d\n", lzmaBlobOffset, ret);
	printf("Properties (lc, lp, pb): %d, %d, %d\n", lzmaState.Properties.lc, lzmaState.Properties.lp, lzmaState.Properties.pb);
	return 0;
}