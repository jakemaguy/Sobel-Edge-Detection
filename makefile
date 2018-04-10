all: 
	gcc part1/part1_prog3.c -o part1/part1 -lm
	gcc part2/part2_prog3.c -o part2/part2 -lm

.PHONY:clean
clean:
	rm part1/part1 part2/part2



