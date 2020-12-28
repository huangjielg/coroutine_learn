#include "pch.h"
#include "sched.h"
#include "task.h"
#include "workitem.h"
#include "file.h"

using namespace cpp_test;

task<int> co1(int a)
{
    co_return a * 3;
}

task<int> co2(int a)
{
    int r = co_await co1(a + 1);
    std::cout << "co_await return " << r << std::endl;
    co_return r + 1;
}
#if 1
task<int> ff(int a)
{

    co_await WorkItem(
        [&a]() {
            a = a * 3;
            return;
        });
    co_return a;
}
#endif

task<int> f_file(LPCTSTR lpPath){
    File  f = co_await File::CreateFile(lpPath,GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_ALWAYS,0,INVALID_HANDLE_VALUE);
    std::wcout << "Create File " << lpPath << " Handle=" << f.Handle() << std::endl;
    char buf[16];
    for(int i=0;i<16;i++)
    {
        buf[i]='a'+i;
    }
    for(int n=0;n<4;n++){
        int nWrite = co_await f.WriteFile(0,buf,16);
        std::wcout<<lpPath << " Write out " << nWrite <<" bytes" << std::endl;
        int nRead = co_await f.ReadFile(0,buf,16);
        std::wcout<<lpPath  << " nRead out " << nRead <<" bytes" << std::endl;
    }
    co_return 0;
}

int main(int argc, char *argv[])
{
    Scheduler::Init();
    // auto f2=co2(3);
    // std::cout << "f2 return  " << f2.get_value() << std::endl;
    if (0)
    {
        int a = 3;
        auto f1 = ff(a);
        Scheduler::ProcessOneEvent();
        std::cout << "done result " << f1.get_value() << " a= " << a << std::endl;
    }
    if(1){
        auto f1=f_file(_T("h:\\a.txt"));
        auto f2=f_file(_T("h:\\b.txt"));
        for(int i=0;!(f1.done()&&f2.done());i++){
           // std::cout <<"---------ProcessOneEvent----------:" << i << std::endl;
            Scheduler::ProcessOneEvent();
        }
    }
}