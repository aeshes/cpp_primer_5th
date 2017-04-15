#include <iostream>
#include <string>
#include <memory>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <vector>

class StrVec
{
public:
	StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec &);
	StrVec(std::initializer_list<std::string> il);
	StrVec(StrVec&&) noexcept;
	StrVec& operator=(const StrVec &);
	StrVec& operator=(StrVec&&) noexcept;
	StrVec& operator=(std::initializer_list<std::string> il);
	std::string& operator[](std::size_t n) { return elements[n]; }
	const std::string& operator[](std::size_t n) const { return elements[n]; }
	~StrVec();

	void push_back(const std::string &);
	void push_back(std::string&&);
	std::size_t size() const { return first_free - elements; }
	std::size_t capacity() const { return cap - elements; }
	std::string *begin() const { return elements; }
	std::string *end() const { return first_free; }

private:
	std::allocator<std::string> alloc;
	void check_and_alloc()
	{
		if (size() == capacity()) reallocate();
	}
	std::pair<std::string *, std::string *> alloc_and_copy(const std::string *, const std::string *);
	void free();
	void reallocate();

	std::string *elements;
	std::string *first_free;
	std::string *cap;
};

void StrVec::push_back(const std::string &s)
{
	check_and_alloc();
	alloc.construct(first_free++, s);
}

void StrVec::push_back(std::string&& s)
{
	check_and_alloc();
	alloc.construct(first_free++, std::move(s));
}

std::pair<std::string *, std::string *>
StrVec::alloc_and_copy(const std::string *b, const std::string *e)
{
	auto data = alloc.allocate(e - b);
	return { data, std::uninitialized_copy(b, e, data) };
}

void StrVec::free()
{
	if (elements)
	{
		/*for (auto p = first_free; p != elements; )
			alloc.destroy(--p);*/
		std::for_each(elements, first_free, [&](auto &s) { alloc.destroy(&s); });
		alloc.deallocate(elements, cap - elements);
	}
}

StrVec::StrVec(const StrVec &s)
{
	auto newdata = alloc_and_copy(s.begin(), s.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}

StrVec::StrVec(std::initializer_list<std::string> il)
{
	auto newdata = alloc_and_copy(il.begin(), il.end());
	elements = newdata.first;
	first_free = cap = newdata.second;
}

StrVec::StrVec(StrVec&& s) noexcept
	: elements(s.elements), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.first_free = s.cap = nullptr;
}

StrVec::~StrVec()
{
	free();
}

StrVec& StrVec::operator=(const StrVec &rhs)
{
	auto data = alloc_and_copy(rhs.begin(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}

StrVec& StrVec::operator=(StrVec&& rhs) noexcept
{
	if (this != &rhs)
	{
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;

		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}

void StrVec::reallocate()
{
	auto newcapacity = size() ? 2 * size() : 1;
	auto first = alloc.allocate(newcapacity);
	auto last = std::uninitialized_copy(std::make_move_iterator(begin()),
		std::make_move_iterator(end()),
		first);

	free();

	elements = first;
	first_free = last;
	cap = elements + newcapacity;
}

StrVec& StrVec::operator=(std::initializer_list<std::string> il)
{
	auto data = alloc_and_copy(il.begin(), il.end());
	free();

	elements = data.first;
	first_free = cap = data.second;

	return *this;
}


int main()
{
	StrVec v = { "aaa", "bbb", "ccc" };

	for (auto str : v) std::cout << str << std::endl;
}