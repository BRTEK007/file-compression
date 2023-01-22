all: compressor

compressor: main.c uni_vec.h
	gcc main.c -o compressor

clean:
	rm -f compressor
