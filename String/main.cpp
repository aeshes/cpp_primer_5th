#include <iostream>
#include <cstring>

class String
{
public:
	String() : buffer(nullptr), length(0) {}
	String(const char *pStr);
	~String();
	String(const String&);
	String(String&&);
	String& operator=(const String&);
	String& operator=(String&&);
	size_t size() { return length; }

private:
	char *buffer;
	size_t length;

	void free();
	void alloc_and_copy(const char *);
};

String::String(const char *pStr)
{
	alloc_and_copy(pStr);
}

String::~String()
{
	free();
}

String::String(const String& rhs)
{
	alloc_and_copy(rhs.buffer);
	std::cout << "Copy constructor" << std::endl;
}

String::String(String&& str)
{
	buffer = str.buffer;
	length = str.length;

	str.buffer = nullptr;
	str.length = 0;

	std::cout << "Move constructor" << std::endl;
}

String& String::operator=(const String& rhs)
{
	if (this != &rhs)
	{
		free();
		alloc_and_copy(rhs.buffer);
	}
	std::cout << "Assignment operator" << std::endl;
	return *this;
}

String& String::operator=(String&& rhs)
{
	if (this != &rhs)
	{
		buffer = rhs.buffer;
		length = rhs.length;

		rhs.buffer = nullptr;
		rhs.length = 0;
	}
	std::cout << "Move assignment operator" << std::endl;
	return *this;
}

void String::free()
{
	if (buffer)
		delete[] buffer;
}

void String::alloc_and_copy(const char *pStr)
{
	size_t len = std::strlen(pStr);
	char *data = new char[len + 1];

	std::copy(pStr, pStr + len, data);
	data[len] = 0;

	length = len;
	buffer = data;
}

int main()
{
	String a("xxx");
	String b(a);
	//b = std::move(a);
	b = a;
}