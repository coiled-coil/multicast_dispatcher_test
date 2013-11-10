#include <iostream>
#include <functional>
#include <messenger/multicast_messenger.hpp>
#include "coroutine.hpp"

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( multicast_messenger_test )
{
    int count = 0;

    messenger::multicast_messenger m;
    m.register_handler(1, [&](int error_code, int token) {
        count++;
    });
    m.register_handler(1, [&](int error_code, int token) {
        count++;
    });

    m.invoke(0, 1);

    BOOST_CHECK_EQUAL(2, count);
}

#define TRACE do { cout << "trace: " << __LINE__ << endl; } while(0)

#include "yield.hpp"
BOOST_AUTO_TEST_CASE( yield_test )
{
    using namespace std;

    int count = 0;
    messenger::multicast_messenger m;

    coroutine ctx;
    function< void(int, int) > handler = [&](int error_code, int token)
    {
        cout << "ctx.value_ = " << ctx.value_ << " token = " << token << endl;

        reenter(ctx) {
            TRACE;
            yield m.register_handler(1, handler);
            count++;

            TRACE;
            yield m.register_handler(2, handler);
            count++;

            TRACE;
            yield m.register_handler(3, handler);
            count++;

            TRACE;
            yield m.register_handler(4, handler);
            count++;

            TRACE;
        }

        TRACE;
    };

    TRACE;
    handler(0, 0);

    TRACE;
    m.invoke(0, 1);
    TRACE;
    m.invoke(0, 2);
    TRACE;
    m.invoke(0, 3);
    TRACE;

    BOOST_CHECK_EQUAL(3, count);
}
#include "unyield.hpp"
