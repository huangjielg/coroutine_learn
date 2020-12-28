#ifndef _SCHED_H_
#define _SCHED_H_

namespace cpp_test
{
    struct IoCompleteLamda
    {
    public:
        OVERLAPPED o;
        std::function<void(IoCompleteLamda &)> m_f;
        std::coroutine_handle<> waiter;
        IoCompleteLamda(std::function<void(IoCompleteLamda &)> f) : m_f(f),waiter(nullptr)
        {
           // std::cout<<"IoCompleteLamda at:" << this<<std::endl;
            o.hEvent = NULL;
            o.Internal = 0;
            o.InternalHigh = 0;
            o.Offset = 0;
            o.OffsetHigh = 0;
        }
        void do_callback(){
          //  std::cout<<"IoCompleteLamda do_callback at:" << this<<std::endl;
            m_f(*this);
            if(waiter)
                waiter.resume();
        }

        //struct awaiter
        //{
            bool await_ready() const noexcept
            {
                return false;
            }
            bool await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
            {
                //  std::cout<<"IoCompleteLamda do_callback await_suspend : " << awaitingCoroutine.address() <<" on :" <<this <<std::endl;
                waiter = awaitingCoroutine;
                return true;
            }
            void await_resume() noexcept
            {
            }
            //awaiter(IoCompleteLamda &i) : lo(i)
           // {
            //}
            //IoCompleteLamda &lo;
        //};
     };
    class Scheduler
    {
        static HANDLE hIoComplete;

    public:
        static INT Init();
        static VOID DeInit();
        static VOID Queue(IoCompleteLamda &lo);
        static INT ProcessOneEvent(DWORD dwMilliseconds = INFINITE);
        static BOOL BindFile(HANDLE hFile);
    };
} // namespace cpp_test
#endif