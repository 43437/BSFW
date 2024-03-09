#ifndef __BS_OBJECT_H__
#define __BS_OBJECT_H__

#include "BSThread.h"
#include <type_traits>
#include <condition_variable>

namespace BSFW
{
	class BSThread;
	class BSFW_LIB_EXPORT BSObject
	{
	public:
		BSObject();
		virtual ~BSObject() = 0;

		std::thread::id GetTheadID() const;

	public:
		void MoveToThread(BSThread* pThread);

	private:
		std::thread::id m_objThreadID;
	};

	template<typename C, typename Func, typename... Args>
	bool InvokeMethod(C* obj, Func func, EInvokeMethodType eType, Args& ...args)
	{
		bool bRet = false;
		auto dstObjThreadID = obj->GetTheadID();

		if (std::this_thread::get_id() != dstObjThreadID) {
			if (eInvokeMethodType_Block == eType) {
				std::mutex SyncMutex;
				std::condition_variable SyncCond;

				auto funcWrapper = [&]() {
					(obj->*func)(args...);
					{
						std::unique_lock<std::mutex> lock(SyncMutex);
						SyncCond.notify_one();
					}
				};
				{
					std::unique_lock<std::mutex> lock(SyncMutex);
					bRet = BSThread::Invoke(dstObjThreadID, funcWrapper);
					SyncCond.wait(lock);
				}
			}
			else {
				auto funcWrapper = [=]() mutable {
					(obj->*func)(args...);
				};
				bRet = BSThread::Invoke(dstObjThreadID, funcWrapper);
			}
		}
		else {
			[&]() {
				(obj->*func)(args...);
			}();
			bRet = true;
		}
		return bRet;
	}

	template<typename C, typename Func, typename... Args>
	bool InvokeMethod(C* obj, Func func, Args& ...args)
	{
		return InvokeMethod(obj, func, BSFW::eInvokeMethodType_Auto, args...);
	}

	template<typename C, typename... Args>
	bool InvokeLambda(C* obj, VOID_FUNC_VOID func, EInvokeMethodType eType, Args& ...args)
	{
		bool bRet = false;
		auto dstObjThreadID = obj->GetTheadID();
		if (std::this_thread::get_id() != dstObjThreadID) {
			if (eInvokeMethodType_Block == eType) {
				std::mutex SyncMutex;
				std::condition_variable SyncCond;
				auto funcWrapper = [&]() {
					func(args...);
					{
						std::unique_lock<std::mutex> lock(SyncMutex);
						SyncCond.notify_one();
					}
				};
				{
					std::unique_lock<std::mutex> lock(SyncMutex);
					bRet = BSThread::Invoke(dstObjThreadID, funcWrapper);
					SyncCond.wait(lock);
				}
			}
			else {
				auto funcWrapper = [=]() mutable {
					func(args...);
				};
				bRet = BSThread::Invoke(dstObjThreadID, funcWrapper);
			}
		}
		else {
			[&]() {
				func(args...);
			}();
			bRet = true;
		}
		return bRet;
	}

	template<typename C, typename... Args>
	bool InvokeLambda(C* obj, VOID_FUNC_VOID func, Args& ...args)
	{
		return InvokeLambda(obj, func, BSFW::eInvokeMethodType_Auto, args...);
	}
};


#endif 
