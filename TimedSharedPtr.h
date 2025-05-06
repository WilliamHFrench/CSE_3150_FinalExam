#ifndef TIMED_SHARED_PTR_H
#define TIMED_SHARED_PTR_H

#include <iostream>
#include <chrono>
#include "ControlTimedSharedPtr.h" // Assuming this is a separate class

using namespace std;

using Clock = chrono::high_resolution_clock;
typedef chrono::time_point<chrono::high_resolution_clock> clockTime;
typedef chrono::milliseconds milliSeconds;

template<typename T>
class TimedSharedPtr {
private:
    ControlTimedSharedPtr* _ptrToControl;  // Control structure for shared pointer

public:
    TimedSharedPtr();  // Default constructor
    TimedSharedPtr(T* raw, long deleteMeInMilliseconds);  // Constructor with raw pointer and time
    TimedSharedPtr(T* raw);  // Constructor with raw pointer (defaults to 1000ms)
    TimedSharedPtr(const TimedSharedPtr& other);  // Copy constructor
    TimedSharedPtr& operator=(const TimedSharedPtr& other);  // Assignment operator

    long use_count() const noexcept;  // Returns the use count
    T* get() const;  // Returns the raw pointer or nullptr if expired

    ~TimedSharedPtr();  // Destructor
};

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr() : _ptrToControl(nullptr) {}

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr(T* raw, long deleteMeInMilliseconds) 
    : _ptrToControl(new ControlTimedSharedPtr(raw, deleteMeInMilliseconds)) {}

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr(T* raw) 
    : _ptrToControl(new ControlTimedSharedPtr(raw, 1000)) {}

template<typename T>
TimedSharedPtr<T>::TimedSharedPtr(const TimedSharedPtr& other) {
    _ptrToControl = other._ptrToControl;
    if (_ptrToControl) {
        _ptrToControl->myUseCount++;
    }
}

template<typename T>
TimedSharedPtr<T>& TimedSharedPtr<T>::operator=(const TimedSharedPtr& other) {
    if (this != &other) {
        if (_ptrToControl && --_ptrToControl->myUseCount == 0) {
            delete static_cast<T*>(_ptrToControl->ptr);
            delete _ptrToControl;
        }
        _ptrToControl = other._ptrToControl;
        if (_ptrToControl) {
            _ptrToControl->myUseCount++;
        }
    }
    return *this;
}

template<typename T>
long TimedSharedPtr<T>::use_count() const noexcept {
    return _ptrToControl ? _ptrToControl->myUseCount : 0;
}

template<typename T>
T* TimedSharedPtr<T>::get() const {
    if (!_ptrToControl) return nullptr;

    auto millisecondsSinceStart = Clock::now() - _ptrToControl->TimedSharedPtrStartTime;
    if (millisecondsSinceStart > _ptrToControl->deletePtrInMs) {
        if (_ptrToControl->ptr) {
            cout << "Expired — deleting ptr" << endl;
            delete static_cast<T*>(_ptrToControl->ptr);
            _ptrToControl->ptr = nullptr;
        }
        return nullptr;
    }

    return static_cast<T*>(_ptrToControl->ptr);
}

template<typename T>
TimedSharedPtr<T>::~TimedSharedPtr() {
    if (_ptrToControl) {
        _ptrToControl->myUseCount--;
        if (_ptrToControl->myUseCount == 0) {
            delete static_cast<T*>(_ptrToControl->ptr);
            delete _ptrToControl;
        }
    }
}

#endif // TIMED_SHARED_PTR_H
