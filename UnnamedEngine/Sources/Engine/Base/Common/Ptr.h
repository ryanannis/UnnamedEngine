#pragma once

#include <type_traits>

template <typename T>
class Ptr
{
public:
	Ptr() : mPtr(nullptr) {}
	Ptr(T* t) : mPtr(t) {}
	const std::add_lvalue_reference<T> operator*() const { return(*mPtr); }
	const T* operator->() const { return(mPtr); }
	std::add_lvalue_reference<T> operator*() { return(*mPtr); }
	T* operator->() { return(mPtr); }
	operator bool() { return(mPtr != nullptr); };

	// This is purposely obnoxious - you shouldn't need except for interfacing and doing weird stuff
	operator T*() { return(mPtr); } 
private:
	T* mPtr;
};