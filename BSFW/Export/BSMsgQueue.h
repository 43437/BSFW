#ifndef __BS_MSG_QUEUE_H__
#define __BS_MSG_QUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>

namespace BSFW
{
	// A threadsafe-queue.
	template <class T>
	class BSMsgQueue
	{
	public:
		BSMsgQueue(void)
			: q()
			, m()
			, c()
		{}

		~BSMsgQueue(void)
		{}

		// Add an element to the queue.
		void EnQueue(T t)
		{
			std::lock_guard<std::mutex> lock(m);
			q.push(t);
			c.notify_one();
		}

		// Get the "front"-element.
		// If the queue is empty, wait till a element is avaiable.
		T DeQueue(void)
		{
			std::unique_lock<std::mutex> lock(m);
			// release lock as long as the wait and reaquire it afterwards.
			//while (q.empty())
			//{
				c.wait(lock, [this]() {return !q.empty(); });
			//}
			T val = q.front();
			q.pop();

			return val;
		}

		bool DeQueue(T& val)
		{
			bool bRet = false;
			std::lock_guard<std::mutex> lock(m);
			if (!q.empty())
			{
				val = q.front();
				q.pop();
				bRet = true;
			}

			return bRet;
		}

	private:
		std::queue<T> q;
		mutable std::mutex m;
		std::condition_variable c;
	};
};
#endif

