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
	line l = b;
	if (l.issymbol()&&l.issymbol())
		l = search(l.ssymbol());
	auto p = frame.back();
	(*p)[a] = l;
}

void environment::push(shared_ptr<map<string, line>> a)
{
	frame.push_back(a);
}

line environment::search(const string & p) const
{
	for (auto i = frame.rbegin(); i != frame.rend();++i)
		if ((*i)->find(p) != (*i)->end())
			return (*(*i))[p];
	return line(p);
}

shared_ptr<map<string, line>> environment::top()const
{
	return frame.back();
}

void environment::print(std::ostream &os) const
{
	std::cout << "---------------" << std::endl;
	std::cout << "environment:  " << std::endl;
	for (int i = 0; i < frame.size(); ++i)
	{
		//std::cout << i << std::endl;
		auto j = frame[i]->cbegin();
		while (j != frame[i]->cend())
		{
			std::cout << j->first << "   " << j->second << std::endl;
			++j;
		}
	}
	std::cout << "---------------" << std::endl;
	return;
}

environment::~environment()
{
}
