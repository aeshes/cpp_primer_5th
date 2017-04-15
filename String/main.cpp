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
	friend std::ostream& operator<<(std::ostream&, const String&);
	friend bool operator==(const String& lhs, const String& rhs);
	friend bool operator!=(const String& lhs, const String& rhs);
	friend bool operator<(const String& lhs, const String& rhs);
	char operator[](std::size_t n) { return buffer[n]; }
	const char operator[](std::size_t n) const { return buffer[n]; }

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
		free();

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

std::ostream& operator<<(std::ostream& os, const String& str)
{
	os << str.buffer;
	return os;
}

bool operator==(const String& lhs, const String& rhs)
{
	return std::strcmp(lhs.buffer, rhs.buffer) == 0;
}

bool operator!=(const String& lhs, const String& rhs)
{
	return !(lhs == rhs);
}

bool operator<(const String& lhs, const String& rhs)
{
	return std::strcmp(lhs.buffer, rhs.buffer) < 0;
}

int main()
{
	String a("xxx");
	String b(a);
	String c("yyy");
	//b = std::move(a);
	b = a;

	std::cout << b << std::endl;
	std::cout << (a == b) << std::endl;
	std::cout << (a == c) << std::endl;
	std::cout << (a < c) << " " << (c < a) << std::endl;

	std::cout << c[1] << std::endl;
}