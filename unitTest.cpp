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

TEST_CASE("Use count works correctly") {
    TimedSharedPtr<int> p(new int(42), 500);
    CHECK(p.use_count() == 1);

    TimedSharedPtr<int> q = p;
    CHECK(p.use_count() == 2);
    CHECK(q.use_count() == 2);

    {
        TimedSharedPtr<int> r = p;
        CHECK(p.use_count() == 3);
    }

    CHECK(p.use_count() == 2);
}

TEST_CASE("Default expiration works") {
    TimedSharedPtr<int> p(new int(123)); // Defaults to 1000ms
    CHECK(p.get() != nullptr);

    this_thread::sleep_for(chrono::milliseconds(1100));
    CHECK(p.get() == nullptr);
}
