#include <iostream>

class ternary_op
{
public:
	int operator()(int first, int second, int third) const
	{
		if (first)
			return second;
		else
			return third;
	}
};

int main()
{
	ternary_op t;

	std::cout << t(0, 2, 3) << std::endl;
}