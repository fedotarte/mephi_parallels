#include <iostream>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

void DoSheit()
{
    for (size_t i = 0; i < 10; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(700));
        cout << "Thread ID is: " << this_thread::get_id() << "\tDoSheit " << i << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    thread th(DoSheit);
    thread th2(DoSheit);
    for (size_t i = 0; i < 10; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        cout << "Thread ID is: " << this_thread::get_id() << "\tmain " << i << endl;
    }
    th.join();
    th2.join();
    return 0;
}