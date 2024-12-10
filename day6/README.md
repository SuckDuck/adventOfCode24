## Overview

This one was actually fun. I felt like I was making some kind of simple grid-based game, something like Tetris
or Snake. I spent around 6 hours on it—maybe that's too slow. My solution is probably overthought, and to be honest,
I submitted a wrong answer on two occasions: the first time because I didn’t read the entire rules, and the second
time because I made an error in the second part.

## About my solution
I parsed the input file into an array of structs that I called "square." These structs have some
parameters, such as their position on the matrix, a variable to mark them as obstacles, and a generic variable called
"value," which I used to mark them as already traveled. Then, I made another struct to represent the guard, which has 
variables for its position on the matrix, its orientation, and how many steps it has made. I also created a helper
function called `guard_step`, which tries to move the guard one step in its direction. If the next square is not an 
obstacle, the guard moves; if it is, the guard rotates to the right and tries to move again. The function then
returns 0 if the guard moves successfully or -1 if the guard leaves the grid.

So that’s the base. From there, both parts consist of a cycle that makes the guard travel its route one step at a
time. The first part waits until `guard_step` returns -1, then resets the grid and adds the values of every square's
"value." I think it would also work just by using the "total_displacement" value of the guard, but whatever. The
second part iterates over each square, temporarily marking it as an obstacle, then makes the guard follow its path
and counts how many times the guard "walks" over each square. If a square is walked over more than 4 times 
(once for each direction), it identifies it as an infinite loop and repeats the process until all squares are marked
as obstacles.


## About my mistakes
About my mistakes: The first one happened because I didn’t understand that the guard should "leave" the matrix. 
I thought I had to treat the matrix borders as obstacles too, so the guard would stop when it couldn’t find another
path after turning. The second mistake was in the section that identifies a path as a loop. I thought walking over
the same square 3 times was enough since the guard only turns in one direction. However, I was wrong. 
I changed it to 4 (once for each direction), and everything worked fine.


### Note
I don’t think anyone will ever read this (not even future me), but whatever. I made a visual representation of 
the matrix for debugging, to see the guard moving around it. The command should be run as: `first (input file) true`.

The second part actually takes a little time to resolve, probably there is something more fancy to be done to optimize
that.