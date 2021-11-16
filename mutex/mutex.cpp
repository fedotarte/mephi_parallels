#include <windows.h>
#include <stdio.h>

#define THREADCOUNT 4

HANDLE ghMutex;

DWORD WINAPI WriteToDatabase(LPVOID);
 //   'https://dev-api-internal-op.apigee.lmru.tech/orders/detailed-solution/v1/detailed-solutions:search?page=0&size=20&required=payment&required=fulfillment&required=delivery' \
// https://test-api-internal-op.apigee.lmru.tech/orders/detailed-solution/v1/detailed-solutions:search?page=0&size=10
int main(void)
{
    HANDLE aThread[THREADCOUNT];
    DWORD ThreadID;
    int i;

    // Create a mutex with no initial owner

    ghMutex = CreateMutex(
        NULL,             // default security attributes
        FALSE,            // initially not owned
        "MutextSheeeit"); // unnamed mutex

    if (ghMutex == NULL)
    {
        printf("CreateMutex error: %d\n", GetLastError());
        return 1;
    }

    // Create worker threads

    for (i = 0; i < THREADCOUNT; i++)
    {
        aThread[i] = CreateThread(
            NULL, // default security attributes
            0,    // default stack size
            (LPTHREAD_START_ROUTINE)WriteToDatabase,
            NULL,       // no thread function arguments
            0,          // default creation flags
            &ThreadID); // receive thread identifier

        if (aThread[i] == NULL)
        {
            printf("CreateThread error: %d\n", GetLastError());
            return 1;
        }
    }

    // Wait for all threads to terminate

    WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

    // Close thread and mutex handles

    for (i = 0; i < THREADCOUNT; i++)
        CloseHandle(aThread[i]);

    CloseHandle(ghMutex);

    return 0;
}

DWORD WINAPI WriteToDatabase(LPVOID lpParam)
{
    // lpParam not used in this example
    UNREFERENCED_PARAMETER(lpParam);

    DWORD dwCount = 0, dwWaitResult;

    // Request ownership of mutex.

    while (dwCount < 20)
    {
        dwWaitResult = WaitForSingleObject(
            ghMutex,   // handle to mutex
            INFINITE); // no time-out interval

        switch (dwWaitResult)
        {
        // The thread got ownership of the mutex
        case WAIT_OBJECT_0:
            __try
            {
                // TODO: Write to the database
                printf("Thread %d writing to database...\n",
                       GetCurrentThreadId());
                dwCount++;
            }

            __finally
            {
                // Release ownership of the mutex object
                if (!ReleaseMutex(ghMutex))
                {
                    // Handle error.
                }
            }
            break;

        // The thread got ownership of an abandoned mutex
        // The database is in an indeterminate state
        case WAIT_ABANDONED:
            return FALSE;
        }
    }
    return TRUE;
}