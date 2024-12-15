## Overview

This one was the first in a while that wasn’t about a matrix. It felt pretty easy at first, 
but then it got really hard in the second part. I didn’t submit any wrong answers, and part 
one was done in about 1.5 hours. However, the second part was a real hell due to some spaghetti 
code, and it took me several hours.

## About my solution

For this one, as it wasn't related to matrices, I chose a simpler approach than the previous 
ones, without any structs—just an array and some loops. It worked great for the first part, 
but not so much for the second one.

I'm not sure I can explain it in detail anymore, but the principles of the solution are two 
loops that iterate over the input array (the one that looks like this `0..111....2222`). 
The first loop iterates backwards, and once it finds a `digit` (for the first part) or a `block` 
(for the second part), it starts the second loop, which iterates forwards until it finds some 
free space to place the digit (or the block).

Then, there is another loop that calculates the final value as the puzzle states—nothing hard 
enough to be worth explaining.

## About my mistakes

I didn’t make any special mistakes that led to a wrong answer in this one. But I have to say 
that my simple, 100% procedural approach didn’t work so well, as it made the code somewhat hard 
to read and understand, even though it’s really small. It’s basically spaghetti code. Maybe it 
isn’t that bad, but in the end, I would have preferred to start with structs for the `file blocks` 
and for the `free memory blocks`, along with more functions to work with them.

### Note

To run the first puzzle solution, use `day9 [input file] first_part`, and for the second, use 
`day9 [input file] second_part`.