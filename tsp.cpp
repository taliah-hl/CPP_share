/**
 * 印度佬教學exmaple:
 * https://www.youtube.com/watch?v=1FEP_sNb62k
 *
 *
 *
 */

#include <iostream>
#include <queue>
#include <climits>
#include <fstream>
#include <time.h>
#include <chrono>

using namespace std;

#define DIM 5
bool DEBUG = false;
bool FINEDEBUG = false;
bool INFO = false;
bool LOGABLE = false;
bool PRINTANS = true;



class City
{

public:
    int N;
    int **reducedMatrix;
    int cost;
    int cityNum; // number of this city
    int level;
    int *route; // cur path from root to this city

    City(int N, int cityNum) // constructor forroot city
        : N(N), level(0), cost(-1), cityNum(cityNum)
    {
        if (DEBUG)
            cout << "city = " << cityNum << "created as root, level=" << level << endl;
        reducedMatrix = new int *[N];
        for (int i = 0; i < N; i++)
        {
            reducedMatrix[i] = new int[N];
            for (int j = 0; j < N; j++)
            {
                reducedMatrix[i][j] = INT_MAX;
            }
        }
        route = new int[N+1];
        for (int i = 0; i < N+1; i++)
            route[i] = -1;
        route[level] = cityNum;
    }

    City(int N, int cityNum, City *parent)
        : N(N), level(parent->level + 1), cost(-1), cityNum(cityNum)
    {

        // copy parent->reducedMatrix to this->reducedMatrix
        reducedMatrix = new int *[N];
        for (int i = 0; i < N; i++)
        {
            reducedMatrix[i] = new int[N];
        }

        // Copy parent's reducedMatrix to this reducedMatrix
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                reducedMatrix[i][j] = parent->reducedMatrix[i][j];
            }
        }

        route = new int[N];
        for (int i = 0; i < N; i++)
        {
            route[i] = parent->route[i];
        }

        // Add this city to the route
        route[level] = cityNum;

        if (DEBUG)
            cout << "city = " << cityNum << " created, parent=city" << parent->cityNum << " level=" << level << endl;
    }

    ~City()
    {
        for (int i = 0; i < N; i++)
        {
            delete[] reducedMatrix[i];
        }
        delete[] reducedMatrix;
        delete[] route;
    }

    void initReducedMatrix(int **costGraph)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                reducedMatrix[i][j] = costGraph[i][j];
            }
        }
    }
};

int reducedRow(int **matrix, int N)
{

    if (FINEDEBUG)
        cout << "entered reducedRow\n";
    // calculate row min
    int *row_min = new int[N];
    for (int i = 0; i < N; i++)
        row_min[i] = INT_MAX;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            row_min[i] = min(row_min[i], matrix[i][j]);
        }
    }
    for (int i = 0; i < N; i++)
    {
        if (row_min[i] != 0 && row_min[i] != INT_MAX)
        {
            for (int j = 0; j < N; j++)
            {
                if (matrix[i][j] != INT_MAX)
                    matrix[i][j] -= row_min[i];
            }
        }
    }
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        if (row_min[i] != INT_MAX)
        {
            sum += row_min[i];
        }
    }

    if (FINEDEBUG)
        cout << "reducedRow done\n";
    return sum;
}

int reducedColumn(int **matrix, int N)
{

    if (FINEDEBUG)
        cout << "entered reducedColumn\n";
    // calculate column min
    int *col_min = new int[N];
    for (int i = 0; i < N; i++)
        col_min[i] = INT_MAX;
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < N; i++)
        {
            col_min[j] = min(col_min[j], matrix[i][j]);
        }
    }
    for (int i = 0; i < N; i++)
    {
        if (col_min[i] != 0 && col_min[i] != INT_MAX)
        {
            for (int j = 0; j < N; j++)
            {
                if (matrix[j][i] != INT_MAX)
                    matrix[j][i] -= col_min[i];
            }
        }
    }
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
        if (col_min[i] != INT_MAX)
        {
            sum += col_min[i];
        }
    }

    if (FINEDEBUG)
        cout << "reducedColumn done\n";
    return sum;
}

int getCostI(City *i, int num_i, int N)
{
    if (DEBUG)
        cout << "entered getCostI for num_i: " << num_i << "\n";
    int sum = 0;
    sum += reducedRow(i->reducedMatrix, N);
    sum += reducedColumn(i->reducedMatrix, N);
    i->cost = sum;
    if (DEBUG)
        cout << "debug: city" << num_i << "'s cost updated to " << i->cost << "\n";
    return i->cost;
}

int getCostIj(City *from, City *to,
              int num_from, int num_to, int N)
{
    /*
     * @brief cost from city i to j.
     *
     * @return cost from i to j
     *
     */
    if (DEBUG)
        cout << "debug: entered getCostIj for from city: " << num_from << " to city: " << num_to << "\n";
    for (int k = 0; k < N; k++)
    {

        // Set row i to INF
        to->reducedMatrix[num_from][k] = INT_MAX;

        /// set col j to INF
        to->reducedMatrix[k][num_to] = INT_MAX;
        // set j to start point to INF
        to->reducedMatrix[num_to][0] = INT_MAX;
    }
    int sumRowColMin = 0;
    sumRowColMin += reducedRow(to->reducedMatrix, N);
    sumRowColMin += reducedColumn(to->reducedMatrix, N);

    to->cost = from->cost + sumRowColMin + from->reducedMatrix[num_from][num_to];
    if (DEBUG)
        cout << "debug: city" << num_to << "'s cost updated to " << to->cost << "\n";
    return to->cost;
}

