#pragma once

template <typedef T>
class Ptr
{
public:
	inline explicit Ptr(T* t) : mPtr(t) {}
	inline const T operator*() const { return(mPtr); }
	inline const T operator->() const { return(mPtr); }

private:
	T* mPtr;
};