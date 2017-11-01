#pragma once

template <typename T>
class Ptr
{
public:
	inline explicit Ptr() : mPtr(nullptr) {}
	inline explicit Ptr(T* t) : mPtr(t) {}
	inline const T operator*() const { return(mPtr); }
	inline const T operator->() const { return(mPtr); }

private:
	T* mPtr;
};