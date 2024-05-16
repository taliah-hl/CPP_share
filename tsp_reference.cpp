#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;


// implementation of traveling Salesman Problem
int travllingSalesmanProblem(int** graph, int s, vector<int>& best_path, int V)
{
    // store all vertex apart from source vertex
    vector<int> vertex;
    for (int i = 0; i < V; i++)
        if (i != s)
            vertex.push_back(i);

    // store minimum weight Hamiltonian Cycle.
    int min_path = INT_MAX;
    do {
        // store current Path weight(cost)
        int current_pathweight = 0;

        // compute current path weight
        int k = s;
        for (int i = 0; i < vertex.size(); i++) {
            current_pathweight += graph[k][vertex[i]];
            k = vertex[i];
        }
        current_pathweight += graph[k][s];

        // update minimum
        if(min_path > current_pathweight) {
            min_path = current_pathweight;
            best_path = vertex;
        }

    } while (next_permutation(vertex.begin(), vertex.end()));

    return min_path;
}

// Driver Code
// Driver Code
int main()
{
    // read the number of vertices
    int V;
    cin >> V;

    // matrix representation of graph
    int** graph = new int*[V];
    for(int i = 0; i < V; ++i)
        graph[i] = new int[V];

    // read the graph
    for(int i = 0; i < V; ++i)
        for(int j = 0; j < V; ++j)
            cin >> graph[i][j];

    int s = 0;
    vector<int> best_path;
    cout << "Minimum cost : " << travllingSalesmanProblem(graph, s, best_path, V) << endl;
    cout << "Best path : " << s << " ";
    for(int i=0; i<best_path.size(); i++)
        cout << best_path[i] << " ";
    cout << s << endl;

    // free the memory allocated for the graph
    for(int i = 0; i < V; ++i)
        delete [] graph[i];
    delete [] graph;

    return 0;
}
