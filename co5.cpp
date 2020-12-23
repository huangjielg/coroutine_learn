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

#include "task.h"
#include "file.h"
#include "workitem.h"
using namespace cpp_test;

task<int> co1(int a)
{
    co_return a*3;
}

task<int> co2(int a)
{
	int r = co_await co1(a + 1);
	std::cout << "co_await return " << r << std::endl;
	co_return r + 1;
}
#if 1
task<int> ff( int a)
{
  
   co_await WorkItem(
        [&a](){
            a=a*3;
            return ;
        });
    co_return a;
}
#endif
int main(int argc,char*argv[])
{
    Scheduler::Init();
   // auto f2=co2(3);
   // std::cout << "f2 return  " << f2.get_value() << std::endl;
    int a=3;
    auto f1=ff(a);
    Scheduler::ProcessOneEvent();
    std::cout << "done result "<<f1.get_value() <<" a= " <<a << std::endl;
}