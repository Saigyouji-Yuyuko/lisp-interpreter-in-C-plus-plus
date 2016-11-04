#pragma once
#include<vector>
#include<map>
#include<string>
#include<memory>
#include"line.h"
class line;
using std::string;
using std::map;
using std::shared_ptr;
class environment
{
public:
	environment():frame(1)
	{
		frame[0] = std::make_shared<map<string, line> >();
	}
	environment(const environment& a) :environment(){ *this = a; }
	environment(shared_ptr<map<string,line> >);
	shared_ptr<map<string, line> > at(int)const;
	void pop();
	int size()const;
	void insert(const string&, const line&);
	void push(shared_ptr<map<string, line> >);
	line search(const string&)const;
	shared_ptr<map<string, line>> top()const;
	void print(std::ostream &os = std::cout)const;
	~environment();
private:
	std::vector<shared_ptr<map<string, line> > > frame;
};

