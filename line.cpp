#include "line.h"
#include<stack>
#include<iostream>
const string primer[] = { "+" ,"-" ,"*" ,"/" ,"%" ,"=" ,">" ,"<" ,"print" ,"car","cdr","null?","number?","symbol?","pair?","atom?","read","string?","eval","apply"};
bool isjiichu(const string& s)
{
	for(auto i:primer)
		if(s == i)
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
		//std::cout << sp << std::endl;
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
		ss = sp;
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
	//std::cout << tag << std::endl << ss << std::endl;
}

line::~line()
{
}

std::shared_ptr<environment> line::getenv() const
{
	return env;
}

std::string line::gettag() const
{
	return tag;
}

void line::changeenv(environment & env1) 
{
	env = std::make_shared<environment>(env1);
	return;
}

bool line::isnumber() const
{
	return tagged("number");
}

bool line::islist() const
{
	return tagged("list");
}

bool line::iscons() const
{
	return tagged("cons");
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

bool line::iscond() const
{
	return tagged("cond");
}

bool line::islet() const
{
	return tagged("let");
}

bool line::isset() const
{
	return tagged("set!");
}

bool line::islambda() const
{
	return tagged("lambda");
}

bool line::isdefine() const
{
	return tagged("define");
}

bool line::isquote() const
{
	return tagged("quote");
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
	return *part[i];
}

line line::changeapplication()
{
	if (this->isapplication())
		return line(*this);
	else
	{
		line temp;
		temp.ss = "(" + this->ss + ")";
		temp.part.push_back(std::make_shared<line>(*this));
		temp.tag = "application";
		return temp;
	}
}

void line::changetag(const std::string & s)
{
	tag = s;
}

void line::print(std::ostream & os)const
{
	using std::endl;
	os << tag <<":"<< endl;
	os << ss << endl;
	for (int i = 0; i < part.size(); ++i)
		part[i]->print(os);
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
		this->ss.replace(ss.size()-1,1," "+p.ss+")");
		this->part.push_back(std::make_shared<line>(p));
		this->tag = "application";
	}
	else
	{
		line temp = *this;
		this->ss.clear();
		this->part.clear();
		this->ss = "(" + temp.ss + " " + p.ss + ")";
		this->part.push_back(std::make_shared<line>(temp));
		this->part.push_back(std::make_shared<line>(p));
		this->tag = "application";
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

void line::clear()
{
	ss.clear();
	part.clear();
	env = nullptr;
	tag.clear();
}

inline void line::put(std::string&  s)
{
	if (s.empty())
		return;
	
	if (this->empty())
	{
		//std::cout << s << std::endl;
		if (s == "lambda")
			tag = "lambda";
		else if (s == "procedure")
			tag = "procedure";
		else if (isjiichu(s))
			tag = "jichu";
		else if (s == "cond")
			tag = "cond";
		else if (s == "set!")
			tag = "set!";
		else if (s == "list")
			tag = "list";
		else if (s == "define")
			tag = "define";
		else if (s == "define-syntax")
			tag = "define-syntax";
		else if (s == "call/cc")
			tag = "call/cc";
		else if (s == "if")
			tag = "if";
		else if (s == "quote")
			tag = "quote";
		else if (s == "cons")
			tag = "cons";
		else if (s == "let")
			tag = "let";
		else
			tag = "application";
		part.push_back(std::make_shared<line>(s));
		s.clear();
		return;
	}
	part.push_back(std::make_shared<line>(s));
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
	os << s.tag << "    ";
	os << s.ss << std::endl;
	//if (s.ispro())
		//s.env->print();
	return os;
	if (s.isnumber())
		os << s.stoi();
	else if (s.issymbol())
		os << s.ssymbol();
	else
		os << "ok";
	return os;
}