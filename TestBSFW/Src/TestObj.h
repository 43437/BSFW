#ifndef __TEST_OBJ_H__
#define __TEST_OBJ_H__

#include "Export/BSObject.h"
#include "Export/BSTimer.h"

class TestObj : public BSFW::BSObject
{
public:
	TestObj();
	virtual ~TestObj() {};
	void Test(int& a, char b);
	void TestRef(int&a, char b);

	void TestCall(TestObj* obj);

private:
	void OnTimer();

private:
	BSFW::BSTimer* m_pTimer;
};

#endif

