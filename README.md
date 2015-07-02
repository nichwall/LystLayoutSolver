LystLayoutSolver
================

Program to solve the layout of the first Lyst puzzle.

This program will first generate every valid column, sorting them into left, right, and middle columns based on the side states of these columns. It will then combine columns together in order to reduce the number of checks it needs to do as it solves the puzzle. After creating all of these blocks of pieces, it will output these blocks to files, `*blocks.txt`, in order to increase startup time on subsequent runs.
Once these files have been created, change `restoreState` to be non-zero, and the program will then get the blocks from the files instead of generating them. This program will also continue from the last root index that was run, so killing the program will allow you to continue from just before it stopped without having to redo the entire puzzle.

*********************
TODO:
*********************
- Add additional configuration
- Clean up code
- Comment code
