#include <iostream>

class Foo
{
public:
	Foo sorted() &&;
	Foo sorted() const &;
};

Foo Foo::sorted() &&
{
	std::cout << "sorted() && is called" << std::endl;

	return *this;
}

Foo Foo::sorted() const &
{
	std::cout << "sorted() const & is called" << std::endl;

	//Foo ret(*this);
	//return ret.sorted();
	return Foo(*this).sorted();
}

int main()
{
	Foo f;
	f.sorted();
}