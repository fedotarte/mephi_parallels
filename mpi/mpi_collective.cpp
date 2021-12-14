#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <iostream>

using namespace std;

vector<pair<double, double>> answer;

double WTFFunction(double x);

pair <double, double> MinMaxHelper(double left, double right, double delta);


int main(int argc, char** argv)
{
    int world_rank, world_size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    double start = 0;
    double end = 100;
    double delta = 1e-4;

    double perproc = (end - start) / world_size;

    double global_minimum = 0;
    double global_maximum = 0;

    pair<double, double> mm_local;

    mm_local = MinMaxHelper(start + world_rank * perproc, start + (world_rank + 1) * perproc, delta);

    MPI_Reduce(&mm_local.first, &global_minimum, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
//     if(rank!= 0){
//       MPI_Send(&MinMaxHelper, 4, )
//     }
    MPI_Reduce(&mm_local.second, &global_maximum, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
   

    if (world_rank == 0) {
        cout << "MIN is : " << global_minimum << endl;
        cout << "MAX is : " << global_maximum << endl;
    }

    MPI_Finalize();

    return 0;
}

double WTFFunction(double x) {
    return (3 * cos(pow(x, 3) + sin(8 * pow(x, 8)))) * pow(x, 5);
}

pair <double, double> MinMaxHelper(double inf, double sup, double diff) {
    double reduced_min = WTFFunction(inf), reduced_max = WTFFunction(inf);
    while (inf <= sup) {
        inf += diff;
        reduced_min = min(reduced_min, WTFFunction(inf));
        reduced_max = max(reduced_max, WTFFunction(inf));
    }
    return make_pair(reduced_min, reduced_max);
}
