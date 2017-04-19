The program is written in C++11, can be compiled with any standard C++11 compiler (I used MinGW).

Input:
Input to the program can be in one of the following ways, the mode can be changed by #defining the corresponding variable:
1. store the input in 'input.txt' file (stdIO)
2. random input can be generated in run-time (randomInput)
3. hard-code the input in the program (fixedInput)

Heuristic:
The  heuristic is computed in 2 steps:
1. Stack #1 is evaluated. The heuristic is simply twice the number of misplaced blocks in stack1. The intuition is that each of the misplaced tile here has to be removed and replaced, hence cost of 2 per block is a lower-bound for the cost.
2. For all the other stacks, we do the following: each blocks adds a cost=1 and for each bigger block on top of a smaller block, there is additional cost=1. The intuition here is that each node needs atleast one move to be placed in stack1 and the bigger blocks are to be moved to a temporary stack to move the smaller block to stack1.
The heuristic is admissible since we never over-estimate the cost, we only consider the minimal cost needed.

Restriction:
The implementation is such that it could run for #stacks + #blocks <= 17.

Output:
The program outputs the following data to StdOut:
1. The path from start configuration to end configuration.
2. The summary statistics.