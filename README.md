Purpose:
The purpose of this project is to perform a Sobel Edge Operator on a digital image to emphasize
the edges on the image.  The computer vision applications for this filter are explored.  Averaging
filters were also explored to reduce the amount of noise/disotorion in an image.

Source Code Files:
 /part1/part1_prog3.c  -- Source for part1
 /part1/part1_prog3.h  -- Header for part1

 /part2/part2_prog3.c  -- Source for part2
 /part2/part2_prog3.h  -- header for part2

 /images/*  -- Directory where images are kept

 How to Run:

 For Part1:
 1) Open up the part1_prog3.c file with vim, gedit, or any IDE
 2) Go to line 65 and edit the binary image threshold value to desired value

 For Part2:
 1)	Open up the part2_prog3.c file with vim, gedit, or any IDE
 2) Go to line 65 and edit the noise-free binary image threshold value to desired value
 3) Go to line 189 and edit noise factor to desired value
 4) Go to line 239 and edit noisy threshold to desired value

 Compling:
 In the project home directory, type make at the console.

 To run part1, navigate to part1 folder and type:
 ./part1 
 
To run part2, navigate to part2 folder and type:
 ./part2

The images are stored in the project root images folder

After executing part2, there should be a file named: accuracy.csv
that contains the accuracy calculations for reference

To remove the executables, run make clean at the console, in the home directory.
