## Overview

This one wasn't actually hard, but it was tricky for a couple of reasons, and I still submitted a 
bad answer twice. I spent around 3 hours on the first part and about 4-5 on the second. 
The first hour of the first part was for parsing, and the rest for the logic.
I felt REALLY slow on this one, but at least I think the code is quite decent.

## About my solution

Since I had read the problem, I thought about a binary count. As there were only two operators, 
I could generate every possible operator combination for the equations using a simple counter 
and some bitwise operations. It actually worked, and I think it is quite optimized.

The second part was harder, but it followed the same principle. Instead of a binary count, 
it needed to be **ternary**, which made it harder because it became impossible to determine the operators
using a single variable counter and bitwise operations. I tried different approaches but eventually
treated the counter as an array of integers and made it count in ternary using a recursive function.

Once that was done, it was really simple: the program executed every possible equation and compared 
its result to the target from the input. If it matched, it added the equation to the results and moved 
on to the next one.

## About my mistakes

My logic was actually correct from the very beginning, but I failed on my first attempt because I didn't 
notice that the input numbers were much larger. Since the example's results were small, my code worked on 
them but not on the actual input data. However, the solution was obvious: I just changed my `int` variables
to `uint64_t` ones, and everything worked fine.

### Note
To run the first puzzle solution, use `day7 [input file] first_part`, and for the second, use 
`day7 [input file] second_part`.