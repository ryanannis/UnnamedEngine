#pragma once

template <typename T>
class Ptr
{
public:
	inline Ptr() : mPtr(nullptr) {}
	inline Ptr(T* t) : mPtr(t) {}
	inline const T operator*() const { return(mPtr); }
	inline const T operator->() const { return(mPtr); }
	operator bool(){ return(mPtr != nullptr) };

	// This is purposely obnoxious - you shouldn't need except for interfacing and doing weird stuff
	inline T* GETPTR() { return(mPtr); } 
private:
	T* mPtr;
};