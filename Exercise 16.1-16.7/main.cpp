#include <iostream>
#include <string>
#include <vector>
#include <list>

template <typename T>
inline int compare(const T& lhs, const T& rhs)
{
	if (lhs < rhs) return -1;
	if (rhs < lhs) return 1;
	return 0;
}

struct complex
{
	double re;
	double im;
};

template <typename Iter, typename T>
Iter xfind(Iter begin, Iter end, const T& search)
{
	while (begin++ != end)
		if (*begin == search)
			return begin;
	return end;
}

template <typename T, std::size_t N >
void print(T (&arrayref)[N])
{
	for (auto i : arrayref)
		std::cout << i << " ";
	std::cout << std::endl;
}

template <typename T, std::size_t N>
T* array_begin(T (&arr)[N])
{
	return arr;
}

template <typename T, std::size_t N>
T* array_end(T(&arr)[N])
{
	return arr + N;
}

template <typename T, std::size_t N>
constexpr std::size_t array_length(T (&arr)[N])
{
	return N;
}

int main()
{
	std::string a("aaa"), b("aaa"), c("ccc");

	std::cout << compare(a, c) << std::endl;

	int arr[] = { 1, 2, 3, 4, 5 };
	print(arr);

	for (auto it = array_begin(arr); it != array_end(arr); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;

	std::cout << array_length(arr) << std::endl;
}