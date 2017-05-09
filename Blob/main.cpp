#include <iostream>
#include <vector>
#include <initializer_list>
#include <memory>

template <typename T> class Blob
{
public:
	typedef T value_type;
	typedef typename std::vector<T>::size_type size_type;

	Blob() : data(std::make_shared<std::vector<T>>()) {}
	Blob(std::initializer_list<T> il) : data(std::make_shared<std::vector<T>>(il)) {}
	size_type size() const { return data->size(); }
	bool empty() const { return data->empty(); }
	void push_back(const T& t) { data->push_back(t); }
	void push_back(T&& t) { data->push_back(std::move(t)); }
	void pop_back();
	T& back();
	const T& back() const;
	T& front();
	const T& front() const;
	T& operator[](size_type i);

private:
	std::shared_ptr<std::vector<T>> data;
	void check(size_type i, const std::string& msg) const;
};

template <typename T>
void Blob<T>::check(size_type i, const std::string& msg) const
{
	if (i >= data->size())
		throw std::out_of_range(msg);
}

template <typename T>
T& Blob<T>::back()
{
	check(0, "back() on empty Blob");
	return data->back();
}

template <typename T>
const T& Blob<T>::back() const
{
	check(0, "const back() on empty Blob");
	return data->back();
}

template <typename T>
T& Blob<T>::front()
{
	check(0, "front() on empty Blob");
	return data->front();
}

template <typename T>
const T& Blob<T>::front() const
{
	check(0, "const front() on empty Blob");
	return data->front();
}

template <typename T>
void Blob<T>::pop_back()
{
	check(0, "pop_back() on empty Blob");
	data->pop_back();
}

template <typename T> class BlobPtr
{
public:
	BlobPtr() : curr(0) {}
	BlobPtr(Blob& b, std::size_t sz = 0)
		: wptr(b.data), curr(sz) {}
	std::string& deref() const;
	BlobPtr& incr();

private:
	std::shared_ptr<std::vector<T>> check(std::size_t, std::string&) const;
	std::weak_ptr<std::vector<T>> wptr;
	std::size_t curr;
};

template <typename T>
std::shared_ptr<std::vector<T>> BlobPtr<T>::check(std::size_t i, std::string& msg) const
{
	auto ret = wptr.lock();
	if (!ret)
		throw std::runtime_error("Unbound BlobPtr");
	if (i >= ret->size())
		throw std::out_of_range(msg);
	return ret;
}

int main()
{

}