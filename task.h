#ifndef _TASK_H_
#define _TASK_H_
namespace cpp_test
{
    template <class T>
    struct task
    {

        struct promise_type
        {
            T value;
            std::coroutine_handle<> awaitingCoroutine;
            task get_return_object()
            {
                return {*this};
            }
            std::suspend_never initial_suspend()
            {

                return {};
            }
            auto final_suspend() noexcept
            {

                struct final_awaiter
                {
                    std::coroutine_handle<> awaitingCoroutine;
                    bool await_ready()
                    {

                        return false;
                    }
                    void await_resume() {}
                    final_awaiter(std::coroutine_handle<> h) : awaitingCoroutine(h) {}
                    std::coroutine_handle<> await_suspend(std::coroutine_handle<> h)
                    {

                        return awaitingCoroutine ? awaitingCoroutine : std::noop_coroutine();
                    }
                };
                //
                return final_awaiter(this->awaitingCoroutine);
            }
            void return_value(T v)
            {

                value = std::move(v);
            }
            auto yield_value(T v)
            {
                //hYield = std::coroutine_handle<promise_type>::from_promise(*this);

                value = std::move(v);
                return std::suspend_always{};
            }
            void unhandled_exception()
            {
            }
            promise_type() : value(0) {}
            ~promise_type()
            {
            }
            void *operator new(size_t sz)
            {
                // sz is NOT equal to sizeof(promise_type)
                // but size of all state objects of the coroutine (include promise )

                //std::cout<<"operator new for promise:"<<sz<<std::endl;
                //promise_alloc++;
                return malloc(sz);
            }
            void operator delete(void *p)
            {

                //	promise_alloc--;
                free(p);
            }
        };

        auto operator co_await()
        {

            struct task_awaiter
            {
                bool await_ready() const noexcept
                {

                    bool b = std::coroutine_handle<promise_type>::from_promise(m_promise).done();
                    //	std::cout << "return " << b << endl;
                    return b;
                }
                bool await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
                {

                    m_promise.awaitingCoroutine = awaitingCoroutine;
                    return true;
                }
                T &await_resume() noexcept
                {

                    return m_promise.value;
                }
                task_awaiter(promise_type &p) : m_promise(p)
                {
                }
                promise_type &m_promise;
            };
            return task_awaiter(m_promise);
        }
        T &get_value()
        {
            return m_promise.value;
        }
        task(promise_type &p) : m_promise(p)
        {

            //std::cout << "this:" << this << std::endl;
        }
        ~task()
        {

            //std::cout << "this:" << this << std::endl;
            std::coroutine_handle<promise_type>::from_promise(m_promise).destroy();
        }
        promise_type &m_promise;
        std::coroutine_handle<> awaitingCoroutine;
    };
} // namespace cpp_test
#endif