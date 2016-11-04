// WTF.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include<iostream>
#include<stack>
#include<cstdio>
#include<algorithm>
#include<numeric>
#include<string>
#include<map>
#include"line.h"
#include"environment.h"
using namespace std;
line eval(const line&, environment&);
istream& input(istream& is, string& s)
{
	fflush(stdin);
	//cout << "?????????????????????????????" << endl;
	s.clear();
	stack<int> check;
	string ss;
	do
	{
		getline(is, ss);
		if (ss.empty())
			continue;
		for (auto i : ss)
		{
			if (i == '(')
				check.push(1);
			else if (i == ')')
				check.pop();
		}
		if(!s.empty())
			s += " ";
		s += ss;
	} while (!check.empty());
	//fflush(stdin);
	return is;
}
line eval_jichu(const line& s,environment& env)
{
	line first = eval(s.at(1), env);
	line second = eval(s.at(2), env);
	cout << first << endl << second << endl;
	if (s.at(0).ssymbol() == "+")
		return line(to_string(first.stoi() + second.stoi()));		
	if (s.at(0).ssymbol() == "-")
		return line(to_string(first.stoi() - second.stoi()));
	if (s.at(0).ssymbol() == "*")
		return line(to_string(first.stoi() * second.stoi()));
	if (s.at(0).ssymbol() == "/")
		return line(to_string(first.stoi() / second.stoi()));
	if (s.at(0).ssymbol() == ">")
		return line(to_string(first.stoi() > second.stoi()));
	if (s.at(0).ssymbol() == "<")
		return line(to_string(first.stoi() < second.stoi()));
	if (s.at(0).ssymbol() == "=")
		return line(to_string(first.stoi() == second.stoi()));
}
line eval_define(const line& s, environment& env)
{
	//cout << eval(s.at(2), env) << endl;
	env.insert(s.at(1).ssymbol(), eval(s.at(2),env));
	return line("nothing", "()");
}
line eval_if(const line& s, environment& env)
{
	if (eval(s.at(1), env).stoi() == 1)
		return eval(s.at(2), env);
	return eval(s.at(3), env);
}
line eval_lambda(const line& s, environment& env)
{
	shared_ptr<environment> p = make_shared<environment>(env);
	return line(s, p);
}
line argument(const line& s, environment& env)
{
	line p(eval(s.at(1),env));
	for (int i = 2; i < s.size(); ++i)
		p.addarg(eval(s.at(i),env));
	if (!p.isapplication())
		return line("(" + p.ssymbol() + ")");
	return p;
}
int bind(const line& a,const line& b, shared_ptr<map<string, line> >& p)
{
	//cout << a << endl << b << endl;
	if (a.size() == b.size())
	{
		for (int i = 0; i < a.size(); ++i)
			(*p)[a.at(i).ssymbol()] = b.at(i);
		return 0;
	}
	else if (a.size() > b.size())
	{
		for (int i = 0; i < b.size(); ++i)
			(*p)[a.at(i).ssymbol()] = b.at(i);
		return 1;
	}
	else
		return a.size() - b.size();
}
line list_eval(const line& s, environment& env)
{
	//cout << s << endl;
	//env.print();
	for (auto i = 0; i < s.size() - 1; ++i)
		eval(s.at(i), env);
	//cout << eval(s.size() - 1, env) << endl;
	return eval(s.at(s.size() - 1), env);
}
line apply(const line& s,const line& arg)
{
	shared_ptr<environment> p = make_shared<environment>(*(s.getenv()));
	shared_ptr<map<string, line> > l = make_shared<map<string,line> >();
	int k = bind(s.at(1), arg, l);
	//cout << k << endl;
	if (k == -2)
		return line("error!");
	else
	{
		p->push(l);
		//p->print();
		if (k == 0)
			return list_eval(s.at(2), *p);
		else
		{
			line pp = line(s,p);
			//cout << "pp"<< endl;
			pp.changepara(-k);
			//cout << "dd" << endl;
			//cout << pp.ssymbol() << endl;
			return pp;
		}
	}
	return line();
}
line eval(const line& s, environment& env)
{
	//cout << s << endl;
	if (s.isnumber()||s.ispro())
		return s;
	else if (s.issymbol())
		return env.search(s.ssymbol());
	else if (s.isjichu())
		return eval_jichu(s, env);
	else if (s.isdefine())
		return eval_define(s, env);
	else if (s.isif())
		return eval_if(s, env);
	else if (s.islambda())
		return eval_lambda(s, env);
	else
		return apply(eval(s.at(0), env),argument(s,env));
}
int main()
{	
	string s;
	environment env;
	//chushihua(env);
	//line("(+)").print();
	while (input(cin,s))
	{
		//cout << s << endl;
		line p(s);
		//p.print();
		cout << endl << eval(p, env) << endl;
		//env.print();
	}
    return 0;
}