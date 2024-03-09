#include "Export/BSThread.h"
#include "Export/BSMsgQueue.h"
#include "Export/BSObject.h"
#include <unordered_map>

namespace BSFW
{
	class BSThreadHelper
	{
	public:
		static BSThreadHelper& Instance()
		{
			static BSThreadHelper __instance;
			return __instance;
		};

		void RegistThread(BSThread* pThd)
		{
			std::lock_guard<std::recursive_mutex> lock(m_objMutex);
			m_hsThread[pThd->GetTheadID()] = pThd;
		}

		BSThread* Thread(const std::thread::id& objThID)
		{
			BSThread* ret = nullptr;

			std::lock_guard<std::recursive_mutex> lock(m_objMutex);
			if (m_hsThread.count(objThID) > 0)
			{
				ret = m_hsThread[objThID];
			}

			return ret;
		}

		std::thread::id GetDefaultThreadID()
		{
			if (nullptr == m_pDefaultThd)
			{
				std::lock_guard<std::recursive_mutex> lock(m_objMutex);
				if (nullptr == m_pDefaultThd)
				{
					m_pDefaultThd = new BSThread();
					m_pDefaultThd->Start();
				}
			}
			return m_pDefaultThd->GetTheadID();
		}

		bool Invoke(std::thread::id objThdID, VOID_FUNC_VOID func)
		{
			{
				std::lock_guard<std::recursive_mutex> lock(m_objMutex);
				if (m_hsThread.count(objThdID) > 0)
				{
					auto pThd = m_hsThread[objThdID];
					pThd->Invoke(func);
				}
				else
				{
					return false;
				}
			};

			return true;
		}

		~BSThreadHelper()
		{
			std::lock_guard<std::recursive_mutex> lock(m_objMutex);
			for (auto itThread = m_hsThread.begin(); m_hsThread.end() != itThread; )
			{
				auto pThread = itThread->second;
				itThread = m_hsThread.erase(itThread);
				delete pThread;
			}
		}

	private:
		BSThreadHelper() : m_pDefaultThd(nullptr)
		{
			//主线程ID
			m_objMainThreadID = std::this_thread::get_id();
		}

		BSThreadHelper(BSThreadHelper&) = delete;

	private:
		//线程和线程对象映射
		std::unordered_map<std::thread::id, BSThread*> m_hsThread;
		//默认线程
		BSThread* m_pDefaultThd;
		//主线程ID
		std::thread::id m_objMainThreadID;
		mutable std::recursive_mutex m_objMutex;
	};


	BSThread::BSThread() : m_pMsgQueue(new BSMsgQueue<VOID_FUNC_VOID>())
		, m_bRunLoop(true)
		, m_bStarted(false)
	{
		m_pThread = new std::thread(&BSThread::ThreadFunc, this);

		BSThreadHelper::Instance().RegistThread(this);
	}

	BSThread::~BSThread()
	{
		m_pThread->join();
		delete m_pThread;
	}

	std::thread::id BSThread::GetTheadID() const
	{
		return m_pThread->get_id();
	}

	BSThread* BSThread::Thread(const std::thread::id& objThID)
	{
		return BSThreadHelper::Instance().Thread(objThID);
	}

	std::thread::id BSThread::GetDefaultThreadID()
	{
		return BSThreadHelper::Instance().GetDefaultThreadID();
	}

	std::thread::id BSThread::CurrThreadID()
	{
		return std::this_thread::get_id();
	};

	bool BSThread::Invoke(const std::thread::id& objThID, VOID_FUNC_VOID func)
	{
		return BSThreadHelper::Instance().Invoke(objThID, func);
	}

	void BSThread::Invoke(VOID_FUNC_VOID func)
	{
		m_pMsgQueue->EnQueue(func);
	}

	void BSThread::Start()
	{
		m_pMsgQueue->EnQueue([this]() {
			m_bStarted = false;

			Run();
		});
	}

	void BSThread::Stop()
	{
		BSFW::InvokeLambda(this, [this]() {
			m_bRunLoop = false;
		});
	}

	void BSThread::Run()
	{
		while (m_bRunLoop)
		{
			LoopMessage();
		}
	}

	void BSThread::LoopMessage()
	{
		auto func = m_pMsgQueue->DeQueue();
		if (nullptr != func)
		{
			func();
		}
	}

	void BSThread::HandleMessage()
	{
		VOID_FUNC_VOID func = nullptr;
		if (m_pMsgQueue->DeQueue(func))
		{
			if (nullptr != func)
			{
				func();
			}
		}
	}

	void BSThread::ThreadFunc()
	{
		auto func = m_pMsgQueue->DeQueue();
		if (nullptr != func)
		{
			func();
		}
	}
};