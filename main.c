#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>

void display_help();

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

    /* default options */
    char* output_filename = NULL;
    uint8_t filename_was_set = 0;
    short channels = 1;
    uint32_t sample_rate = 44100;
    short bits_per_sample = 16;

    static struct option long_options[] = {
                                           {"help", no_argument, NULL, 'h'},
                                           {"output", required_argument, NULL, 'o'},
                                           {"channels", required_argument, NULL, 'c'},
                                           {"sample-rate", required_argument, NULL, 's'},
                                           {"bits-per-sample", required_argument, NULL, 'b'},
                                           {NULL, 0, NULL, 0}
    };

    char *end;
    int c;

    while ((c = getopt_long(argc, argv, "ho:c:s:b:", long_options, NULL)) != -1) {
        switch (c)
            {
            case 'h':
                display_help();
                exit(EXIT_FAILURE);
            case 'o':
                output_filename = optarg;
                filename_was_set = 1;
                break;
            case 'c':
                channels = (int)strtol(optarg, &end, 10);
                break;
            case 's':
                sample_rate = (int)strtol(optarg, &end, 10);
                break;
            case 'b':
                bits_per_sample = (int)strtol(optarg, &end, 10);
                break;
            case '?':
                fprintf(stderr,
                        "%s: Use -h or --help to display options.\n", argv[0]);
                exit(EXIT_FAILURE);
            default: {}
            }
    }

    /* TODO: catch parsing errors */

    target_file = fopen(argv[1], "rb");
    fseek(target_file, 0, SEEK_END);
    uint64_t target_file_size = ftell(target_file);
    fseek(target_file, 0, SEEK_SET);

    const uint32_t wav_header_length = 44;

    uint32_t byte_rate = sample_rate * channels * bits_per_sample / 8;
    short block_align = channels * bits_per_sample / 8;
    uint32_t number_of_samples = target_file_size / block_align + 1;    /* we need 1 more sample, because arbitrary file */
                                                                        /* might not be aligned perfectly */
    uint32_t data_length = number_of_samples * block_align;
    uint32_t padding_length = target_file_size - data_length;

    WavHeader wav_header = {
                            .riff_header = "RIFF",
                            .chunk_size = 36 + data_length,
                            .format = "WAVE",
                            .subchunk1_id = "fmt ",
                            .subchunk1_size = 16,
                            .audio_format = 1,
                            .channels = channels,
                            .sample_rate = sample_rate,
                            .byte_rate = byte_rate,
                            .block_align = block_align,
                            .bits_per_sample = bits_per_sample,
                            .subchunk2_id = "data",
                            .subchunk2_size = data_length
    };

    return 0;
}

void display_help(){
    /* Not implemented yet */
}
