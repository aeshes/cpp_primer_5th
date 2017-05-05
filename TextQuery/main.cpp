#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>


class QueryResult;

class TextQuery
{
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream &);
	QueryResult query(const std::string &) const;

private:
	std::shared_ptr<std::vector<std::string>> file;
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;
};

TextQuery::TextQuery(std::ifstream &is) : file(new std::vector<std::string>)
{
	std::string text;
	while (std::getline(is, text))
	{
		file->push_back(text);
		int n = file->size() - 1;
		std::istringstream line(text);
		std::string word;
		while (line >> word)
		{
			auto &lines = wm[word];
			if (!lines)
				lines.reset(new std::set<line_no>);
			lines->insert(n);
		}
	}
}

class QueryResult
{
	friend std::ostream& print(std::ostream&, const QueryResult&);
	using line_no = TextQuery::line_no;
public:
	QueryResult(std::string s,
		std::shared_ptr<std::set<line_no>> p,
		std::shared_ptr<std::vector<std::string>> f)
		: sought(s), lines(p), file(f) {}
	std::set<line_no>::iterator begin() { return lines->begin(); }
	std::set<line_no>::iterator end() { return lines->end(); }
	std::shared_ptr<std::vector<std::string>> get_file() { return file; }
private:
	std::string sought;
	std::shared_ptr<std::set<line_no>> lines;
	std::shared_ptr<std::vector<std::string>> file;
};

QueryResult TextQuery::query(const std::string &sought) const
{
	static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);
	auto loc = wm.find(sought);
	if (loc == wm.end())
		return QueryResult(sought, nodata, file);
	else
		return QueryResult(sought, loc->second, file);
}

std::ostream& print(std::ostream &os, const QueryResult &qr)
{
	os << qr.sought << " occurs " << qr.lines->size() << " times." << std::endl;
	for (auto num : *qr.lines)
		os << "\t(lines " << num + 1 << ") "
		<< *(qr.file->begin() + num) << std::endl;
	return os;
}


class QueryBase
{
	friend class Query;
protected:
	using line_no = TextQuery::line_no;
	virtual ~QueryBase() = default;
private:
	virtual QueryResult eval(const TextQuery&) const = 0;
	virtual std::string rep() const = 0;
};

class Query
{
	friend Query operator~(const Query&);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);
public:
	Query(const std::string&);
	QueryResult eval(const TextQuery &t) const { return q->eval(t); }
	std::string rep() const { return q->rep(); }
private:
	Query(std::shared_ptr<QueryBase> query) : q(query) {}
	std::shared_ptr<QueryBase> q;
};

std::ostream& operator<<(std::ostream &os, const Query query)
{
	return os << query.rep();
}

class WordQuery : public QueryBase
{
	friend class Query;
	WordQuery(const std::string &s) : query_word(s) {}
	QueryResult eval(const TextQuery &t) const { return t.query(query_word); }
	std::string rep() const { return query_word; }
	std::string query_word;
};

inline Query::Query(const std::string &s) : q(new WordQuery(s)) {}

class NotQuery : public QueryBase
{
	friend Query operator~(const Query&);
	NotQuery(const Query &q) : query(q) {}
	std::string rep() const { return "~(" + query.rep() + ")"; }
	QueryResult eval(const TextQuery&) const;
	Query query;
};

inline Query operator~(const Query &operand)
{
	return std::shared_ptr<QueryBase>(new NotQuery(operand));
}

class BinaryQuery : public QueryBase
{
protected:
	BinaryQuery(const Query &l, const Query &r, std::string s)
		: lhs(l), rhs(r), opSym(s) {}
	std::string rep() const { return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")"; }
	Query lhs, rhs;
	std::string opSym;
};

class AndQuery : public BinaryQuery
{
	friend Query operator&(const Query&, const Query&);
	AndQuery(const Query &left, const Query &right)
		: BinaryQuery(left, right, "&") {}
	QueryResult eval(const TextQuery&) const;
};

inline Query operator&(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<QueryBase>(new AndQuery(lhs, rhs));
}

class OrQuery : public BinaryQuery
{
	friend Query operator|(const Query&, const Query&);
	OrQuery(const Query &left, const Query &right)
		: BinaryQuery(left, right, "|") {}
	QueryResult eval(const TextQuery&) const;
};

inline Query operator|(const Query &lhs, const Query &rhs)
{
	return std::shared_ptr<QueryBase>(new OrQuery(lhs, rhs));
}

QueryResult OrQuery::eval(const TextQuery &text) const
{
	auto left = lhs.eval(text), right = rhs.eval(text);
	auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());
	ret_lines->insert(right.begin(), right.end());
	return QueryResult(rep(), ret_lines, left.get_file());
}


int main()
{
	
}