#include <iostream>
#include <cstddef>
#include <chrono>
#include <utility>
#include <thread>
#include "node.h"

using namespace std;

int main() {
    // Original tests
    TimedSharedPtr<Node> myNode(new Node(7), 100); // Node expires in 100ms
    TimedSharedPtr<Node> myOtherNode = myNode;

    this_thread::sleep_for(chrono::milliseconds(50)); // Sleep for 50ms
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;

    cout << "myNode.use_count(): " << myNode.use_count() << endl;
    cout << "myOtherNode.use_count(): " << myOtherNode.use_count() << endl;

    this_thread::sleep_for(chrono::milliseconds(25)); // Sleep for 25 more ms
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;

    this_thread::sleep_for(chrono::milliseconds(75)); // Sleep for 75 more ms
    cout << "The ptr should have expired: " << endl;
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;

    // Demonstrating replaceItem()
    cout << "-----------" << endl;
    TimedSharedPtr<int> p(new int(42), 200);
    cout << "Original value: " << *p.get() << endl;
    p.replaceItem(new int(99));
    cout << "After replaceItem(): " << *p.get() << endl;

    // Demonstrating resetTimer()
    cout << "-----------" << endl;
    cout << "Testing resetTimer()" << endl;
    TimedSharedPtr<int> timerTest(new int(100), 200); // Expires in 200ms

    cout << "Initial remaining time: " << timerTest.remainingTime().count() << " ms" << endl;
    this_thread::sleep_for(chrono::milliseconds(150));
    cout << "After 150ms sleep, remaining: " << timerTest.remainingTime().count() << " ms" << endl;

    // Reset timer
    cout << "Resetting timer..." << endl;
    timerTest.resetTimer();
    cout << "Remaining time after reset: " << timerTest.remainingTime().count() << " ms" << endl;

    this_thread::sleep_for(chrono::milliseconds(150));
    cout << "After another 150ms sleep, remaining: " << timerTest.remainingTime().count() << " ms" << endl;

    this_thread::sleep_for(chrono::milliseconds(100));
    cout << "Finally, is it expired? " << (timerTest.get() == nullptr ? "Yes" : "No") << endl;

    return 0;
}
