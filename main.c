#include <stdio.h>
#include <stdint.h>

typedef struct {
    char riff_header[4];
    uint32_t chunk_size;
    char format[4];

    char subchunk1_id[4];
    uint32_t subchunk1_size;
    short audio_format;
    short channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    short block_align;
    short bits_per_sample;

    char subchunk2_id[4];
    uint32_t subchunk2_size;
    char *data;
} WavHeader;


int main(int argc, char *argv[]){

    FILE* output_file;
    FILE* target_file;

    target_file = fopen(argv[1], "rb");
    fseek(target_file, 0, SEEK_END);
    uint64_t target_file_size = ftell(target_file);
    fseek(target_file, 0, SEEK_SET);

    return 0;
}
