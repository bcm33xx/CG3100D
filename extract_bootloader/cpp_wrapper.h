#ifndef __CPP_WRAPPER
#define __CPP_WRAPPER

#ifdef __cplusplus
extern "C" {
#endif

int decompress_lzma(const unsigned char* in_data, 
						unsigned in_size, 
						unsigned char* out_data, 
						unsigned out_size);

#ifdef __cplusplus
}
#endif

#endif