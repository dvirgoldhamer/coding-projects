/*******************************************************************************
* File: dispatcher_test.cpp
*
* Purpose:
*   Test suite for dispatcher.hpp
*
* Author:
*   Shani Zuniga
*
*******************************************************************************/

#include <iostream> // cout, endl

#include "testing_util.h" // testing utils
#include "dispatcher.hpp" // own header

using namespace ilrd;

/******************************************************************************/

// Test Event Types
struct TestEvent
{
    int value;
    std::string message;

    TestEvent(int v = 0, const std::string& msg = "") : value(v), message(msg) {}

    bool operator==(const TestEvent& other) const
    {
        return ((value == other.value) && (message == other.message));
    }
};

// Mock Callback for Testing
template<typename EVENT>
class MockCallback : public CallbackBase<EVENT>
{
public:
    MockCallback() : notifCount(0), deathCount(0), lastEvent() {}

    virtual void Notify(const EVENT& event) override
    {
        ++notifCount;
        lastEvent = event;
    }

    virtual void NotifyDeath() override
    {
        ++deathCount;
    }

    // Test accessors
    int GetNotifyCount() const { return notifCount; }
    int GetDeathCount() const { return deathCount; }
    const EVENT& GetLastEvent() const { return lastEvent; }
    void ResetCounts() { notifCount = 0; deathCount = 0; }

private:
    int notifCount;
    int deathCount;
    EVENT lastEvent;
};

// Observer class for testing the Callback wrapper
class TestObserver
{
public:
    TestObserver() : notifCount(0), deathCount(0), lastValue(0) {}

    void OnNotify(const TestEvent& event)
    {
        ++notifCount;
        lastEvent = event;
        lastValue = event.value;
    }

    void OnNotifyInt(const int& value)
    {
        ++notifCount;
        lastValue = value;
    }

    void OnDeath()
    {
        ++deathCount;
    }

    // Test accessors
    int GetNotifyCount() const { return notifCount; }
    int GetDeathCount() const { return deathCount; }
    int GetLastValue() const { return lastValue; }
    const TestEvent& GetLastEvent() const { return lastEvent; }
    void ResetCounts() { notifCount = 0; deathCount = 0; }

private:
    int notifCount;
    int deathCount;
    int lastValue;
    TestEvent lastEvent;
};

// Tests Prototypes
static int TestDispatcherCtorDtor(void);
static int TestSubscribe(void);
static int TestUnsubscribe(void);
static int TestNotify(void);
static int TestCallbackDtor(void);
static int TestMultipleSubscribers(void);
static int TestEdgeCases(void);
static int TestCallbackWithObserver(void);
static int TestCallbackDeathNotification(void);

/******************************************************************************/

static int TestDispatcherCtorDtor(void)
{
    int status = 0;

    std::cout << "Testing Dispatcher Ctor/Dtor...";

    {
        Dispatcher<int> dispatcher;
        status |= TEST_CHECK(1, "Constructed without failure");

        Dispatcher<TestEvent> dispatcher2;
        status |= TEST_CHECK(1, "Constructed with custom event type");
    } // destructors called here

    status |= TEST_CHECK(1, "Destructed without failure, run with valgrind");

    TEST_RESULT(status);
    return (status);
}

static int TestSubscribe(void)
{
    int status = 0;

    std::cout << "Testing Subscribe...\t";

    Dispatcher<TestEvent> dispatcher;
    MockCallback<TestEvent> callback1;
    MockCallback<TestEvent> callback2;

    // Test single subscription
    dispatcher.Subscribe(&callback1);
    status |= TEST_CHECK(1, "Single subscription successful");

    // Test multiple subscriptions
    dispatcher.Subscribe(&callback2);
    status |= TEST_CHECK(1, "Multiple subscriptions successful");

    // Test same callback subscribed multiple times (should be allowed)
    dispatcher.Subscribe(&callback1);
    status |= TEST_CHECK(1, "Duplicate subscription handled");

    TEST_RESULT(status);
    return (status);
}

