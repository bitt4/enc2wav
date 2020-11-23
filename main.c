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
    uint32_t output_filename_arg_index = 0;
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
                output_filename_arg_index = optind;
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

    return 0;
}

void display_help(){
    /* Not implemented yet */
}
