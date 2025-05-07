#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "include/doctest.h"
#include "node.h"
#include <thread>
#include <chrono>

using namespace std;

TEST_CASE("Node expires after delay") {
    TimedSharedPtr<Node> myNode(new Node(7), 100);
    TimedSharedPtr<Node> myOtherNode = myNode;

    this_thread::sleep_for(chrono::milliseconds(50));
    CHECK(myNode.get() != nullptr);

    this_thread::sleep_for(chrono::milliseconds(25));
    CHECK(myNode.get() != nullptr);

    this_thread::sleep_for(chrono::milliseconds(50));
    CHECK(myNode.get() == nullptr);
    CHECK(myOtherNode.get() == nullptr);
}

TEST_CASE("Use count, copy constructor and assignment operator =)") {
    TimedSharedPtr<int> p(new int(42), 500);
    CHECK(p.use_count() == 1);

    TimedSharedPtr<int> q = p;
    CHECK(p.use_count() == 2);
    CHECK(q.use_count() == 2);

    TimedSharedPtr<int> r;
    r = p;
    CHECK(p.use_count() == 3);

}

TEST_CASE("Default expiration works") {
    TimedSharedPtr<int> p(new int(123)); 
    CHECK(p.get() != nullptr);

    this_thread::sleep_for(chrono::milliseconds(1100));
    CHECK(p.get() == nullptr);
}

TEST_CASE("remainingTime works") {
    TimedSharedPtr<int> p(new int(10), 200); // 200 ms expiration
    CHECK(p.get() != nullptr);

    this_thread::sleep_for(chrono::milliseconds(100));
    auto remaining = p.remainingTime();
    CHECK(remaining.count() <= 100); // should be ≤100ms remaining
    CHECK(remaining.count() > 75);    // should be > 75

    this_thread::sleep_for(chrono::milliseconds(50));
    remaining = p.remainingTime();
    CHECK(remaining.count() <= 50); // should be ≤50ms remaining
    CHECK(remaining.count() > 25);    // should > 25

    this_thread::sleep_for(chrono::milliseconds(120));
    CHECK(p.get() == nullptr);
    CHECK(p.remainingTime().count() == 0); // expired, so should return 0
}

TEST_CASE("replaceItem works") {
    TimedSharedPtr<int> a(new int(5), 1000);
    TimedSharedPtr<int> b = a;

    CHECK(*a.get() == 5);
    CHECK(*b.get() == 5);

    a.replaceItem(new int(99)); // Replace value

    CHECK(*a.get() == 99);
    CHECK(*b.get() == 99); // b also sees the new value

    CHECK(a.use_count() == 2);
}

TEST_CASE("Reset Timer works as expected") {
    TimedSharedPtr<Node> myNode(new Node(7), 100); 

    // Sleep for 50ms (before expiration)
    this_thread::sleep_for(chrono::milliseconds(50));
    CHECK(myNode.get() != nullptr);  // Node should still exist

    // Reset the timer
    myNode.resetTimer();

    // Sleep for another 50ms (total 100ms since start, but timer was reset)
    this_thread::sleep_for(chrono::milliseconds(50));
    CHECK(myNode.get() != nullptr);  // Node should still exist due to reset

    // Sleep again to exceed the timer (total 150ms since reset)
    this_thread::sleep_for(chrono::milliseconds(50));
    CHECK(myNode.get() == nullptr);  
}