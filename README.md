# enc2wav
Hide other files inside wav files

## Installation
```
git clone https://github.com/bitt4/enc2wav
cd enc2wav
make
./enc2wav [OPTIONS...] FILE
```
## Usage
Use `./enc2wav FILE` to hide contents of FILE in another file, which is of wave sound format and can be played as sound.

### Options
| Option                       | Effect              | Default value    |
|------------------------------|---------------------|------------------|
|`-o`, `--output=FILE`         | Name of output file | source file name |
|`-c`, `--channels=NUM`        | Number of channels  | 1                |
|`-s`, `--sample-rate=NUM`     | Set sample rate     | 44100            |
|`-b`, `--bits-per-sample=NUM` | Set bits per sample | 16               |
|`-h`, `--help`                | Display help        |                  |

Bits per sample can be set to either 8, 16, 24 or 32.
