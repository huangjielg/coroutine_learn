#ifndef _WORK_ITEM_H
#define _WORK_ITEM_H


namespace cpp_test
{
    class WorkItem
    {
        std::function<void()> m_f;
     
        IoCompleteLamda la;
    protected:
        void Action()
        {
            m_f();
            Scheduler::Queue(la);
        }
        static DWORD WINAPI ThreadProc(
            LPVOID lpParameter)
        {
            WorkItem *pThis = (WorkItem *)lpParameter;
            pThis->Action();
            return 0;
        }

    public:
        WorkItem(std::function<void()> f) : m_f(std::move(f)),
         //bdone(false),
         la([this](IoCompleteLamda&){
    
         })

        {
       //     std::cout << "WorkItem::WorkItem" << std::endl;
            if(!::QueueUserWorkItem(ThreadProc, this, 0)){
                throw std::system_error(std::error_code(GetLastError(),std::system_category()));
            }
        }
        ~WorkItem()
        {
         //   std::cout << "WorkItem::~WorkItem" << std::endl;
        }
#if 0
        auto operator co_await()
        {

            struct WorkItem_awaiter
            {
                bool await_ready() const noexcept
                {
                    return item.bdone;
                }
                bool await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
                {
                    item.waiter = awaitingCoroutine;
                    return true;
                }
                void await_resume() noexcept
                {
                }
                WorkItem_awaiter(WorkItem &i) : item(i)
                {
                }
                WorkItem &item;
            };
            return WorkItem_awaiter(*this);
        }
#endif 
          IoCompleteLamda & operator co_await() {
              return  la;
          }  
    };
} // namespace cpp_test
#endif