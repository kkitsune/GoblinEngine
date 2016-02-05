#pragma once

#include <stdexcept>
#include <atomic>

#include "Common.hpp"

class ENGINE_API Object
{
public:
	inline Object()
	{ }

	inline Object(Object const&) : _ref(0)
	{ }

	inline int ref_count() const
	{ return _ref; }

	inline void add_ref() const
	{ ++_ref; }

	inline void dec_ref(bool dealloc = true) const
	{
		--_ref;
		if(_ref == 0 && dealloc)
			delete this;
		else
			throw std::runtime_error("Internal Error : reference count < 0");
	}

protected:
	virtual ~Object()
	{ }

private:
	mutable std::atomic_int _ref { 0 };
};

template<typename T>
class Ref
{
public:
	inline Ref() : _ptr(nullptr)
	{ }

	inline Ref(T* ptr) : _ptr(ptr)
	{ if(_ptr) _ptr->add_ref(); }

	inline ~Ref()
	{ if(_ptr) _ptr->dec_ref(); }

	inline Ref(Ref const& other) : _ptr(other._ptr)
	{ if(_ptr) _ptr->add_ref(); }

	inline Ref(Ref&& other) : _ptr(other._ptr)
	{ other._ptr = nullptr; }

	inline Ref& operator=(Ref const& other)
	{
		if(_ptr == other._ptr)
			return *this;

		if(_ptr) _ptr->dec_ref();
		_ptr = other._ptr;
		if(_ptr) _ptr->add_ref();
		return *this;
	}

	inline Ref& operator=(Ref&& other)
	{
		if(*this == other)
			return *this;

		if(_ptr) _ptr->dec_ref();
		_ptr = other._ptr;
		other._ptr = nullptr;
		return *this;
	}

	inline Ref& operator=(T* ptr)
	{
		if(_ptr == ptr)
			return *this;

		if(_ptr) _ptr->dec_ref();
		_ptr = ptr;
		if(_ptr) _ptr->add_ref();
		return *this;
	}

	bool operator==(Ref const& other) const
	{ return _ptr == other._ptr; }

	bool operator!=(Ref const& other) const
	{ return _ptr != other._ptr; }

	bool operator==(T const* ptr) const
	{ return _ptr == ptr; }

	bool operator!=(T const* ptr) const
	{ return _ptr != ptr; }

	T* operator->()
	{ return _ptr; }

	const T* operator->() const
	{ return _ptr; }

	T& operator*()
	{ return *_ptr; }

	const T& operator*() const
	{ return *_ptr; }

	operator T*()
	{ return _ptr; }

	T* get()
	{ return _ptr; }

	const T* get() const
	{ return _ptr; }

	operator bool() const
	{ return _ptr != nullptr; }

private:
	T* _ptr;
};
