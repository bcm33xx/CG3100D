#include "cpp_wrapper.h"
#include "7z/7z.h"

int decompress_lzma(const unsigned char* in_data, 
						unsigned in_size, 
						unsigned char* out_data, 
						unsigned out_size) {
	return decompress_lzma_7z(in_data, in_size, out_data, out_size) ? 1 : 0;
}
