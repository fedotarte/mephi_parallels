#include <iostream>
#include <limits>
#include <cmath>
// vse taki sdelat s krugami
double f(double x)
{
    return sin(5 * pow(x, 3)) + cos(7 * pow(x, 4)) * x;
}
/*
a,b  - inf(x), sup(x),
h - step 
*/
int main()
{
    double max_x = 100;
    double h = 1e-4;
    double x = 0;
    double min_x, min_y;
    double cur_f;
    min_y = std::numeric_limits<double>::max(); // min_y  - kak v node -Math.Infinity

    while (x < max_x)
    {
        cur_f = f(x);
        if (cur_f < min_y)
        {
            min_y = cur_f;
            min_x = x;
        }
        x += h;
    }

    std::cout << min_y << " " << min_x;
}
