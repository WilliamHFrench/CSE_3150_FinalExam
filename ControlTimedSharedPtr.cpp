#include "ControlTimedSharedPtr.h"

using namespace std;  // You can use namespace std here in the .cpp file.

ControlTimedSharedPtr::ControlTimedSharedPtr(void* p, long deleteMeInMilliseconds)
    : ptr(p),
      myUseCount(1),
      TimedSharedPtrStartTime(Clock::now()),
      deletePtrInMs(std::chrono::milliseconds(deleteMeInMilliseconds)) 
{
    cout << "ControlTimedSharedPtr " << static_cast<void*>(ptr)
         << " start: " << chrono::duration_cast<milliSeconds>(milliSeconds(0)).count()
         << " ms " << endl;
}

ControlTimedSharedPtr::~ControlTimedSharedPtr() {
    cout << "ControlTimedSharedPtr " << static_cast<void*>(ptr)
         << " end: "
         << chrono::duration_cast<milliSeconds>(
                Clock::now().time_since_epoch() - TimedSharedPtrStartTime.time_since_epoch())
                .count()
         << " ms " << endl;
}
