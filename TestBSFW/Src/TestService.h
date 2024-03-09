#ifndef __TEST_SERVICE_H__
#define __TEST_SERVICE_H__

#include "Export/BSService.h"
#include <string>

#define TEST_SRV "TestSrv"

class TestMsg : public BSFW::BSMsgBase
{
	DECL_MSG_TYPE(TestMsg)
public:
	int m_a;
	std::string m_b;
};

class TestService : public BSFW::BSService
{
public:
	TestService(const std::string& srvName);
	~TestService();

	BEGIN_BIND
		BIND_TRIGGER("test", &TestService::OnTest);
	END_BIND

private:
	void OnTest(BSFW::BSMsgBase* pMsgBase);
};

#endif