#include <iostream>

template<class T>
class SmartPointer
{
public:
	SmartPointer(T * object)
	{
		ref = object;
		ref_count = new std::size_t(1);
	}
	SmartPointer(SmartPointer<T> &sptr)
	{
		ref = sptr.ref;
		ref_count = sptr.ref_count;
		++(*ref_count);
	}
	~SmartPointer()
	{
		remove();
	}

	SmartPointer<T>& operator=(SmartPointer<T> &sptr)
	{
		if (this == &sptr) return *this;

		if (*ref_count > 0) remove();

		ref = sptr.ref;
		ref_count = sptr.ref_count;
		++(*ref_count);

		return *this;
	}

	T operator *()
	{
		return *ref;
	}

protected:
	void remove()
	{
		--(*ref_count);
		if (*ref_count == 0)
		{
			delete ref;
			delete ref_count;

			ref = nullptr;
			ref_count = nullptr;
		}
	}

	T * ref;
	std::size_t * ref_count;
};

class Object
{
public:
	Object()
	{
		std::cout << "Object created" << std::endl;
	}
	~Object()
	{
		std::cout << "Object deleted" << std::endl;
	}
	void action()
	{
		std::cout << "action() called" << std::endl;
	}
};


void test()
{
	SmartPointer<Object> ptr(new Object);	// Create new smart pointer to Object
	SmartPointer<Object> cpy = ptr;			// Create the second smart pointer pointing to the same Object

	(*ptr).action();						// Call Object's method using smart pointer
}


int main()
{
	test();
}