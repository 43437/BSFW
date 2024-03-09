#include "TestObj.h"
#include "Export/BSObject.h"
#include "TestService.h"
#include <iostream>

TestObj::TestObj()
{
	m_pTimer = new BSFW::BSTimer;
	m_pTimer->Subscribe(this, [this]() {
		OnTimer();
	});

	m_pTimer->Start(1000);
}

void TestObj::Test(int& a, char b)
{
	std::cout << __FUNCTION__ << a << b << std::endl;
}

void TestObj::TestRef(int&a, char b)
{
	std::cout << __FUNCTION__ << a << b << std::endl;
	a += 1;
}

void TestObj::TestCall(TestObj* pObj)
{
	int param1 = 10;
	char param2 = 'b';
	BSFW::InvokeMethod(pObj, &TestObj::TestRef, BSFW::eInvokeMethodType_Block, param1, param2);
}

void TestObj::OnTimer()
{
	std::cout << __FUNCTION__ << " " << BSFW::BSTimer::CurrMSSinceEpoch() << std::endl;

	TestMsg objTestMsg;
	objTestMsg.m_a = 0;
	objTestMsg.m_b = "hello";
	BSFW::FireEvent(TEST_SRV, "test", &objTestMsg/*, BSFW::eInvokeMethodType_Block*/);

	std::cout << "after fire " << objTestMsg.m_a << " " << objTestMsg.m_b << std::endl;

	BSFW::FireEvent(TEST_SRV, "test", &objTestMsg, BSFW::eInvokeMethodType_Block);
	std::cout << "after block fire " << objTestMsg.m_a << " " << objTestMsg.m_b << std::endl;
}