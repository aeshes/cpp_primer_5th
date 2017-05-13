#include <iostream>
#include <string>
#include <vector>
#include <list>

template <class C>
void print(const C& c)
{
	using size_type = typename C::size_type;
	for (size_type i = 0; i < c.size(); ++i)
		std::cout << c[i] << std::endl;
}

template <class C>
void print2(const C& c)
{
	for (auto it = c.begin(); it != c.end(); ++it)
		std::cout << *it << std::endl;
}

int main()
{
	std::vector<int> v = { 1, 2, 3 };
	print(v);
	print2(v);
}