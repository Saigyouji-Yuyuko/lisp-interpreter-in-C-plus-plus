#include "line.h"
#include<stack>
#include<iostream>
bool isjiichu(const string& s)
{
	if (s == "+" || s == "-" || s == "*" || s == "/" || s == "=" || s == ">" || s == "<")
		return true;
	return false;
}

line::line(const std::string & s)
{
	//if(s.empty())
	//std::cout << s << std::endl;
	ss = s;
	using std::string;
	string sp;
	bool flag = false;
	if (ss[0] != '(')
		 sp =ss;
	else
	{
		sp=string(s.begin() + 1, s.end() - 1);
		flag = true;
	} 
	//std::cout << sp << std::endl;
	if (!flag)
	{

		for(auto i :sp)
			if (!isdigit(i))
			{
				flag = true;
				break;
			}
		if (flag)
			tag = "symbol";
		else
			tag = "number";
	}
	else
	{
		string temp;
		std::stack<int> check;
		for (int i = 0; i < sp.size(); ++i)
		{
			if (check.empty())
			{
				if (sp[i] == ' ')
					put(temp);
				else if (sp[i] == '(')
				{
					put(temp);
					temp = "(";
					check.push(1);
				}
				else
					temp += sp[i];
			}
			else
			{ 
				temp += sp[i];
				if (sp[i] == '(')
					check.push(1);
				else if (sp[i] == ')')
				{
					check.pop();
					if (check.empty())
						put(temp);
				}
			}
		}
		put(temp);
	}
}

line::~line()
{
}

std::shared_ptr<environment> line::getenv() const
{
	return env;
}

bool line::isnumber() const
{
	return tagged("number");
}

bool line::issymbol() const
{
	return tagged("symbol");
}

bool line::isjichu() const
{
	return tagged("jichu");
}

bool line::ispro() const
{
	return tagged("procedure");
}

bool line::isif() const
{
	return tagged("if");
}

bool line::islambda() const
{
	return tagged("lambda");
}

bool line::isdefine() const
{
	return tagged("define");
}

bool line::isapplication() const
{
	return tagged("application");
}

int line::size() const
{
	return part.size();
}

bool line::empty() const
{
	return part.size() == 0;
}

int line::stoi() const
{
	return std::stoi(ss);
}

std::string line::ssymbol() const
{
	return std::string(ss);
}

const line & line::at(const int i) const
{
	return part[i];
}

void line::print(std::ostream & os)const
{
	using std::endl;
	os << tag <<":"<< endl;
	os << ss << endl;
	for (int i = 0; i < part.size(); ++i)
		part[i].print(os);
}

void line::addarg(const line& p)
{
	using std::string;
	if (p.ss.empty())
		return;
	else if (this->ss.empty())
		*this = p;
	else if (this->isapplication())
	{
		string pp(this->ss.begin(), this->ss.end() - 1);
		pp += ' ';
		pp += p.ss;
		pp += ')';
		*this=line(pp);
	}
	else
	{
		string pp = "(";
		pp += this->ss;
		pp += ' ';
		pp += p.ss;
		pp += ')';
		*this=line(pp);
	}
}

void line::changepara(const int k)
{
	line p = this->at(1);
	int l = p.size() + k;
	std::cout << k<<std::endl<<l << std::endl;
	line ll(p.at(l));
	for (int i = l + 1; i < p.size(); ++i)
		ll.addarg(p.at(i));
	std::string pl = "("+this->at(0).ssymbol()+" ";
	if (ll.isapplication())
		pl += ll.ssymbol();
	else
		pl += "(" + ll.ssymbol() + ")";
	for (auto i = 2; i < this->size(); ++i)
		pl += " " + this->at(i).ssymbol();
	pl += ")";
	*this = line(pl, this->env);
	return;
}

inline void line::put(std::string&  s)
{
	if (s.empty())
		return;
	
	if (part.empty())
	{
		//std::cout << s << std::endl;
		if (s == "lambda")
			tag = "lambda";
		else if (s == "procedure")
			tag = "procedure";
		else if (isjiichu(s))
			tag = "jichu";
		else if (s == "define")
			tag = "define";
		else if (s == "if")
			tag = "if";
		else if (s == "cons")
			tag = "cons";
		else
			tag = "application";
		part.push_back(line(s));
		s.clear();
		return;
	}
	part.push_back(line(s));
	//std::cout << s << std::endl;
	s.clear();
}

bool line::tagged(const std::string&  s)const
{
	return tag == s;
}

bool operator<(const line &a, const line &b)
{
	return a.ss<b.ss;
}

bool operator==(const line &a, const line &b)
{
	return a.ss == b.ss;
}

std::ostream& operator<<(std::ostream& os, const line& s)
{
	//os << s.tag << std::endl;
	//os << s.ss << std::endl;
	//if (s.ispro())
		//s.env->print();
	//return os;
	if (s.isnumber())
		os << s.stoi();
	else if (s.issymbol())
		os << s.ssymbol();
	else
		os << "ok";
	return os;
}
