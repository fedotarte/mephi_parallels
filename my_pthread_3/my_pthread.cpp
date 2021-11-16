#include <iostream>
#include <cmath>
#include <limits>
#include <pthread.h>

using namespace std;

double calculateY(double x){
    return sin(5 * pow(x, 3)) + cos(7 * pow(x, 4)) * x;
}

struct thread_params{
	int thread_num; // thread index number
	double a; // inf
	double b; // sup
};

const int N = 5; // thread quantity
double min_x_[N], min_y_[N]; //arrays for threads
/*
У звездочки несколько смыслов, 
1) после типа void* task <=> таск, это указатель на void
2) разыменование, func_print(int* a) => cout << *a+1
*/
void* foo(void* task){
    thread_params* params = (thread_params*) task; // cast task to the thread params pointer
    double a = params->a; // params.a, iz-za ukdazatelya: ->, eto razyimenovanie 
    double b = params->b; 
    int id = params->thread_num;
    double h = 1e-4; //step
    double x = a; 
    double min_x, min_y;
    double cur_f;
    min_y = std::numeric_limits<double>::max(); // +Math.Infinity

    while (x < b) {
		cur_f = calculateY(x);
		if (cur_f < min_y) {
			min_y = cur_f;
			min_x = x;
		}
		x += h;
	}

    std::cout << "thread " << params->thread_num << " "
		<< min_y << " " << min_x << std::endl;

	min_y_[id] = min_y; // TODO rename array
	min_x_[id] = min_x;
}

int main(){
	thread_params params[N]; // array of structs
    pthread_t pthreads[N]; // array of p_threads descriptors

	double b = 100;

	for (int i = 0; i < N; i++) {
		params[i].thread_num = i;
		params[i].a = b * i / N;
		params[i].b = b * (i + 1) / N;
        // sozdaem tredy
		int return_code = pthread_create(&pthreads[i], NULL, foo, &params[i]);

        if (return_code == 0)
            std::cout << "thread " << i << " was succesfully started" << std::endl;
        else
            std::cout << "thread " << i << " was not succesfully started" << std::endl;
	}
    
	for (int i = 0; i < N; i++) {
		pthread_join(pthreads[i], NULL);  //realizaziya po hash'am? joinyatsya pod kapotom
	}

    double min_x, min_y;
    min_y = std::numeric_limits<double>::max(); // + Infinity
    for (int i = 0; i < N; i++) {
        if (min_y_[i] < min_y){
            min_y = min_y_[i];
            min_x = min_x_[i];
        }
    }

	std::cout << std::endl << min_y << " " << min_x << std::endl;
}