#include "Export/BSTimer.h"
#include "Export/BSThread.h"
#include "Export/BSObject.h"
#include <chrono>
#include <set>
#include <mutex>

namespace BSFW
{
	class BSTimerHelper : public BSThread
	{
	public:
		static BSTimerHelper& Instance()
		{
			static BSTimerHelper __instance;
			return __instance;
		}

		virtual ~BSTimerHelper()
		{
			Stop();
		}

		void RegistTimer(BSTimer* pTimer)
		{
			InvokeLambda(this, [=]() {
				m_stTimer.insert(pTimer);
			});
		}

		void UnRegistTimer(BSTimer* pTimer)
		{
			InvokeLambda(this, [=]() {
				m_stTimer.erase(pTimer);
			});
		}

		void CheckTimeout()
		{
			uint64_t curTime = BSTimer::CurrMSSinceEpoch();

			bool bStopTimer = false;
			for (auto itTimer = m_stTimer.begin(); m_stTimer.end() != itTimer; )
			{
				bStopTimer = false;
				auto pTimer = *itTimer;
				if (pTimer->IsEnabled())
				{
					if (pTimer->m_uiNextShotTime <= curTime)
					{
						//trigger here
						pTimer->OnTimer();

						pTimer->m_uiNextShotTime = curTime + pTimer->m_uiInterval;

						if (pTimer->IsSingleShot())
						{
							//stop it
							pTimer->m_bEnabled = false;
							bStopTimer = true;
						}
					}
				}

				if (!bStopTimer)
				{
					++itTimer;
				}
				else
				{
					itTimer = m_stTimer.erase(itTimer);
				}
			}
		}

	protected:
		virtual void Run() override
		{
			while (m_bRunLoop)
			{
				HandleMessage();

				std::this_thread::sleep_for(std::chrono::milliseconds(1));

				CheckTimeout();
			}
		}

	private:
		BSTimerHelper() : BSThread()
		{
			Start();
		}

	private:
		std::set< BSTimer*> m_stTimer;
	};

	BSTimer::BSTimer() : m_bEnabled(false)
		, m_bSingleShot(false)
	{
	}

	BSTimer::~BSTimer()
	{
		Stop();
	}

	void BSTimer::OnTimer()
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		for (auto itObserver = m_mpObserver.begin(); m_mpObserver.end() != itObserver; ++itObserver)
		{
			auto pObserver = itObserver->first;
			auto func = itObserver->second;
			InvokeLambda(pObserver, func);
		}
	}

	void BSTimer::Subscribe(BSObject* obj, VOID_FUNC_VOID func)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		m_mpObserver[obj] = func;
	}

	void BSTimer::UnSubscribe(BSObject* obj)
	{
		std::lock_guard<std::recursive_mutex> lock(m_mutex);
		m_mpObserver.erase(obj);
	}

	uint64_t BSTimer::CurrMSSinceEpoch()
	{
		using namespace std::chrono;
		return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}

	bool BSTimer::IsEnabled() const
	{
		return m_bEnabled;
	}

	bool BSTimer::IsSingleShot() const
	{
		return m_bSingleShot;
	}

	void BSTimer::Start(const uint64_t ms, bool bSingleShot/* = false*/)
	{
		if (ms != 0)
		{
			m_bEnabled = true;
			m_bSingleShot = bSingleShot;

			m_uiInterval = ms;
			m_uiNextShotTime = CurrMSSinceEpoch() + m_uiInterval;

			BSTimerHelper::Instance().RegistTimer(this);
		}
	}

	void BSTimer::Stop()
	{
		m_bEnabled = false;
		BSTimerHelper::Instance().UnRegistTimer(this);
	}
};