// customized comparing function for priority queue
struct CityCompareFn
{
    bool operator()(const City *a, const City *b) const
    {
        return a->cost > b->cost;
    }
};

void solver(int n, int **CostGraph)
{

    priority_queue<City *, vector<City *>, CityCompareFn> q;
    if (FINEDEBUG)
        cout << "debug: priority_queue created\n";

    // create vector of City
    City *root = new City(n, 0);
    root->initReducedMatrix(CostGraph);
    if (DEBUG)
        cout << "debug: city root created\n";

    getCostI(root, 0, n);
    q.push(root);

    while (!q.empty())
    {
        if (FINEDEBUG)
            cout << "debug: entered while(! q.empty()) loop\n";
        City *minCity = q.top(); // Get city with minimum cost
        q.pop();

        if(DEBUG) cout << "examining city " << minCity->cityNum << "level=" << minCity->level << endl;

        // If all cities have been visited
        if (minCity->level == n - 1)
        {

            if (DEBUG)
                cout << "debug: all cities visited\n";
            // Return to the starting city
            minCity->route[(minCity->level)+1] = minCity->route[0];
            // Print the route

            if (PRINTANS)
            {
                if (INFO) cout << "Route: ";
                for (int i = 0; i < n+1; i++)
                {
                    cout << minCity->route[i] << " ";
                }
                if (INFO) cout << "\nCost: " ;
                cout << endl << minCity->cost;
            }


            if (LOGABLE)
            {
                ofstream logFile;

                // write log
                logFile.open("./log/log.txt", std::ios_base::app);
                if (logFile.is_open())
                {
                    logFile << "path: \n";
                    for (int i = 0; i < n; i++)
                    {
                        logFile << minCity->route[i] + 1 << " ";
                    }
                    logFile << "\n";
                    logFile << "cost: " << minCity->cost << "\n";
                }
                logFile.close();
                logFile.open("./log/bb_result.txt", std::ios_base::app);

                // only write result
                if (logFile.is_open())
                {
                    logFile << minCity->cost << "\n";
                }
                logFile.close();
            }
            // Deallocate memory for all cities in the queue
            while (!q.empty())
            {
                City *city = q.top();
                q.pop();
                delete city;
            }

            // Deallocate memory for the minCity
            delete minCity;
            return;
        }

        // level not reached n yet
        // create node for each city that has path to
        for (int i = 0; i < n; i++)
        {
            if (DEBUG)
                cout << "debug: entered for loop for traversing city, i= " << i << "\n";
            if (minCity->reducedMatrix[minCity->cityNum][i] != INT_MAX)
            {
                // Create a new city and calculate its cost
                City *city = new City(n, i, minCity); // create child node
                getCostIj(minCity, city, minCity->cityNum, i, n);
                q.push(city);
            }
        }
    }
}

int driver(int **arr, int dim)
{

    // int CostGraphMatrix[DIM][DIM] = { { INT_MAX, 20,30, 10, 11 },
    //                                 { 15, INT_MAX, 16, 4, 2 },
    //                                 { 3, 5, INT_MAX, 2, 4},
    //                                 { 19, 6, 18, INT_MAX, 3},
    //                                 {16, 4, 7, 16, INT_MAX} };

    //  write log to log.txt
    if (LOGABLE)
    {
        ofstream logFile;
        logFile.open("./log/log.txt", std::ios_base::app);
        if (logFile.is_open())
        {
            logFile << "input\n";
            for (int i = 0; i < dim; i++)
            {
                for (int j = 0; j < dim; j++)
                {
                    logFile << arr[i][j] << " ";
                }
                logFile << "\n";
            }
        }
        else
        {
            if (INFO)
                cout << "log file open failed, continue without logging\n";
        }
        logFile.close();
    }

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (arr[i][j] == -1)
                arr[i][j] = INT_MAX;
        }
    }

    // priint arr to check
    if (INFO)
    {
        cout << "check input:.....\n";
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                cout << arr[i][j] << " ";
            }
            cout << "\n";
        }
    }

    solver(dim, arr);
}

int main()
{

    // ss is numeric

    int **arr;
    int dim;
    int tmp;
    cin >> dim;
    arr = new int *[dim];
    for (int i = 0; i < dim; i++)
        arr[i] = new int[dim];

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            cin >> tmp;
            arr[i][j] = tmp;
        }
    }

    auto tStart = std::chrono::high_resolution_clock::now();

    // do something with arr[i][j]
    driver(arr, dim);

    auto tEnd = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> ms = tEnd - tStart;

    

    for (int i = 0; i < dim; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;
}
