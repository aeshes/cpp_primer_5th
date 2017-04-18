#include <iostream>
#include <string>

class ReadString
{
public:
	ReadString(std::istream &i = std::cin)
		: is(i) {}
	void operator()(std::string &s)
	{
		is >> s;
		if (!is) s = std::string();
	}
private:
	std::istream &is;
};

int main()
{
	std::string s;
	ReadString reader;

	reader(s);
	std::cout << s << std::endl;
}