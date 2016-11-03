#include "environment.h"

environment::environment(shared_ptr<map<string, line>> p)
{
	frame.push_back(p);
}

shared_ptr<map<string, line>> environment::at(int i) const
{
	return frame[i];
}

void environment::pop()
{
	frame.erase(frame.begin()+frame.size()-1);
}

int environment::size() const
{
	return frame.size();
}

void environment::insert(const string &a, const line &b)
{
	auto p = frame.back();
	(*p)[a] = b;
}

void environment::push(shared_ptr<map<string, line>> a)
{
	frame.push_back(a);
}

line environment::search(const string & p) const
{
	for (auto i : frame)
		if ((*i).find(p) != (*i).end())
			return (*i)[p];
	return line("(error)");
}

shared_ptr<map<string, line>> environment::top()const
{
	return frame.back();
}

environment::~environment()
{
}
