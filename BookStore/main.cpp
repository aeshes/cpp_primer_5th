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
	virtual void debug() const;
	virtual ~quote() = default;

private:
	std::string bookNo;
protected:
	double price = 0.0;
};

void quote::debug() const
{
	std::cout << "in quote: ";
	std::cout << "bookNo = " << bookNo;
	std::cout << " price = " << price << std::endl;
}

class disc_quote : public quote
{
public:
	disc_quote() = default;
	disc_quote(const std::string &book, double price, std::size_t qty, double disc)
		: quote(book, price), quantity(qty), discount(disc) {}
	double net_price(std::size_t n) const = 0;

protected:
	std::size_t quantity = 0;
	double discount = 0.0;
};

class bulk_quote : public disc_quote
{
public:
	bulk_quote() = default;
	bulk_quote(const std::string &book, double p, std::size_t qty, double disc)
		: disc_quote(book, p, qty, disc) {}
	double net_price(std::size_t n) const override;
	void debug() const override;

private:
	std::size_t min_qty = 0;
};

double bulk_quote::net_price(std::size_t cnt) const
{
	if (cnt >= min_qty)
		return cnt * (1 - discount) * price;
	else
		return cnt * price;
}

void bulk_quote::debug() const
{
	std::cout << "in bulk_quote:";
	std::cout << " price = " << price;
	std::cout << "min_qty = " << min_qty << " discount = " << discount << std::endl;
}

double print_total(std::ostream &os, const quote &item, std::size_t n)
{
	double ret = item.net_price(n);
	os << "ISBN: " << item.isbn() << " # sold: " << n << " total due: " << ret << std::endl;
	return ret;
}

int main()
{
	quote base("xxx", 10);
	bulk_quote derived("xxx", 10, 1, 0.1);

	print_total(std::cout, base, 1);
	print_total(std::cout, derived, 1);

	base.debug();
	derived.debug();
}