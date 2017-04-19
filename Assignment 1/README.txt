The programs are written in C++11, can be compiled with any standard C++11 compiler (I used MinGW).

Input:
Input are directly read from a 'Input.txt' file in the same folder as the compiled executalbes. The program takes input in the following format:
1. First line contains number of vertices
2. Each vertex is defined with 3 integers in the form: VertexID x-coordinate y-coordinate
3. Next line contains number of edges in the graph
4. Each edge is described with 3 integers in the form: EdgeID VertexID1 VertexID2
5. The start and end points for the search are described in the form: VertexID1 VertexID2

Output:
The program outputs the following data to StdOut:
1. The debug data
2. The path from start vertex to end vertex
3. The summary statistics