#include <iostream>
#include <algorithm>
#include <random>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265

int main()
{
    HANDLE mkEvent() { return CreateEvent(NULL, FALSE, FALSE, NULL); }
    int theta, r = 1;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 100);
    // cout << dist6(rng) << std::endl;
    const int SIZE = dist6(rng);
    printf("randomed num is: (%d)", SIZE);

    int arr[SIZE];
    std::fill(arr, arr + SIZE, 1);

    for (size_t i = 0; i < SIZE - 1; i++)
    {
        std::cout << arr[i];
    }

    // float x, y, r, theta;
    // clrscr();
    // printf("Enter radius of polar coordinate (r): ");
    // scanf("%f", &r);
    // printf("Enter angle of polar coordinate in degree (theta): ");
    // scanf("%f", &theta);

    /* from degree to radian */
    theta = theta * PI / 180.0;

    /* Calculating cartesian coordinate x */
    double x = r * cos(theta);

    /* Calculating cartesian coordinate y */
    double y = r * sin(theta);

    // printf("Cartesian coordinates is: (%0.3f, %0.3f)", x, y);
    // getch();
    return 0;
}