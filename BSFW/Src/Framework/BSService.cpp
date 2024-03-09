#include "Export/BSService.h"
#include <unordered_map>

namespace BSFW
{
	BSServiceHelper& BSServiceHelper::Instance()
	{
		static BSServiceHelper __instance;
		return __instance;
	}

		
	bool BSServiceHelper::Trigger(const std::string& strSrvName, const std::string& method, BSMsgBase* pMsgBase, const EInvokeMethodType eType)
	{
		BSService* pSrv = nullptr;

		bool bRet = true;
		InvokeLambda(this, [&]() {
			if (m_mpSrv.count(strSrvName) > 0)
			{
				pSrv = m_mpSrv[strSrvName];
				bRet = true;
			}
			else
			{
				bRet = false;
			}
		}, eInvokeMethodType_Block);

		if (bRet)
		{
			BSMsgBase* pMsgSend = nullptr;
			bool bCopyParam = false;
			if (eInvokeMethodType_Block == eType
				|| BSThread::CurrThreadID() == pSrv->GetTheadID())
			{
				pMsgSend = pMsgBase;
				bCopyParam = false;
			}
			else
			{
				pMsgSend = BSMsgHelper::Instance().Reflect(pMsgBase->TypeName());
				if (nullptr != pMsgSend)
				{
					pMsgSend->CopyFrom(pMsgBase);
					bCopyParam = true;
				}
			}
			bRet = InvokeMethod(pSrv, &BSService::OnTrigger, eType, method, pMsgSend, bCopyParam);
		}

		return bRet;
	}

	BSServiceHelper::BSServiceHelper() {};


	BSService::BSService(const std::string& srvName) : BSObject()
		, m_SrvName(srvName)
	{
		m_pThread = new BSThread;
		m_pThread->Start();

		MoveToThread(m_pThread);
	}

	BSService::~BSService()
	{

	}

	std::string BSService::GetSrvName() const
	{
		return m_SrvName;
	}

	void BSService::OnTrigger(const std::string& id, BSMsgBase* pMsgBase, bool CopyParam)
	{
		if (m_mpHandler.count(id) > 0)
		{
			auto fun = m_mpHandler[id];
			(this->*fun)(pMsgBase);
		}

		if (CopyParam 
			&& nullptr != pMsgBase)
		{
			delete pMsgBase;
		}
	}

	void BSService::Start()
	{
		BindTrigger();
	}

	bool FireEvent(const std::string& srvName, const std::string& id, BSMsgBase* pMsgBase, const EInvokeMethodType eType /*= eInvokeMethodType_Auto*/)
	{
		return BSServiceHelper::Instance().Trigger(srvName, id, pMsgBase, eType);
	}
};