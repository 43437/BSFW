#include "Export/BSMsgBase.h"

namespace BSFW
{
	BSMsgHelper::BSMsgHelper()
	{}

	BSMsgHelper& BSMsgHelper::Instance()
	{
		static BSMsgHelper __instance;
		return  __instance;
	}

	BSMsgBase* BSMsgHelper::Reflect(const std::string& typeName)
	{
		BSMsgBase* ret = nullptr;
		std::lock_guard<std::recursive_mutex> lock(m_objMutex);
		if (m_mpConstruct.count(typeName) > 0)
		{
			ret = m_mpConstruct[typeName]();
		}
		return ret;
	}

	BSMsgBase::BSMsgBase()
	{}

	BSMsgBase::~BSMsgBase()
	{}
};