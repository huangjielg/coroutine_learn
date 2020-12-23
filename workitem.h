#ifndef _WORK_ITEM_H
#define _WORK_ITEM_H
#include <functional>
#include "sched.h"
namespace cpp_test
{
    class WorkItem
    {
        std::function<void()> m_f;
        std::coroutine_handle<> waiter;
        bool bdone;

    protected:
        void Action()
        {
            m_f();
            Scheduler::Queue([this]() {
                bdone = true;
                if (waiter)
                {
                    waiter.resume();
                }
            });
        }
        static DWORD WINAPI ThreadProc(
            LPVOID lpParameter)
        {
            WorkItem *pThis = (WorkItem *)lpParameter;
            pThis->Action();
            return 0;
        }

    public:
        WorkItem(std::function<void()> f) : m_f(std::move(f)), bdone(false)
        {
            std::cout << "WorkItem::WorkItem" << std::endl;
            ::QueueUserWorkItem(ThreadProc, this, 0);
        }
        ~WorkItem()
        {
            std::cout << "WorkItem::~WorkItem" << std::endl;
        }

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
    };
} // namespace cpp_test
#endif