#ifndef __BS_THREAD_H__
#define __BS_THREAD_H__

#include <functional>
#include <thread>
#include <mutex>
#include "Export/BSMisc.h"

namespace BSFW
{
	template <class T>
	class BSMsgQueue;

	enum EInvokeMethodType
	{
		eInvokeMethodType_From = 0,
		eInvokeMethodType_Auto,
		eInvokeMethodType_Block,
		eInvokeMethodType_NoBlock,
		eInvokeMethodType_To,
	};

	class BSFW_LIB_EXPORT BSThread
	{
	public:
		BSThread();
		virtual ~BSThread();

	public:
		std::thread::id GetTheadID() const;

		void Invoke(VOID_FUNC_VOID func);

		static BSThread* Thread(const std::thread::id& objThID);
		static std::thread::id GetDefaultThreadID();
		static bool Invoke(const std::thread::id& objThID, VOID_FUNC_VOID func);
		static std::thread::id CurrThreadID();

		void Start();
		virtual void Stop();

	protected:
		virtual void Run();
		void LoopMessage();
		void HandleMessage();
		void ThreadFunc();

	protected:
		BSMsgQueue<VOID_FUNC_VOID>* m_pMsgQueue;
		std::thread* m_pThread;
		bool m_bRunLoop;
		bool m_bStarted;
	};
};

//#define INVOKE_METHOD(obj, FUNC, INVOKE_TYPE, ...)				\
//	do {														\
//		decltype(##FUNC) func=##FUNC;							\
//																\
//		auto dstObjThreadID = obj->GetTheadID();				\
//		if (std::this_thread::get_id() != dstObjThreadID){		\
//			if (eInvokeMethodType_QueueBlock == INVOKE_TYPE){	\
//				std::mutex SyncMutex;							\
//				std::condition_variable SyncCond;				\
//				auto funcWrapper = [&]() {								\
//					(obj->*func)(##__VA_ARGS__);				\
//					{											\
//						std::unique_lock<std::mutex> lock(SyncMutex);		\
//						SyncCond.notify_one();					\
//					}											\
//				};												\
//				{												\
//					std::unique_lock<std::mutex> lock(SyncMutex);			\
//					BSThread::Invoke(dstObjThreadID, funcWrapper);		\
//					SyncCond.wait(lock);						\
//				}												\
//			}else {												\
//				auto funcWrapper = [=]() mutable{								\
//					(obj->*func)(##__VA_ARGS__);				\
//				};												\
//				BSThread::Invoke(dstObjThreadID, funcWrapper);			\
//			}													\
//		} else {												\
//			[&]() {												\
//				(obj->*func)(##__VA_ARGS__);					\
//			}();												\
//		}														\
//	}while(0);

#endif
