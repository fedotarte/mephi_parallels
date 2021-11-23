#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;
// define a const  for threads
#define THREADCOUNT 6

HANDLE Record_Event;
HANDLE re_Threads[THREADCOUNT];

DWORD WINAPI ThreadProc(LPVOID);

void CreateEventsAndThreads(void);

void WriteToBuffer(VOID);

void CloseEvents();

int main(void)
{
    DWORD dwWaitResult;

    // Create events and THREADCOUNT threads to read from the buffer

    CreateEventsAndThreads();

    // At this point, the reader threads have started and are most
    // likely waiting for the global event to be signaled. However,
    // it is safe to write to the buffer because the event is a
    // manual-reset event.

    WriteToBuffer();

    printf("Main thread waiting for threads to exit...\n");

    // The handle for each thread is signaled when the thread is
    // terminated.
    dwWaitResult = WaitForMultipleObjects(
        THREADCOUNT, // number of handles in array
        re_Threads,   // array of thread handles
        TRUE,        // wait until all are signaled
        INFINITE);

    switch (dwWaitResult)
    {
    // All thread objects were signaled
    case WAIT_OBJECT_0:
        printf("All threads ended, cleaning up for application exit...\n");
        break;

    // An error occurred
    default:
        printf("WaitForMultipleObjects failed (%d)\n", GetLastError());
        return 1;
    }

    // Close the events to clean up

    CloseEvents();

    return 0;
}

void CreateEventsAndThreads(void)
{
    int i;
    // unsigned int for WIN
    DWORD dwThreadID;

    Record_Event = CreateEvent(
        NULL,              // default security attributes
        TRUE,              // lapki-reset event
        FALSE,             // initial state is nonsignaled
        TEXT("WriteEvent") // object name
    );

    if (Record_Event == NULL)
    {
        cout << "CreateEvent failed: " << GetLastError() << endl;
        return;
    }

    // Create multiple threads to read from the buffer.

    for (i = 0; i < THREADCOUNT; i++)
    {
        // TODO: More complex scenarios may require use of a parameter
        //   to the thread procedure, such as an event per thread to
        //   be used for synchronization.
        re_Threads[i] = CreateThread(
            NULL,       // default security
            0,          // default stack size
            ThreadProc, // name of the thread function
            NULL,       // no thread parameters
            0,          // default startup flags
            &dwThreadID);

        if (re_Threads[i] == NULL)
        {
            cout << "CreateThread failed " << GetLastError() << endl;
            return;
        }
    }
}

void WriteToBuffer(VOID)
{
    // TODO: Write to the shared buffer.

    cout << "Main thread writing to the shared buffer" << endl;


    if (!SetEvent(Record_Event))
    {
        cout << "SetEvent failed " << GetLastError() << endl;
        return;
    }
}

void CloseEvents()
{
    CloseHandle(Record_Event);
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwWaitResult;

    printf("Thread %d waiting for record an event...\n", GetCurrentThreadId());
    cout << "Thread №"<< GetCurrentThreadId() << " waiting for record an event" << endl;

    dwWaitResult = WaitForSingleObject(
        Record_Event,
        INFINITE);    

    switch (dwWaitResult)
    {

    case WAIT_OBJECT_0:
        //
        // TODO: Read from the shared buffer
        //
        printf("Thread %d reading from buffer\n",
               GetCurrentThreadId());
        break;

    default:
        printf("Wait error (%d)\n", GetLastError());
        return 0;
    }

    printf("Thread %d exiting\n", GetCurrentThreadId());
    return 1;
}