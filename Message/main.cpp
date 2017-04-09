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
	message(message&&);
	message& operator=(message&&);

	void save(folder &f);
	void remove(folder &f);
	void show();

	void add_folder(folder *f);
	void rem_folder(folder *f);
	void move_folders(message *m);

	friend void swap(message &lhs, message &rhs);

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

message::message(message&& m) : contents(std::move(m.contents))
{
	move_folders(&m);
}

message& message::operator=(message&& rhs)
{
	if (this != &rhs)
	{
		remove_from_folders();
		contents = std::move(rhs.contents);
		move_folders(&rhs);
	}
	return *this;
}

void message::add_folder(folder *f)
{
	folders.insert(f);
}

void message::rem_folder(folder *f)
{
	folders.erase(f);
}

void message::move_folders(message *m)
{
	folders = std::move(m->folders);

	for (auto f : folders)
	{
		f->rem_msg(m);
		f->add_msg(this);
	}

	m->folders.clear();
}

void swap(message &lhs, message &rhs)
{
	using std::swap;

	for (auto f : lhs.folders)
		f->rem_msg(&lhs);
	for (auto f : rhs.folders)
		f->rem_msg(&rhs);

	swap(lhs.folders, rhs.folders);
	swap(lhs.contents, rhs.contents);

	for (auto f : lhs.folders)
		f->add_msg(&lhs);
	for (auto f : rhs.folders)
		f->add_msg(&rhs);
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