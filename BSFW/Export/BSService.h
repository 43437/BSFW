#ifndef __BS_SERVICE_H__
#define __BS_SERVICE_H__

#include "BSObject.h"
#include "BSMsgBase.h"
#include <unordered_map>

namespace BSFW
{
	class BSThread;
	class BSServiceHelper;

#define BEGIN_BIND				\
protected:						\
	virtual void BindTrigger(){
		
#define END_BIND	\
	}
#define BIND_TRIGGER(METHOD_ID, METHOD)	\
	m_mpHandler[METHOD_ID] = (BSService::fpHandler)METHOD

#define REGIST_SRV(SRV_NAME, SRV_TYPE)			\
	struct REGIST_SRV_##SRV_NAME##SRV_TYPE{	\
		REGIST_SRV_##SRV_NAME##SRV_TYPE(){		\
			BSFW::BSServiceHelper::Instance().RegistService<SRV_TYPE>(SRV_NAME);		\
		};					\
	}objREGIST_SRV_##SRV_NAME_##SRV_TYPE;

	class BSFW_LIB_EXPORT BSService : public BSObject
	{
	public:
		typedef void (BSService::*fpHandler)(BSMsgBase* pMsgBase);
	public:
		BSService(const std::string& srvName);
		virtual ~BSService() = 0;

		std::string GetSrvName() const;

		void OnTrigger(const std::string& id, BSMsgBase* pMsgBase, bool CopyParam);

		BEGIN_BIND
		END_BIND

	private:
		void Start();

	protected:
		BSThread* m_pThread;

	protected:
		const std::string m_SrvName;
		std::unordered_map<std::string, BSService::fpHandler> m_mpHandler;
	private:
		friend class BSServiceHelper;
	};

	class BSFW_LIB_EXPORT BSServiceHelper : public BSObject
	{
	public:
		static BSServiceHelper& Instance();

		template<typename T>
		bool RegistService(const std::string& strSrvName)
		{
			bool bRet = true;
			InvokeLambda(this, [&]() {
				if (m_mpSrv.count(strSrvName) == 0)
				{
					BSService* pSrv = new T(strSrvName);
					m_mpSrv[strSrvName] = pSrv;
					pSrv->Start();
					bRet = true;
				}
				else
				{
					bRet = false;
				}
			}, eInvokeMethodType_Block);

			return bRet;
		}

		bool Trigger(const std::string& strSrvName, const std::string& method, BSMsgBase* pMsgBase, const EInvokeMethodType eType);

	private:
		BSServiceHelper();

	private:
		std::unordered_map<std::string, BSService*> m_mpSrv;
	};

	bool BSFW_LIB_EXPORT FireEvent(const std::string& srvName, const std::string& id, BSMsgBase* pMsgBase, const EInvokeMethodType eType = eInvokeMethodType_Auto);
};

#endif