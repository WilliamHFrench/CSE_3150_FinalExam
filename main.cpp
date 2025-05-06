/*
    This is starter code - it DOES NOT FULLY solve the Final exam
        This example also does not delete its ControlTimedSharedPtr instances...

    It does show several key functions
    in the struct ControlTimedSharedPtr it shows how to work the timer
    in the class TimedSharedPtr it shows how we invoke instances of ControlTimedSharedPtr 

*/
#include <iostream>
#include <cstddef>
#include <chrono>
#include <utility>
#include <thread>
#include "node.h"

using namespace std;

int main() {

    
    TimedSharedPtr<Node> myNode(new Node(7), 100); // Node holds data of 7 and expires in 100 milliseconds
                                                   // Note: the 100 is an argument to the TimedSharedPtr constructor
    TimedSharedPtr<Node> myOtherNode = myNode;

    this_thread::sleep_until(Clock::now() + chrono::milliseconds(50)); // sleep for 50 milliseconds
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;

    cout << "myNode.use_count(): " << myNode.use_count() << endl;
    cout << "myOtherNode.use_count(): " << myOtherNode.use_count() << endl;

    this_thread::sleep_until(Clock::now() + chrono::milliseconds(25)); // sleep for 25 more milliseconds
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;

    this_thread::sleep_until(Clock::now() + chrono::milliseconds(75)); // sleep for 75 more milliseconds
    cout << "The ptr should have expired: " << endl;
    cout << "myNode.get() address: <" << myNode.get() << ">" << endl;

    cout << "-----------" << endl;
    TimedSharedPtr<int> p(new int(42));

    cout << p.get() << endl;
    cout << "p.use_count(): " << p.use_count() << endl;

    TimedSharedPtr<int> q = p;
    cout << "p.use_count(): " << p.use_count() << endl;
    cout << "q.use_count(): " << q.use_count() << endl;

    return 0;
}
