FLAGS=-O3 -Wall
EXE=ext2wav

main: main.c
	gcc main.c -o $(EXE) $(FLAGS)

clean:
ifeq ($(OS),Windows_NT)
	del $(EXE).exe
else
	rm -f $(EXE)
endif
