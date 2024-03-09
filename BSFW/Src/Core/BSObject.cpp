#include "Export/BSObject.h"
#include <iostream>

namespace BSFW
{
	BSObject::BSObject()
	{
		m_objThreadID = BSThread::GetDefaultThreadID();
	}

	BSObject::~BSObject()
	{}

	std::thread::id BSObject::GetTheadID() const
	{
		return m_objThreadID;
	}

	void BSObject::MoveToThread(BSThread* pThread)
	{
		if (nullptr != pThread)
		{
			m_objThreadID = pThread->GetTheadID();
		}
	}
};