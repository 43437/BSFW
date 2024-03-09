#ifndef __BS_MSG_BASE_H__
#define __BS_MSG_BASE_H__

#include "Export/BSMisc.h"
#include <unordered_map>
#include <mutex>

namespace BSFW
{
#define DECL_MSG_TYPE(T)									\
	protected:												\
		struct SRegType {									\
			SRegType()										\
			{												\
				BSFW::BSMsgHelper::Instance().RegistClass<T>();	\
			}												\
		};													\
	public:													\
		virtual void CopyFrom(BSMsgBase* oth)				\
		{													\
			const T* pOth = dynamic_cast<T*>(oth);			\
			if (nullptr != pOth)							\
			{												\
				*this = *pOth;								\
			}												\
		}													\
															\
		virtual std::string TypeName()						\
		{													\
			static SRegType objRegType;						\
			return typeid(T).name();						\
		}													

	class BSFW_LIB_EXPORT BSMsgBase;
	class BSFW_LIB_EXPORT BSMsgHelper
	{
	public:
		typedef std::function<BSMsgBase*()> fpConstructor;

		static BSMsgHelper& Instance();

		BSMsgBase* Reflect(const std::string& typeName);

		template<typename T>
		void RegistClass()
		{
			std::lock_guard<std::recursive_mutex> lock(m_objMutex);
			std::string key = typeid(T).name();
			m_mpConstruct[key] = []() {
				return new T();
			};
		}

		BSMsgHelper(const BSMsgHelper&) = delete;

	private:
		BSMsgHelper();

	private:
		std::unordered_map<std::string, fpConstructor> m_mpConstruct;
		std::recursive_mutex m_objMutex;
	};

	class BSFW_LIB_EXPORT BSMsgBase
	{
		DECL_MSG_TYPE(BSMsgBase)
	public:
		BSMsgBase();
		virtual ~BSMsgBase();
	};
};


#endif