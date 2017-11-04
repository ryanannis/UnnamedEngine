#pragma once

template <typename T>
class Ptr
{
public:
	Ptr() : mPtr(nullptr) {}
	Ptr(T* t) : mPtr(t) {}
	inline const T operator*() const { return(mPtr); }
	inline const T operator->() const { return(mPtr); }
	inline T operator*() { return(mPtr); }
	inline T* operator->() { return(mPtr); }
	operator bool() { return(mPtr != nullptr); };

	// This is purposely obnoxious - you shouldn't need except for interfacing and doing weird stuff
	operator T*() { return(mPtr); } 
private:
	T* mPtr;
};