#ifndef TIMED_SHARED_PTR_H
#define TIMED_SHARED_PTR_H

#include <iostream>
#include <chrono>
#include "ControlTimedSharedPtr.h" 

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
    T* get();  // Returns the raw pointer or nullptr if expired
    chrono::milliseconds remainingTime(); //returns remaining milliseconds before expirey
    void replaceItem(T* newItem); // replaces the ietm in the shared pointer
    void resetTimer(); //resets the timer to the current time

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
T* TimedSharedPtr<T>::get() {
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
chrono::milliseconds TimedSharedPtr<T>::remainingTime (){
    if (!_ptrToControl) {
        return std::chrono::milliseconds(0);
    }
    auto elapsed = Clock::now() - _ptrToControl->TimedSharedPtrStartTime;
    auto remaining = _ptrToControl->deletePtrInMs - std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    if (remaining.count() < 0) {
        if (_ptrToControl->ptr) {
            cout << "Expired — deleting ptr" << endl;
            delete static_cast<T*>(_ptrToControl->ptr);
            _ptrToControl->ptr = nullptr;
        }
        return std::chrono::milliseconds(0);
    }
    return remaining;
}

template<typename T>
void TimedSharedPtr<T>::replaceItem(T* newItem) {
    if (_ptrToControl) {
        if (_ptrToControl->ptr) {
            delete static_cast<T*>(_ptrToControl->ptr);
        }
        _ptrToControl->ptr = static_cast<void*>(newItem);
    }
}

template<typename T>
void TimedSharedPtr<T>::resetTimer(){
    if (_ptrToControl) {
        _ptrToControl->TimedSharedPtrStartTime = Clock::now();
    }
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
