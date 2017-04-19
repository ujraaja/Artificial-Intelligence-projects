The program was written in C++11, can be compiled with any standard C++11 compiler (I used MinGW).

Input:
The program takes the input from input.txt in the same folder as the executable. The input file should contain:
1. Propositional rules
2. Facts
3. Negation of the statement to be entailed

The input file may contain comments starting with '#'; two consecutive blank lines is considered to be the end of the input. Since the input is read with getline() function, the last line of the input should be followed by a newline character.


Output:
The output displays if the entailment was successful, the number of iterations and the maximum queue size. Additionally, if the entailment succeeded, the trace of derivation is displayed.
