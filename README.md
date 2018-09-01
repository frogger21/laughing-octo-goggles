# laughing-octo-goggles
Kosaraju's Algorithm in iterative implementation in C++.

Takes input file and spits out two output files.
Input file: Array of A B where A->B, they are seperated by space (2 numbers per row).
Output file1: vertex & its leader per row for all vertices.
Output file2: Array of leader node & its size, sorted in order from biggest to smallest

*Change filepaths in the code.


Wikipedia on Kosaraju Algo: In the mathematical theory of directed graphs, a graph is said to be strongly connected or diconnected if every vertex is reachable from every other vertex

source: wikipedia. 3 strongly connected components SCC in a directed graph.
![alt text](https://github.com/frogger21/laughing-octo-goggles/blob/master/Scc.png)

Due to the size of the input file the algo was modified to be iterative instead of its original recursive nature. I was constantly getting the stack-over-flow error with the recursive method and hence went for the iterative one.

There are over 5 million edges in the directed graph.
The Kosaraju algo will find all the SCCs.
