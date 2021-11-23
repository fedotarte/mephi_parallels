#include <windows.h>
#include <iostream>

using namespace std;

HANDLE event, event_1;

void FirstTask(void *);
void SecondTask(void *);
void LastTask(void *);

void main()
{
    event = CreateEvent(NULL, TRUE, FALSE, "FirstStep");
    if (_beginthread(FirstTask, 1024, NULL) == -1)
                  cout << "Error begin thread " << endl;
    if (_beginthread(SecondTask, 1024, NULL) == -1)
        cout << "Error begin thread " << endl;
    if (_beginthread(LastTask, 1024, NULL) == -1)
        cout << "Error begin thread " << endl;
    if (event != NULL)
    {
        Sleep(1000);
        SetEvent(event);
        Sleep(1000);
        ResetEvent(event);
        CloseHandle(event);
    }
    else
    {
        cout << "error create event" << endl;
    }
}

void FirstTask(void *)
{
    DWORD dwWaitResult;
    while (dwWaitResult != WAIT_OBJECT_0)
    {
        dwWaitResult = WaitForSingleObject(event, 1);
        cout << "Test 1 TIMEOUT" << endl;
    }
    cout << "Event Test 1 " << endl;
    _endthread();
}

void SecondTask(void *)
{
    DWORD dwWaitResult;
    while (dwWaitResult != WAIT_OBJECT_0)
    {
        dwWaitResult = WaitForSingleObject(event, 1);
        cout << "Test 2 TIMEOUT" << endl;
    }
    cout << "Event Test 2 " << endl;
    _endthread();
}

void LastTask(void *)
{
    DWORD dwWaitResult;
    while (dwWaitResult != WAIT_OBJECT_0)
    {
        dwWaitResult = WaitForSingleObject(event, 1);
        cout << "Test 3 TIMEOUT" << endl;
    }
    cout << "Event Test 3 " << endl;
    _endthread();
}