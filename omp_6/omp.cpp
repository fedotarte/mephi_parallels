#include <iostream>
#include <vector>

#include <math.h>
#include "stdio.h"
#include <omp.h>

using namespace std;

#define THREADCOUNT 4

double diff;

vector<pair<double, double>> answer;

double WTFFunction(double x)
{
    return (3 * cos(pow(x, 3) + sin(8 * pow(x, 8)))) * pow(x, 5);
}

pair<double, double> GetBoundaries(double inf, double sup, int thread_id)
{
    double minValue = WTFFunction(inf), maxValue = WTFFunction(inf);
    while (inf <= sup)
    {
        inf += diff;
        minValue = min(minValue, WTFFunction(inf));
        maxValue = max(maxValue, WTFFunction(inf));
    }
    return make_pair(minValue, maxValue);
}

int main()
{
    double inf = 0, sup = 100;
    diff = 1e-4;
    omp_set_num_threads(THREADCOUNT);
    double global_minimum = 0, global_maximum = 0;

#pragma omp parallel shared(global_minimum, global_maximum)
    {
        for (int thread_index = 0; thread_index < THREADCOUNT; thread_index++)
        {
            if (thread_index == omp_get_thread_num())
            {
                const double param_inf = inf + thread_index * (sup - inf) / THREADCOUNT;
                const double param_sup = inf + (thread_index + 1) * (sup - inf) / THREADCOUNT;
                answer.push_back(GetBoundaries(param_inf, param_sup, thread_index));
            }
        }

#pragma omp barrier
        for (int counter = 0; counter < THREADCOUNT; counter++)
        {
            if (counter == omp_get_thread_num())
            {
                global_minimum = min(global_minimum, answer[counter].first);
                global_maximum = max(global_maximum, answer[counter].second);
                cout << "global min " << global_minimum << endl;
                cout << "global max " << global_maximum << endl;
            }
        }

#pragma omp barrier
        if (omp_get_thread_num() == 0)
        {
            cout << "Number of threads " << THREADCOUNT << '\n'
                 << "Min value is " << global_minimum << '\n'
                 << "Max value is " << global_maximum << '\n';
        }
    }
}