# выполнить это в командной строке))000

MPI p2p[

файл .cpp для исполнения лежит на уровне с ридмишкой, но исполняемый можно взять из дебага, пример для запуска ниже


```CMD
mpiexec -n 5 C:\Users\Elena_design\cpp_projects\mephi_parallels\mpi\mpi_first\Debug\mpi_first.exe```


Вывод примерно следующий:
```
Worker_1 starting on 1...25
Worker_1: min = -1.42328e+07, max = 1.28176e+07
Worker_2 starting on 2...50
Worker_2: min = -9.29022e+08, max = 6.76537e+08
getting results from: 1
Worker_3 starting on 3...75
getting results from: 2
Worker_3: min = -5.87389e+09, max = 4.77237e+09
Worker_4 starting on 4...100
getting results from: 3
Worker_4: min = -2.70226e+10, max = 2.32492e+10
getting results from: 4
min = -2.70226e+10;     max = 2.32492e+10
```
