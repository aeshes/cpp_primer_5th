#include <iostream>
#include <string>

class quote
{
public:
	quote() = default;
	quote(const std::string &book, double sales_price)
		: bookNo(book), price(sales_price) {}
	std::string isbn() const { return bookNo; }
	virtual double net_price(std::size_t n) const { return n * price; }
	virtual ~quote() = default;

private:
	std::string bookNo;
protected:
	double price = 0.0;
};

class bulk_quote : public quote
{
public:
	double net_price(std::size_t n) const override;
};

double print_total(std::ostream &os, const quote &item, std::size_t n)
{
	double ret = item.net_price(n);
	os << "ISBN: " << item.isbn() << " # sold: " << n << " total due: " << ret << std::endl;
	return ret;
}

int main()
{

}