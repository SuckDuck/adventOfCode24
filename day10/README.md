## Overview

This puzzle has been my favorite so far! I completed both parts in about 1.5 hours. 
It was quite similar to Puzzle 3 and other matrix-based problems, allowing me to reuse 
some of my existing structs and functions. The logic came together almost perfectly on 
the first try.

## About my solution

The solution starts by iterating over every spot in the matrix. When it encounters a 
spot with a height of `0`, it passes this spot to a recursive function. 
This function explores in four directions (up, down, left, and right) from the current spot. 
If it finds the next height in the sequence (i.e., `current + 1`), it recursively calls 
itself with the new spot, continuing the process.

Inside the recursive function, two shared variables track the score and rating. The score 
increases when a previously uncounted `9` is reached, while the rating increments for each 
distinct path leading to a `9`.

## About my mistakes

This time, I didn’t encounter any mistakes worth mentioning. It all worked smoothly! :D

### Note

To run the program, use: `day10 [input file]`
