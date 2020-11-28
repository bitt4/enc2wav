#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

void display_help();

typedef struct {
    char wav_riff_header[4];
    uint32_t wav_chunk_size;
    char wav_format[4];

    char wav_subchunk1_id[4];
    uint32_t wav_subchunk1_size;
    short wav_audio_format;
    short wav_channels;
    uint32_t wav_sample_rate;
    uint32_t wav_byte_rate;
    short wav_block_align;
    short wav_bits_per_sample;

    char wav_subchunk2_id[4];
    uint32_t wav_subchunk2_size;
} WavHeader;


int main(int argc, char *argv[]){

    FILE* output_file;
    FILE* target_file;

    char* target_filename = NULL;

    /* default options */
    char* output_filename = NULL;
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
                break;
            case 'c':
                channels = (short)strtol(optarg, &end, 10);
                break;
            case 's':
                sample_rate = (uint32_t)strtol(optarg, &end, 10);
                break;
            case 'b':
                bits_per_sample = (short)strtol(optarg, &end, 10);
                break;
            case '?':
                fprintf(stderr,
                        "%s: Use -h or --help to display options.\n", argv[0]);
                exit(EXIT_FAILURE);
            default: {}
            }
    }

    /* catch parsing errors */
    if(channels <= 0){
        fprintf(stderr, "Error: Invalid number of channels\n.");
        return -1;
    }
    if(bits_per_sample <= 0 || bits_per_sample > 32 || bits_per_sample % 8 != 0){
        fprintf(stderr, "Error: Invalid number of bits per sample\n.");
        return -1;
    }

    /* parse non-option arguments */
    for (int i = optind; i < argc; i++){
        target_filename = argv[i];
        break;    /* ignore remaining args */
    }

    if(target_filename == NULL){
        fprintf(stderr, "%s: Target file not specified.\n"
                        "%s: Use -h or --help to display options.\n", argv[0], argv[0]);
        return -1;
    }

    /* If name of the output file was not specified, use name of the target file */
    if(output_filename == NULL){
        output_filename = (char*)calloc(strlen(target_filename) + 5, sizeof(char));    /* 4 chars for dot and wav extension  */
        strcat(output_filename, target_filename);                                      /* (".wav") and one for the NULL byte */
        strcat(output_filename, ".wav");
    }

    target_file = fopen(target_filename, "rb");

    if(target_file == NULL){
        fprintf(stderr, "Couldn't open file `%s`\n", target_filename);
        return -1;
    }

    fseek(target_file, 0, SEEK_END);
    uint64_t target_file_size = ftell(target_file);
    fseek(target_file, 0, SEEK_SET);

    const uint32_t wav_header_length = 44;

    uint32_t byte_rate = sample_rate * channels * bits_per_sample / 8;
    short block_align = channels * bits_per_sample / 8;
    uint32_t number_of_samples = target_file_size / block_align + 1;    /* we need 1 more sample, because arbitrary file */
                                                                        /* might not be aligned perfectly */
    uint32_t data_length = number_of_samples * block_align;
    uint32_t padding_length = data_length - target_file_size;

    WavHeader wav_header = {
                            .wav_riff_header = "RIFF",
                            .wav_chunk_size = 36 + data_length,
                            .wav_format = "WAVE",
                            .wav_subchunk1_id = "fmt ",
                            .wav_subchunk1_size = 16,
                            .wav_audio_format = 1,
                            .wav_channels = channels,
                            .wav_sample_rate = sample_rate,
                            .wav_byte_rate = byte_rate,
                            .wav_block_align = block_align,
                            .wav_bits_per_sample = bits_per_sample,
                            .wav_subchunk2_id = "data",
                            .wav_subchunk2_size = data_length
    };

    uint64_t read_buffer_length = wav_header_length + data_length;
    char* target_file_read_buffer = (char*)malloc(read_buffer_length);
    fread(target_file_read_buffer, 1, read_buffer_length, target_file);
    fclose(target_file);

    output_file = fopen(output_filename, "wb");

    if(output_file == NULL){
        fprintf(stderr, "Couldn't create/open file `%s`.\n", output_filename);
        return -1;
    }

    fwrite(&wav_header, sizeof(WavHeader), 1, output_file);
    fwrite(target_file_read_buffer, sizeof(char), read_buffer_length, output_file);
    char* padding = (char*)calloc(padding_length, sizeof(char));
    fwrite(padding, sizeof(char), padding_length, output_file);

    fclose(output_file);

    free(output_filename);
    free(target_file_read_buffer);
    free(padding);

    return 0;
}

void display_help(){
    /* Not implemented yet */
}
