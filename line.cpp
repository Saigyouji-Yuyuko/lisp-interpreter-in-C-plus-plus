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
		
	ss = s;
	using std::string;
	string sp(s.begin() + 1, s.end() - 1);
	//std::cout << sp << std::endl;
	bool flag = false;
	for(auto i: sp)
		if (i == ' ' || i == '(')
		{
			flag = true;
			break;
		}
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
	return std::stoi(std::string(ss.begin()+1,ss.end()-1));
}

std::string line::ssymbol() const
{
	std::string p(ss.begin() + 1, ss.end() - 1);
	return p;
}

const line & line::at(const int i) const
{
	return part[i];
}

void line::print(std::ostream & os)
{
	using std::endl;
	os << tag <<":"<< endl;
	os << ss << endl;
	for (int i = 0; i < part.size(); ++i)
		part[i].print(os);
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
		else if (s == "clause")
			tag = "clause";
		else
			tag = "application";
		std::string temp = s;
		if (s[0] != '(')
			temp = '(' + temp + ')';
		part.push_back(line(temp));
		s.clear();
		return;
	}
	std::string temp = s;
	if (s[0] != '(')
		temp = '(' + temp + ')';
	part.push_back(line(temp)); 
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
