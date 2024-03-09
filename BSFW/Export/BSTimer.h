#ifndef __BS_TIMER_H__
#define __BS_TIMER_H__

#include <stdint.h>
#include <unordered_map>
#include "Export/BSThread.h"
#include "Export/BSMisc.h"

namespace BSFW
{
	class BSObject;
	class BSFW_LIB_EXPORT BSTimer
	{
	public:
		BSTimer();
		virtual ~BSTimer();

		void Start(const uint64_t ms, bool bSingleShot = false);
		void Stop();
		static uint64_t CurrMSSinceEpoch();

		bool IsEnabled() const;
		bool IsSingleShot() const;

		void Subscribe(BSObject* obj, VOID_FUNC_VOID func);
		void UnSubscribe(BSObject* obj);

	private:
		void OnTimer();

	private:
		bool m_bEnabled;
		bool m_bSingleShot;
		uint64_t m_uiInterval;
		uint64_t m_uiNextShotTime;
		friend class BSTimerHelper;
		std::unordered_map< BSObject*, VOID_FUNC_VOID> m_mpObserver;
		std::recursive_mutex m_mutex;
	};
};

#endif // !__BS_TIMER_H__




