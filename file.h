#ifndef _FILE_H_
#define _FILE_H_
#include "sched.h"
namespace cpp_test
{
  
    class File
    {
        HANDLE hFile;
        File(HANDLE h):hFile(h){

        }
        public :
            File():hFile(INVALID_HANDLE_VALUE)
            {
            
            }
            ~File()
            {
                if(hFile!=INVALID_HANDLE_VALUE){
                    CloseHandle(hFile);
                }
            }
            File (File && f){
                hFile = f.hFile;
                f.hFile=INVALID_HANDLE_VALUE;
            }
            //File (const File& f){
            //    hFile = f.hFile;
            //    f.hFile=INVALID_HANDLE_VALUE;
           // }

            File & operator = (File && f){
                hFile = f.hFile;
                f.hFile=INVALID_HANDLE_VALUE;
                return *this;
            }
            HANDLE Handle() {
                return hFile;
            }
            static task<File> CreateFile(
                                     LPCTSTR                 lpFileName,
                                     DWORD                 dwDesiredAccess,
                                     DWORD                 dwShareMode,
                                     LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                     DWORD                 dwCreationDisposition,
                                     DWORD                 dwFlagsAndAttributes,
                                     HANDLE                hTemplateFile)
            {
                HANDLE hFile;
                DWORD  errorCode;
                co_await WorkItem(
                    [=,&hFile,&errorCode](){
                        hFile = ::CreateFile(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes|FILE_FLAG_OVERLAPPED,hTemplateFile);
                        if(hFile!=INVALID_HANDLE_VALUE){
                            Scheduler::BindFile(hFile);
                        }
                        errorCode= GetLastError();                          
                    }
                );
                co_return File(hFile);
            }
             task<DWORD> WriteFile(INT64 offset,LPVOID lpBuffer,int toWrite){

                DWORD nWrite;
                IoCompleteLamda lo ([&nWrite](IoCompleteLamda &thelo){
                    if( ::GetOverlappedResult(INVALID_HANDLE_VALUE,&thelo.o,&nWrite,FALSE))
                    {
                        
                    }else{
                        // operation failed;

                    }
                });
                if(::WriteFile(hFile,lpBuffer,toWrite,&nWrite,&lo.o))
                {
                    std::cout << "File Write Succeeded " << std::endl;
                 }else{

                    if(GetLastError()==ERROR_IO_PENDING){
                        co_await lo;
                    }
                }
                co_return nWrite;
            }
            task<DWORD> ReadFile( INT64 offset,LPVOID lpBuffer,int toWrite){
               
                DWORD nRead;
                IoCompleteLamda lo ([&nRead](IoCompleteLamda&thelo){
                    if( ::GetOverlappedResult(INVALID_HANDLE_VALUE,&thelo.o,&nRead,FALSE))
                    {
                        
                    }else{
                        // operation failed;

                    }
                });
                if(::ReadFile(hFile,lpBuffer,toWrite,&nRead,&lo.o))
                {
                    
                }else{
                    co_await lo;
                }
                co_return nRead;
            }
    };
}
#endif