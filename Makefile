all: compressor

compressor: main.c
	gcc main.c -o compressor

clean:
	rm -f compressor
