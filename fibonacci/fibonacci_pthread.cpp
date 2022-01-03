#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int n;                          
int *fibseq;                    
int i;                          

void *runn(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << ":./a.out" << endl;
        return -1;
    }                           

    if (atoi(argv[1]) < 0)
    {
        cout << atoi(argv[1]) << " >=0!" << endl;
        return -1;
    }                           

    n = atoi(argv[1]);
    fibseq = (int *)malloc(n * sizeof(int));
    pthread_t *threads = (pthread_t *) malloc(n * sizeof(pthread_t));
    pthread_attr_t attr;        
    pthread_attr_init(&attr);

    for (i = 0; i < n; i++)
    {
        pthread_create(&threads[i], &attr, runn, NULL);
    }                           

    int j;

    for (j = 0; j < n; j++)
    {
        pthread_join(threads[j], NULL);
    }                           
    int k;

    for (k = 0; k < n; k++)
    {
        cout << fibseq[k] << endl;
    }                           
    return 0;
}                              

void *runn(void *arg)
{
    if (i == 0)
    {
        fibseq[i] = 0;
        pthread_exit(0);
    }                           
    if (i == 1)
    {
        fibseq[i] = 1;
        pthread_exit(0);
    }                           
    else
    {
        fibseq[i] = fibseq[i - 1] + fibseq[i - 2];
        pthread_exit(0);        
    }                           
}  