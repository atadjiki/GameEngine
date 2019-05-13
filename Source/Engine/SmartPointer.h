#pragma once
#include <assert.h>

struct RefCounts {
	long strongCount;
	long weakCount;
};

template<class T>
class SmartPointer
{
public:

	SmartPointer(T * i_pObject) :
		m_ptr(i_pObject),
		m_pRefCounts(new RefCounts{ 1,0 })
	{
	}

	SmartPointer(T * i_pObject, RefCounts * i_refCounts) :
		m_ptr(i_pObject),
		m_pRefCounts(i_refCounts)
	{
	}

	SmartPointer() :
		m_ptr(nullptr),
		m_pRefCounts(new RefCounts{0,0})
	{
	}

	// copy constructor
	SmartPointer(const SmartPointer & i_other) :
		m_ptr(i_other.m_ptr),	// Copy the pointer to the tracked object
		m_pRefCounts(i_other.m_pRefCounts)	// Copy the pointer to the master ref count
	{
		// We've created a new smart pointer
		// Increment the master reference count
		(*m_pRefCounts).strongCount++;
	}

	// member access operator
	T * operator->()
	{
		return m_ptr;
	}
	// indirection operator
	T & operator*()
	{
		assert(m_ptr != nullptr);

		return *m_ptr;
	}

	bool operator==(const SmartPointer & i_other) {
		return (this->m_pRefCounts->strongCount == i_other.m_pRefCounts->strongCount && this->m_ptr == i_other.m_ptr);
	}

	bool operator!=(const SmartPointer & i_other) {
		return (this->m_pRefCounts->strongCount != i_other.m_pRefCounts->strongCount || this->m_ptr != i_other.m_ptr);
	}

	SmartPointer & operator=(const SmartPointer & i_other)
	{
		ReleaseCurrentReference();
        AcquireNewReference(i_other);
		return *this;

	}

	~SmartPointer()
	{
		ReleaseCurrentReference();
	}

	RefCounts getRefCounts() {
		return *m_pRefCounts;
	}

private:
	T *		m_ptr;
	RefCounts * m_pRefCounts;

	void ReleaseCurrentReference()
	{
		if (--(m_pRefCounts->strongCount) == 0)
		{
			delete m_pRefCounts;
			delete m_ptr;
		}
	}

	void AcquireNewReference(const SmartPointer & other) {

		m_ptr = other.m_ptr;
		m_pRefCounts = other.m_pRefCounts;
	}
};

template<typename T>
class WeakPointer
{
};

