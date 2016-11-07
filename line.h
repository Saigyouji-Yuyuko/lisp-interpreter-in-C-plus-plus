#pragma once
#include<memory>
#include<iostream>
#include<string>
#include<vector>
#include"environment.h"
class environment;
class line;
class line
{
public:
	line() = default;
	line(int i):line(std::to_string(i)){}
	line(const std::string&);
	line(const line& a) { *this = a; }
	line(const std::string& a, const std::string& b) :line(b) { tag = a; }
	line(const line& a, std::shared_ptr<environment> b) :line(a) { tag = "procedure"; env = std::make_shared<environment>(*b); ss.replace(ss.find("lambda"), 6, "procedure"); }
	~line();
	bool isnumber ()const;
	bool islist()const;
	bool iscons()const;
	bool issymbol()const;
	bool isjichu()const;
	bool ispro()const;
	bool isif()const;
	bool iscond()const;
	bool islet()const;
	bool isset()const;
	bool islambda()const;
	bool isdefine()const;
	bool isquote()const;
	bool isapplication()const;
	int size()const;
	bool empty()const;
	int stoi()const;
	std::string ssymbol()const;
	std::shared_ptr<environment> getenv()const;
	std::string gettag()const;
	void changeenv(environment& env1);
	const line& at(const int)const;
	line changeapplication();
	void changetag(const std::string&);
	void print(std::ostream& os = std::cout)const;
	friend bool operator<(const line&, const line&);
	friend bool operator==(const line&, const line&);
	friend std::ostream& operator<<(std::ostream&, const line&);
	void addarg(const line& p);
	void changepara(const int);
	void clear();
	//line& operator+= (const line&);
private:
	void put(std::string&);
	bool tagged(const std::string&)const;
	std::vector<std::shared_ptr<line> > part;
	std::string ss;
	std::string tag;
	std::shared_ptr<environment> env;
};

