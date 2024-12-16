## Overview

This puzzle was both challenging and fun, and I can already feel myself improving in C. 
The difficulty didn’t lie in the logic or the concept but rather in the optimization required. 
With every "blink," the numbers grow larger, and more numbers are generated, so if the process 
isn’t managed efficiently, it quickly spirals out of control.

I spent about 1.5 hours on the first part, and my initial solution took around 15 seconds to 
execute. However, the second part was far more demanding and took me an additional 3 hours to 
solve due to the need for advanced optimizations.

## About my solution

The general process of my solution is straightforward. I represented the input as an array of 
integers, and the program's main loop iterates through all the numbers for each blink, applying 
the rules defined in the problem statement.

The second rule (splitting a stone into two) was the most interesting to implement. This rule 
requires adding a new element to the main array, which meant reallocating memory and shifting 
all subsequent elements to the right before placing the two resulting stones in their respective 
positions.

This approach worked well for the 25 blinks required in the first part. However, in the second 
part, the process quickly became unmanageable. After about 30 blinks, the solution slowed 
significantly due to exponential growth in the number of stones, forcing me to implement two 
key optimizations.

### Optimization 1: Avoiding Array Shifting

Shifting all elements to the right when splitting a stone became extremely slow as the number 
of stones increased. To address this, I changed my approach. Instead of shifting elements, the 
stone being split takes the value of the first new stone, and the second new stone is placed 
at the end of the array. This eliminated the need for costly array shifts.

### Optimization 2: Reducing Redundant Stones

Inspired by a term I saw in a Reddit post—"trim and shrink"—I noticed that many of the numbers 
generated during each blink were duplicates, primarily due to the first and third rules. 
To optimize this, I changed the representation of the stones from an array of integers to an 
array of `stone` structs. Each struct contains a `value` and a `weight` field.

#### I split the process into two phases:
1. **Reduction Phase**: This phase identifies and consolidates duplicate stones. 
When a duplicate is found, its `weight` is added to the first instance, and the duplicate is removed 
from the array.

2. **Generation Phase**: This phase applies the original rules to generate new stones and updates 
the array as needed.

By combining these optimizations, I significantly reduced the size of the array and improved 
performance, making it possible to handle the 75 blinks in the second part in just 730ms

Finally, to compute the result, I iterate through the stones one last time and sum up their `weights`.

## About my mistakes

This time, I didn’t encounter any notable mistakes. 
The primary challenges were related to optimization, which I addressed as described above.

### Note

To run the program, use: `day11 [input file] [blinks]`
