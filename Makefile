.PHONY: all clean
all: compress decompress

compress.o: compress.c
	clang -c -Wall -Werror -gdwarf-4 compress.c

decompress.o: decompress.c
	clang -c -Wall -Werror -gdwarf-4 decompress.c

nibbler.o: nibbler.c
	clang -c -Wall -Werror -gdwarf-4 nibbler.c

compress: compress.o nibbler.o
	clang compress.o nibbler.o -o compress

decompress: decompress.o nibbler.o
	clang decompress.o nibbler.o -o decompress

clean:
	rm -f *.o compress decompress

