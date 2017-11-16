#pragma once

#include <type_traits>

template <typename T>
class Ptr
{
public:
	Ptr() : mPtr(nullptr) {}
	Ptr(T* t) : mPtr(t) {}
	std::add_lvalue_reference<T> operator*() const { return(*mPtr); }
	T* operator->() const { return(mPtr); }
	operator bool() const  { return(mPtr != nullptr); }

	// This is purposely obnoxious - you shouldn't need except for interfacing and doing weird stuff
	operator T*() { return(mPtr); } 

	T* GetPtr() { return(mPtr); }
private:
	T* mPtr;
};