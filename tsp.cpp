#include <iostream>
#include <queue>
#include <climits>



using namespace std;

#define DIM 4
bool DEBUG = true;

class City{

    public:
    int N;
    int** reducedMatrix;
    int cost;
    int num; // number of this city
    int level;
    int* route; // cur path from root to this city

    City(int N, int num) // constructor forroot city
    :N(N), level(0), cost(-1), num(num){
        reducedMatrix = new int*[N];
        for (int i=0;i<N;i++){
            reducedMatrix[i] = new int[N];
            for (int j=0;j<N;j++){
                reducedMatrix[i][j] = INT_MAX;
            }
        }
        route = new int[N];
        for (int i=0;i<N;i++) route[i] = -1;
        route[level] = num;
    }

    City(int N, int num, City* parent)
    :N(N), level(parent->level + 1), cost(-1), num(num){

        //copy parent->reducedMatrix to this->reducedMatrix
        reducedMatrix = new int*[N];
        for (int i = 0; i < N; i++) {
            reducedMatrix[i] = new int[N];
        }

        // Copy parent's reducedMatrix to this reducedMatrix
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                reducedMatrix[i][j] = parent->reducedMatrix[i][j];
            }
        }

        route = new int[N];
        for (int i = 0; i < N; i++) {
            route[i] = parent->route[i];
        }

        // Add this city to the route
        route[level] = num;
    }

    void initReducedMatrix(int** costGraph){
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                reducedMatrix[i][j] = costGraph[i][j];
            }
        }
    }
};

int reducedRow(int ** matrix, int N){

    if(DEBUG) cout << "entered reducedRow\n";
    // cal row min
    int* row_min = new int[N];
    for(int i=0;i<N;i++) row_min[i] = INT_MAX;
    for (int i =0;i<N;i++){
        for(int j=0;j<N;j++){
            row_min[i] = min(row_min[i], matrix[i][j]);
        }
        
    }
    int min_row_min = INT_MAX;
    for(int i=0;i<N;i++) min_row_min = min(min_row_min, row_min[i]);
    if(min_row_min != 0 && min_row_min != INT_MAX){
        // need row reduction
        for (int i =0;i<N;i++){
            for(int j=0;j<N;j++){
                if(matrix[i][j]!= INT_MAX)
                    matrix[i][j] -= min_row_min;
            }
        }
    }

    if(DEBUG) cout << "reducedRow done\n";
    int sum = 0;
    for(int i=0;i<N;i++) sum+= row_min[i];
    return sum;
}

int reducedColumn(int ** matrix, int N){

    if(DEBUG) cout << "entered reducedColumn\n";
    // calculate column min
    int* col_min = new int[N];
    for(int i=0;i<N;i++) col_min[i] = INT_MAX;
    for (int j =0;j<N;j++){
        for(int i=0;i<N;i++){
            col_min[j] = min(col_min[j], matrix[i][j]);
        }
    }
    int min_col_min = INT_MAX;
    for(int i=0;i<N;i++) min_col_min = min(min_col_min, col_min[i]);
    if(min_col_min != 0 && min_col_min != INT_MAX){
        // need column reduction
        for (int j =0;j<N;j++){
            for(int i=0;i<N;i++){
                if(matrix[i][j]!= INT_MAX)
                    matrix[i][j] -= min_col_min;
            }
        }
    }
    int sum=0;
    for(int i=0;i<N;i++) sum+= col_min[i];
    if(DEBUG) cout << "reducedColumn done\n";
    return sum;
}

int getCostI(City* i, int num_i, int N){
    if(DEBUG) cout << "entered getCostI for num_i: " << num_i << "\n";
    i->cost = reducedRow(i->reducedMatrix, N) + reducedColumn(i->reducedMatrix, N);
    if(DEBUG) cout << "debug: city" << num_i <<"'s cost updated to " << i->cost << "\n";
    return i->cost;
}

int getCostIj(City* i, City* j,  
    int num_i, int num_j,  int N){
    /*
     * @brief cost from city i to j.
     * 
     * @return cost from i to j
     * 
    */
    if(DEBUG) cout << "debug: entered getCostIj for num_i: " << num_i << " num_j: " << num_j << "\n";
   for (int k = 0; k < N; k++) {
        
        // Set row i to INF
        j->reducedMatrix[num_i][k] = INT_MAX;
        
        /// set col j to INF
        j->reducedMatrix[k][num_j] = INT_MAX;
        // set j to start point to INF
        j->reducedMatrix[num_j][0] = INT_MAX;
    }

    int sumRowColMin = reducedRow(i->reducedMatrix, N) + reducedColumn(i->reducedMatrix, N);
    j->cost = i->cost + sumRowColMin + i->reducedMatrix[num_i][num_j];
    if(DEBUG) cout << "debug: city" << num_j <<"'s cost updated to " << j->cost << "\n";
    return j->cost;
 
}

//customized comparing function for priority queue
struct CityCompareFn {
    bool operator()(const City* a, const City* b) const
    {
        return a->cost > b->cost;
    }
};

void solver(int n, int ** CostGraph){


    priority_queue<City*, vector<City*>, CityCompareFn> q;
    if (DEBUG) cout << "debug: priority_queue created\n";

    // create vector of City
    City* root = new City(n, 0);
    root->initReducedMatrix(CostGraph);
    if (DEBUG) cout << "debug: city root created\n";

    getCostI(root, 0, n);
    q.push(root);
    if(DEBUG) cout << "debug: root pushed to q\n";

    while(! q.empty()){
        if(DEBUG) cout << "debug: entered while loop\n";
        City* minCity = q.top();  // Get city with minimum cost
        q.pop();

        // If all cities have been visited
        if (minCity->level == n-1) {

            if(DEBUG) cout << "debug: all cities visited\n";
            // Return to the starting city
            minCity->route[minCity->level] = minCity->route[0];
            // Print the route
            cout << "Route: ";
            for (int i = 0; i <= n; i++) {
                cout << minCity->route[i] +1 << " ";
            }
            cout << "\nCost: " << minCity->cost << endl;
            return;
        }

        // level not reached n yet
        // create node for each city that has path to
        for (int i = 0; i < n; i++) {
            if(DEBUG) cout << "debug: entered for loop for traversing city, i= "<< i<< "\n";
            if (minCity->reducedMatrix[minCity->num][i] != INT_MAX) {
                // Create a new city and calculate its cost
                City* city = new City(n, i, minCity); // create child node
                getCostIj(minCity, city, minCity->num, i, n);
                q.push(city);
            }
        }


    }

}

int main (){
    
    int CostGraphMatrix[DIM][DIM] = { { INT_MAX, 10, 15, 20 },
                                  { 10, INT_MAX, 35, 25 },
                                  { 15, 35, INT_MAX, 30 },
                                  { 20, 25, 30, INT_MAX } };
   
    int** costGraph = new int*[DIM];
    for (int i = 0; i < DIM; i++) {
        costGraph[i] = new int[DIM];
    }

    // Copy values from CostGraphMatrix to costGraph
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            costGraph[i][j] = CostGraphMatrix[i][j];
        }
    }

    solver(DIM, costGraph);


}
