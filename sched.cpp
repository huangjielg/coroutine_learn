#include <iostream>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <string>
#include <mutex>

#include <functional>
#include <coroutine>

#include <windows.h>
#include "sched.h"
using namespace cpp_test;
HANDLE Scheduler::hIoComplete = INVALID_HANDLE_VALUE;

INT Scheduler::Init()
{
    hIoComplete = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
    return hIoComplete!=NULL;
}
VOID  Scheduler::DeInit()
{
    CloseHandle(hIoComplete);hIoComplete=INVALID_HANDLE_VALUE;
}

void Scheduler::Queue(IoCompleteLamda &lo)
{
 
    ::PostQueuedCompletionStatus(
        hIoComplete,
        0,
        0,
        &lo.o);
}

 INT Scheduler::ProcessOneEvent(DWORD   dwMilliseconds)
 {
     DWORD nRead;
     ULONG_PTR key;
     LPOVERLAPPED po;
     if(GetQueuedCompletionStatus(
               hIoComplete,
                &nRead,
                &key,
                &po,
               dwMilliseconds
        )
     )
     {
          IoCompleteLamda *p=(IoCompleteLamda*)po;
          p->do_callback();
          return TRUE;
     }else{
         return FALSE;
     }
 }


  BOOL  Scheduler::BindFile(HANDLE hFile)
 {
     if( CreateIoCompletionPort(hFile,hIoComplete,0,0)==NULL)
        {
            return FALSE;
        }
        else{
            return TRUE;
        }
 }