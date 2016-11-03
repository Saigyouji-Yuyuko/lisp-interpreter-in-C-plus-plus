#pragma once

#include<string>
#include<vector>
#include"environment.h"
class environment;
class line
{
public:
	line() = default;
	line(const std::string&);
	line(const line& a) { *this = a; }
	line(const std::string& a, const std::string& b) :line(b) { tag = a; }
	line(const line& a, shared_ptr<environment> b) :line(a) { tag = "procedure"; env = b; }
	~line();
	bool isnumber ()const;
	bool issymbol()const;
	bool isjichu()const;
	bool ispro()const;
	bool isif()const;
	bool islambda()const;
	bool isdefine()const;
	bool isapplication()const;
	int size()const;
	bool empty()const;
	int stoi()const;
	std::string ssymbol()const;
	const line& at(const int)const;
	void print(std::ostream&);
	friend bool operator<(const line&, const line&);
	friend bool operator==(const line&, const line&);
private:
	void put(std::string&);
	bool tagged(const std::string&)const;
	std::vector<line> part;
	std::string ss;
	std::string tag;
	std::shared_ptr<environment> env;
};
