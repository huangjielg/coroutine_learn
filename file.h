#ifndef _FILE_H_
#define _FILE_H_
#include <windows.h>
namespace cpp_test
{
    template <class T>
    class File
    {
        HANDLE hFile;
        File(HANDLE h):hFile(h){

        }
        public :
            static task<File> CreateFile(
                                     LPCSTR                 lpFileName,
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
                    [](){
                        hFile = ::CreateFile(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
                        errorCode= GetLastError();                          
                    }
                );
                co_return File(hFile);
            }
    };
}
#endif