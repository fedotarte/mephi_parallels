#include <iostream>
#include <random>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <algorithm>

#include <stdlib.h>
#include <mpi.h>

using namespace std;

typedef struct min_max_struct
{
    double lmin;
    double lmax;
};

void Count_Min_Max(int l, int k, double& minimum, double& maximum);

double WTFFunction(int x);


int main(int argc, char** argv)
{

    MPI_Init(NULL, NULL);
    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int started = 0;
    int ended = 100;

    int check_point = (ended - started) / (world_size - 1);
    int estimated = (ended - started) % (world_size - 1);

    if (world_rank == 0)
    {
        for (int i = world_rank + 1; i < world_size; i++)
        {
            int start = check_point * (i - 1);
            MPI_Send(
                &start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        double min = 0;
        double max = 0;
        for (int i = world_rank + 1; i < world_size; i++)
        {
            cout << "recieving results from: " << i << endl;
            MPI_Probe(i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            min_max_struct* malloc_workspace = (min_max_struct*)malloc(sizeof(min_max_struct));

            MPI_Recv(malloc_workspace, 2, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (malloc_workspace && malloc_workspace->lmin < min)
                min = malloc_workspace->lmin;
            if (malloc_workspace && malloc_workspace->lmax > max)
                max = malloc_workspace->lmax;
            free(malloc_workspace);
        }
        cout << "min = " << min << ";" << "\t" << "max = " << max << endl;
    }
    else
    {
        int start;
        MPI_Probe(
            0, 
            0, 
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );                    
        MPI_Recv(
            &start, 
            1, 
            MPI_INT, 
            0, 
            0, 
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        ); 
        int length = start + check_point;
        if (world_rank == world_size - 1) 
            length += estimated;
        cout << "Worker_" << world_rank << " starting on " << world_rank << "..." << length << endl;
        min_max_struct* resource_structure = (min_max_struct*)malloc(sizeof(min_max_struct));
        if (resource_structure) {
            Count_Min_Max(
                start, 
                length, 
                resource_structure->lmin, 
                resource_structure->lmax
            );
            cout << "Worker_" << world_rank << ": min = " << resource_structure->lmin << ", max = " << resource_structure->lmax << endl;
            MPI_Send(
                resource_structure, 
                2, 
                MPI_DOUBLE, 
                0, 
                0, 
                MPI_COMM_WORLD
            );
        }
    }
    MPI_Finalize();
    return 0;
}


double WTFFunction(int x)
{
    return (3 * cos(pow(x, 3) + sin(8 * pow(x, 8)))) * pow(x, 5);
}


void Count_Min_Max(int start, int length, double& minimum, double& maximum)
{
    double y;
    double max_value = WTFFunction(length);
    double min_value = WTFFunction(start);
    for (int i = start; i <= length; i++)
    {
        y = WTFFunction(i);
        if (y < min_value)
        {
            min_value = y;
        };
        if (y >= max_value)
        {
            max_value = y;
        };
    }

    minimum = min_value;
    maximum = max_value;
}