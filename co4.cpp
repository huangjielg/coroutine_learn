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
//using namespace std::experimental;
using namespace std;
class function_depth
{
public:
	static int depth;
	function_depth()
	{
		depth++;
	}
	~function_depth()
	{
		depth--;
	}
};
int function_depth::depth = 0;
#define DUMP_FUNCTION()                                                                  \
	function_depth __fd;                                                                 \
	do                                                                                   \
	{                                                                                    \
		for (int i = 0; i < function_depth::depth; i++)                                  \
		{                                                                                \
			std::cout << '\t';                                                           \
		}                                                                                \
		std::cout << " Function:" << __FUNCTION__ << "  Line:" << __LINE__ << std::endl; \
	} while (0)
int promise_alloc = 0;
std::coroutine_handle<> hYield;
template <class T>
struct task
{

	struct promise_type
	{
		T value;
		std::coroutine_handle<> awaitingCoroutine;
		task get_return_object()
		{
			DUMP_FUNCTION();
			return {*this};
		}
		std::suspend_never initial_suspend()
		{
			DUMP_FUNCTION();
			return {};
		}
		auto final_suspend() noexcept  
		{
			DUMP_FUNCTION();
			struct final_awaiter
			{
				std::coroutine_handle<> awaitingCoroutine;
				bool await_ready()
				{
					DUMP_FUNCTION();
					return false;
				}
				void await_resume() { DUMP_FUNCTION(); }
				final_awaiter(std::coroutine_handle<> h) : awaitingCoroutine(h) {}
				std::coroutine_handle<> await_suspend(std::coroutine_handle<> h)
				{
					DUMP_FUNCTION();
					return awaitingCoroutine ? awaitingCoroutine : std::noop_coroutine();
				}
			};
			//DUMP_FUNCTION();
			return final_awaiter(this->awaitingCoroutine);
		}
		void return_value(T v)
		{
			DUMP_FUNCTION();
			value = std::move(v);
		}
		auto yield_value(T v)
		{
			hYield = std::coroutine_handle<promise_type>::from_promise(*this);
			DUMP_FUNCTION();
			value = std::move(v);
			return std::suspend_always{};
		}
		void unhandled_exception()
		{
			DUMP_FUNCTION();
		}
		promise_type() : value(0) {}
		~promise_type()
		{
			DUMP_FUNCTION();
		}
		void *operator new(size_t sz)
		{
			// sz is NOT equal to sizeof(promise_type)
			// but size of all state objects of the coroutine (include promise )
			DUMP_FUNCTION();
			std::cout<<"operator new for promise:"<<sz<<std::endl;
			promise_alloc++;
			return malloc(sz);
		}
		void operator delete(void *p)
		{
			DUMP_FUNCTION();
			promise_alloc--;
			free(p);
		}
	};

	auto operator co_await()
	{
		DUMP_FUNCTION();
		struct task_awaiter
		{
			bool await_ready() const noexcept
			{
				DUMP_FUNCTION();
				bool b = std::coroutine_handle<promise_type>::from_promise(m_promise).done();
				std::cout << "return " << b << endl;
				return b;
			}
			bool await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
			{
				DUMP_FUNCTION();
				m_promise.awaitingCoroutine = awaitingCoroutine;
				return true;
			}
			T &await_resume() noexcept
			{
				DUMP_FUNCTION();
				return m_promise.value;
			}
			task_awaiter(promise_type &p) : m_promise(p)
			{
				DUMP_FUNCTION();
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
		DUMP_FUNCTION();
		std::cout << "this:" << this << std::endl;
	}
	~task()
	{
		DUMP_FUNCTION();
		std::cout << "this:" << this << std::endl;
		std::coroutine_handle<promise_type>::from_promise(m_promise).destroy();
	}
	promise_type &m_promise;
	std::coroutine_handle<> awaitingCoroutine;
};

task<int> co1(int a)
{

	// Promise p = new Promise();
	// R = p.get_return_object() ;
	// co_await p.initial_suspend();
	//try {
		//for (int i = 0; i < 3; i++) {
			DUMP_FUNCTION();
			co_yield a * 2;
			co_return a*3;
		//}
	//co_return 4;
	//}
	//catch(...)
	//{
	// p.unhandled_exception();
	//}
	// final_suspend:
	// co_await p.final_suspend();
}

task<int> co2(int a)
{
	DUMP_FUNCTION();
	int r = co_await co1(a + 1);
	//DUMP_FUNCTION();
	std::cout << "co_await return " << r << endl;
	co_return r + 1;
}

int main(int, char **)
{
	DUMP_FUNCTION();
	if (0)
	{
		task<int> f1 = co1(13);
		std::cout << "F1 value " << f1.get_value() << std::endl;
	}
	if (1)
	{
		auto f2 = co2(3);
		std::cout << " resume waited task " << endl;
		hYield.resume();
		std::cout << " result " << f2.get_value() << endl;
	}
	//DUMP_FUNCTION();
	std::cout << "promise alloc " << promise_alloc << endl;
	return 0;
}