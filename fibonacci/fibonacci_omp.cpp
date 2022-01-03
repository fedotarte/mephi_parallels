#include <math.h>
#include <stdio.h>
#include <inttypes.h>
#include <omp.h>

uint64_t fib(unsigned m)
{
  double sqrt5r = 1.0 / sqrt(5.0);
  double golden = (1.0 + sqrt(5.0)) / 2.0;
  return rint(pow(golden, m) * sqrt5r);
}

void fib_sequence(unsigned n, uint64_t *f)
{
#pragma omp parallel
  {
    size_t t = omp_get_thread_num();
    size_t nt = omp_get_num_threads();
    int x0 = (t + 0) * n / nt;
    int x1 = (t + 1) * n / nt;
    f[x0 + 0] = fib(x0 + 0);
    f[x0 + 1] = fib(x0 + 1);
    for (int i = x0 + 2; i < x1; i++)
    {
      f[i] = f[i - 2] + f[i - 1];
    }
  }
}

int main(void)
{
  const int n = 71;
  uint64_t f1[n], f2[n];
  for (int i = 0; i < n; i++)
    f1[i] = fib(i);
  fib_sequence(n, f2);
  for (int i = 0; i < n; i++)
    if (f1[i] != f2[i])
      printf("%d\n", i);
  for (int i = 0; i < n; i++)
    printf("F%d: %" PRId64 " %" PRId64 "\n", i, f1[i], f2[i]);
  // for (int i = 0; i < n; i++)
  // {
  //   delete[] &f1[i];
  //   delete[] &f2[i];
  // }
  // delete[] f1;
  // delete[] f2;
}