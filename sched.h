#ifndef _SCHED_H_
#define _SCHED_H_
namespace cpp_test
{
    struct IoCompleteLamda{
        public :
             OVERLAPPED o;
             std::function<void()> m_f;
             IoCompleteLamda(std::function<void()>f):m_f(f){
        }
    };
    class Scheduler
    {
        static HANDLE hIoComplete;
        public:
            static INT   Init();
            static VOID  DeInit();
            static VOID  Queue(std::function<void()> f);
            static INT   ProcessOneEvent(DWORD   dwMilliseconds=INFINITE);
    };
}
#endif 