static int TestUnsubscribe(void)
{
    int status = 0;

    std::cout << "Testing Unsubscribe...\t";

    Dispatcher<TestEvent> dispatcher;
    MockCallback<TestEvent> callback1;
    MockCallback<TestEvent> callback2;

    // Subscribe first
    dispatcher.Subscribe(&callback1);
    dispatcher.Subscribe(&callback2);

    // Test unsubscribe existing callback
    dispatcher.Unsubscribe(&callback1);
    status |= TEST_CHECK(1, "Unsubscribe existing callback successful");

    // Test unsubscribe non-existing callback (should not crash)
    MockCallback<TestEvent> callback3;
    dispatcher.Unsubscribe(&callback3);
    status |= TEST_CHECK(1, "Unsubscribe non-existing callback handled gracefully");

    // Test unsubscribe already unsubscribed callback
    dispatcher.Unsubscribe(&callback1);
    status |= TEST_CHECK(1, "Unsubscribe already removed callback handled");

    TEST_RESULT(status);
    return (status);
}

static int TestNotify(void)
{
    int status = 0;

    std::cout << "Testing Notify...\t";

    Dispatcher<TestEvent> dispatcher;
    MockCallback<TestEvent> callback1;
    MockCallback<TestEvent> callback2;

    // Test notify with no subscribers
    TestEvent event1(42, "test message");
    dispatcher.Notify(event1);
    status |= TEST_CHECK(1, "Notify with no subscribers handled");

    // Subscribe and test notify
    dispatcher.Subscribe(&callback1);
    dispatcher.Subscribe(&callback2);

    dispatcher.Notify(event1);
    status |= TEST_CHECK(callback1.GetNotifyCount() == 1,
		"Callback1 not notified");
    status |= TEST_CHECK(callback2.GetNotifyCount() == 1,
		"Callback2 not notified");
    status |= TEST_CHECK(callback1.GetLastEvent() == event1,
		"Callback1 didn't receive correct event");
    status |= TEST_CHECK(callback2.GetLastEvent() == event1,
		"Callback2 didn't receive correct event");

    // Test multiple notifications
    TestEvent event2(100, "second message");
    dispatcher.Notify(event2);
    status |= TEST_CHECK(callback1.GetNotifyCount() == 2,
		"Callback1 not notified 2ne time");
    status |= TEST_CHECK(callback2.GetNotifyCount() == 2,
		"Callback2 not notified 2nd time");
    status |= TEST_CHECK(callback1.GetLastEvent() == event2,
		"Callback1 didn't receive second event");

    TEST_RESULT(status);
    return (status);
}

static int TestCallbackDtor(void)
{
    int status = 0;

    std::cout << "Testing Callback Dtor...";

    Dispatcher<TestEvent> dispatcher;

    // Test callback destructor unsubscribes from dispatcher
    {
        MockCallback<TestEvent> callback;
        dispatcher.Subscribe(&callback);

        TestEvent event(1, "test");
        dispatcher.Notify(event);
        status |= TEST_CHECK(callback.GetNotifyCount() == 1,
			"Callback did not receive notification");
    } // callback destructor called here

    // Create new callback and verify old one was properly removed
    MockCallback<TestEvent> newCallback;
    dispatcher.Subscribe(&newCallback);

    TestEvent event2(2, "test2");
    dispatcher.Notify(event2);
    status |= TEST_CHECK(newCallback.GetNotifyCount() == 1,
		"New callback was not properly notified");

    TEST_RESULT(status);
    return (status);
}

static int TestMultipleSubscribers(void)
{
    int status = 0;

    std::cout << "Testing Multiple Subscribers...";

    Dispatcher<int> dispatcher;
    MockCallback<int> callbacks[5];

    // Subscribe multiple callbacks
    for (int i = 0; i < 5; ++i)
    {
        dispatcher.Subscribe(&callbacks[i]);
    }

    // Notify all
    dispatcher.Notify(999);

    // Verify all received notification
    for (int i = 0; i < 5; ++i)
    {
        status |= TEST_CHECK(callbacks[i].GetNotifyCount() == 1,
            "Callback did not receive notification");
        status |= TEST_CHECK(callbacks[i].GetLastEvent() == 999,
        	"Callback did not receive correct value");
    }

    // Unsubscribe middle callback
    dispatcher.Unsubscribe(&callbacks[2]);
    callbacks[2].ResetCounts();

    dispatcher.Notify(888);

    // Verify unsubscribed callback didn't receive notification
    status |= TEST_CHECK(callbacks[2].GetNotifyCount() == 0,
		"Unsubscribed callback was incorrectly notified");

    // Verify others still received notification
    status |= TEST_CHECK(callbacks[0].GetNotifyCount() == 2,
		"Other callbacks were not notified");
    status |= TEST_CHECK(callbacks[4].GetNotifyCount() == 2,
		"Other callbacks were not notified");

    TEST_RESULT(status);
    return (status);
}

