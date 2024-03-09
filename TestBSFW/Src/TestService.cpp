#include "TestService.h"
#include <iostream>

REGIST_SRV(TEST_SRV, TestService);

TestService::TestService(const std::string& srvName) : BSFW::BSService(srvName)
{

}

TestService::~TestService()
{

}

void TestService::OnTest(BSFW::BSMsgBase* pMsgBase)
{
	std::cout << __FILE__ << __LINE__ << std::endl;
	auto pMsg = dynamic_cast<TestMsg*>(pMsgBase);
	if (nullptr != pMsg)
	{
		std::cout << pMsg->m_a << " " << pMsg->m_b << std::endl;
		pMsg->m_a += 1;
		pMsg->m_b = "bye";
	}
	else
	{
		std::cout << "nullptr. " << std::endl;
	}
	
}