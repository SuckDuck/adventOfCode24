Ok, from now on, I’ll try to add some comments here about my experience with the challenge. 
Once I’m done with every challenge, I’ll rewrite this in a better way. 😄

I didn’t find this challenge hard, so I don’t have much to say about the exercise itself. 
However, I do have a few things to mention about my current skills in C:
I spent around 3 and a half hours on this one, but most of that time was just parsing the 
file—something I could probably do in Python in under 10 minutes. 
This shows that I don’t really know how to work with strings properly in C.

Another thing I want to point out is about parsing and debugging. I wasted a lot of time 
using print statements to test and ensure everything was working at every step. 
I think I could have avoided this if I knew how to use a proper debugger. 
Hopefully, by the time I finish this challenge, I’ll have learned more about gdb or 
another debugging tool.

Now about the challenge itself:

As I mentioned earlier, I didn’t find it hard. I spent about 15–20 minutes thinking through 
the solution, and when I finally coded it, it worked on the first attempt. That said, 
I did a lot of iterations, so there’s probably a more elegant solution out there. 
Still, I don’t dislike my approach.

For the second part, I spent around 30 minutes because I made an error in the order of execution 
within the algorithm. Briefly explained, I did a kind of strange bubble sort (I think?) where 
I iterated over the numbers in each group separately, going from the last number to the first. 
For each number, I compared it against every rule that applied to it. If I found an invalid 
number, I swapped it with the current number and restarted the process for the same group 
until everything was correct.

The logic is likely unoptimized, but it works. My mistake was that I was iterating over the 
groups from the first number to the last instead of the other way around. 
Once I fixed that, it worked as expected.