static int TestEdgeCases(void)
{
    int status = 0;

    std::cout << "Testing Edge Cases...\t";

    // Test with different event types
    Dispatcher<std::string> strDispatcher;
    MockCallback<std::string> strCallback;

    strDispatcher.Subscribe(&strCallback);
    strDispatcher.Notify(std::string("Hello World"));

    status |= TEST_CHECK(strCallback.GetNotifyCount() == 1,
		"String event notification not received");
    status |= TEST_CHECK(strCallback.GetLastEvent() == "Hello World",
		"Incorrect string received");

    // Test unsubscribe all then notify
    Dispatcher<int> dispatcher;
    MockCallback<int> callback1, callback2;

    dispatcher.Subscribe(&callback1);
    dispatcher.Subscribe(&callback2);
    dispatcher.Unsubscribe(&callback1);
    dispatcher.Unsubscribe(&callback2);

    dispatcher.Notify(42);
    status |= TEST_CHECK(callback1.GetNotifyCount() == 0,
		"Unsubscribed callback1 improperly notified");
    status |= TEST_CHECK(callback2.GetNotifyCount() == 0,
		"Unsubscribed callback2 improperly notified");

    TEST_RESULT(status);
    return (status);
}

static int TestCallbackWithObserver(void)
{
    int status = 0;

    std::cout << "Testing Callback w/ Observer...";

    Dispatcher<TestEvent> dispatcher;
    TestObserver observer;

    // Create callback that wraps observer method
    Callback<TestEvent, TestObserver> callback(
        observer,
        &TestObserver::OnNotify,
        &TestObserver::OnDeath
    );

    dispatcher.Subscribe(&callback);

    // Test notification forwarding
    TestEvent event(123, "callback test");
    dispatcher.Notify(event);

    status |= TEST_CHECK(observer.GetNotifyCount() == 1,
		"Observer did not receive notification");
    status |= TEST_CHECK(observer.GetLastValue() == 123,
		"Observer did not receive correct value");
    status |= TEST_CHECK(observer.GetLastEvent() == event,
		"Observer did not receive correct event");

    // Test multiple notifications
    TestEvent event2(456, "second test");
    dispatcher.Notify(event2);

    status |= TEST_CHECK(observer.GetNotifyCount() == 2,
		"Observer did not receive second notification");
    status |= TEST_CHECK(observer.GetLastValue() == 456,
		"Observer did not receive second value");

    TEST_RESULT(status);
    return (status);
}

static int TestCallbackDeathNotification(void)
{
    int status = 0;

    std::cout << "Testing Callback Death Notif...";

    TestObserver observer;

    // Test death notification is forwarded to observer
    {
        Callback<int, TestObserver> callback(
            observer,
            &TestObserver::OnNotifyInt,
            &TestObserver::OnDeath
        );
        Dispatcher<int> dispatcher;

        dispatcher.Subscribe(&callback);
        status |= TEST_CHECK(observer.GetDeathCount() == 0,
			"Death counter non-zero prior to dispatcher death");
    } // dispatcher destructor calls NotifyDeath

    status |= TEST_CHECK(observer.GetDeathCount() == 1,
		"Observer did not receive dispatcher death notification");

    TEST_RESULT(status);
    return (status);
}

int main(void)
{
	int status = 0;

	status |= TestDispatcherCtorDtor();
	status |= TestSubscribe();
	status |= TestUnsubscribe();
	status |= TestNotify();
	status |= TestCallbackDtor();
	status |= TestMultipleSubscribers();
	status |= TestEdgeCases();
	status |= TestCallbackWithObserver();
	status |= TestCallbackDeathNotification();

	PrintTestSummary(status);
	return (status);
}
