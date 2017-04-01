#include <iostream>
#include <string>
#include <set>

class folder;

class message
{
	friend class folder;

public:
	explicit message(const std::string &str = "") : contents(str) {}
	message(const message &msg);
	message& operator=(const message &msg);
	~message();

	void save(folder &f);
	void remove(folder &f);
	void show();

private:
	std::string contents;
	std::set<folder *> folders;

	void add_to_folders(const message &msg);
	void remove_from_folders();
};

class folder
{
	friend class message;

public:
	folder() {}
	folder(const folder &f);

	void add_msg(message *msg);
	void rem_msg(message *msg);
	void show();

private:
	std::set<message *> messages;
};


void message::save(folder &f)
{
	folders.insert(&f);
	f.add_msg(this);
}

void message::remove(folder &f)
{
	folders.erase(&f);
	f.rem_msg(this);
}

void message::show()
{
	std::cout << contents << std::endl;
}

void message::add_to_folders(const message &msg)
{
	for (auto f : msg.folders)
		f->add_msg(this);
}

void message::remove_from_folders()
{
	for (auto f : folders)
		f->rem_msg(this);
}

message::message(const message &msg)
	: contents(msg.contents), folders(msg.folders)
{
	add_to_folders(msg);
}

message& message::operator=(const message &rhs)
{
	remove_from_folders();

	contents = rhs.contents;
	folders = rhs.folders;

	add_to_folders(rhs);
	return *this;
}

message::~message()
{
	remove_from_folders();
}

void folder::add_msg(message *msg)
{
	messages.insert(msg);
}

void folder::rem_msg(message *msg)
{
	messages.erase(msg);
}

void folder::show()
{
	for (auto msg : messages)
		msg->show();
}


int main()
{
	folder incoming;

	message m1("aaa"), m2("bbb"), m3("ccc");
	m1.save(incoming);
	m2.save(incoming);
	m3.save(incoming);

	incoming.show();

	m1.remove(incoming);

	incoming.show();
}