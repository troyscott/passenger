#include "tut.h"
#include "support/Support.h"
#include "StandardApplicationPool.h"
#include "Utils.h"

using namespace Passenger;
using namespace Test;

namespace tut {
	struct StandardApplicationPoolTest {
		ApplicationPoolPtr pool, pool2;
		
		StandardApplicationPoolTest() {
			pool = ptr(new StandardApplicationPool("../bin/passenger-spawn-server"));
			pool2 = pool;
		}
		
		ApplicationPoolPtr newPoolConnection() {
			return pool;
		}
	};

	DEFINE_TEST_GROUP(StandardApplicationPoolTest);

	#define USE_TEMPLATE
	#include "ApplicationPoolTest.cpp"
}
