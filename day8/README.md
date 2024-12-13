## Overview

This was the first puzzle where I didn’t submit a wrong answer in any part. I found it somewhat
similar to the Day 6 puzzle; I even parsed the file the same way. I actually solved this one faster
than the previous puzzles: I spent around 15 minutes thinking about how to solve it, about 1 hour
parsing, and around 1 hour on the first part as well. The second part should have been faster, but
I spent about an hour on it too because of a bug. So, the total time was around 3 hours.

## About my solution

My solution is simple this time. I parsed the file into a matrix that I called `map`. 
A `map` is basically made up of an array of `spots`, which are structs that contain variables 
for their position in `x` and `y` within the map, their antenna type, and whether they have an 
antinode or not.  

Once that was done, I iterated through all the spots in the `map` until I found one with an 
antenna type. Then I saved its position as `s0` and iterated over all the spots again to find 
another spot with the same antenna type. If I found it, I saved its position as `s1` and calculated 
the distance between them by subtracting their `x` and `y` coordinates, saving the distances 
as `sx` and `sy`.

If it was the first part, I added the distance to the original coordinates of `s0` to find a spot 
to place an antinode. If it was the second part, I did the same thing but recursively. 
Then it continued with the two iterations until the end. 

Finally, I iterated over all the spots again, just adding to the result their value in the 
`antinode` variable so I could get the final answer.  

## About my mistakes

In the overview, I mentioned discovering a bug while solving the second part, but actually, 
that bug was present from the beginning —I just didn’t notice it.  

Working with matrices in C is not as straightforward as in other languages. 
Instead of using a `[][]` syntax, which requires putting arrays inside another array and allocating 
everything on the heap, I chose a simpler approach. 
I allocated all the `spots` contiguously in memory and created a function that takes `x` and `y` 
coordinates to return a pointer to the `spot` at those coordinates.  

The function could work in two ways:
1. Iterating over all the `spots` to find the one with the matching coordinates (inefficient and impractical).  
2. Calculating the index directly using the equation `y*c+x`, where `c` is the number of columns in the matrix.  

Here’s where the bug occurred: I mistakenly used the number of rows (`r`) instead of columns (`c`) in the formula, so the index was calculated as `y*r+x` And I didn’t notice the issue at first because both the test input and the actual input had the same number of rows and columns. But once fixed that, everything was ok.   

### Note

To run the first puzzle solution, use `day8 [input file] first_part`, and for the second, use 
`day8 [input file] second_part`.