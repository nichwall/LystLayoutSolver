LystLayoutSolver [![Travis-CI](https://travis-ci.org/nichwall/LystLayoutSolver.svg?branch=master)](https://travis-ci.org/nichwall/LystLayoutSolver.svg?branch=master)
================

Program to solve the layout of the first Lyst puzzle.

This program will first generate every valid column, sorting them into left, right, and middle columns based on the side states of these columns. It will then combine columns together in order to reduce the number of checks it needs to do as it solves the puzzle.

*********************
Usage:
*********************
- ./Lyst [--THREADS=n] [--VERBOSITY=n] [--LOAD|-l]

*********************
TODO:
*********************
- Add additional configuration
- Clean up code
- Comment code
