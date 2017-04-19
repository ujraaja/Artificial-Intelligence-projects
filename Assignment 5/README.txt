The program was written in C++11, can be compiled with any standard C++11 compiler (I used GCC).

Input:
The program takes the input from input.txt in the same folder as the executable. The input file should contain:
1. Initial constraints (task assignment to agents and 3 agent capacity)
2. Requirements (tasks to be performed)

The input file may contain comments starting with '#'; two consecutive blank lines is considered to be the end of the input. Since the input is read with getline() function, the last line of the input should be followed by a newline character.


Output:
The output displays if the entailment was successful and the number of iterations. Additionally, if the entailment succeeded, the chosen agents are displayed.
