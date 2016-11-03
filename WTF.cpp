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
	s.clear();
	stack<int> check;
	string ss;
	do
	{
		getline(is, ss);
		for (auto i : ss)
		{
			if (i == '(')
				check.push(1);
			else if (i == ')')
				check.pop();
		}
		s += ss;
	} while (!check.empty());
	//fflush(stdin);
	return is;
}
line eval_jichu(const line& s,environment& env)
{
	line first = eval(s.at(1), env);
	line second = eval(s.at(2), env);
	if (s.at(1) == line("+"))
		return line(to_string(first.stoi() + second.stoi()));
	if (s.at(1) == line("-"))
		return line(to_string(first.stoi() - second.stoi()));
	if (s.at(1) == line("*"))
		return line(to_string(first.stoi() * second.stoi()));
	if (s.at(1) == line("/"))
		return line(to_string(first.stoi() / second.stoi()));
	if (s.at(1) == line(">"))
		return line(to_string(first.stoi() > second.stoi()));
	if (s.at(1) == line("<"))
		return line(to_string(first.stoi() < second.stoi()));
	if (s.at(1) == line("="))
		return line(to_string(first.stoi() == second.stoi()));
}
void chushihua(environment& env)
{
	shared_ptr<map<string, string>> p = make_shared<map<string,string>>();

}
line eval_define(const line& s, environment& env)
{
	env.insert(s.ssymbol(), eval(s.at(2),env));
	return line("nothing", "()");
}
line eval_if(const line& s, environment& env)
{
	if (eval(s.at(1), env) == line("1"))
		return eval(s.at(2), env);
	return eval(s.at(3), env);
}
line eval_lambda(const line& s, environment& env)
{
	shared_ptr<environment> p = make_shared<environment>(env);
	return line(s, p);
}
string apply(const line& s,const line& arg,environment& env)
{
	shared_ptr<map<string,line> >p = make_shared<map<string,line> >();
	environment env2 = env;
	for(s.at(1).size()
}
line eval(const line& s,environment& env)
{
	if (s.isnumber())
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
		return apply(eval(s.at(1), env),s,env);
}
int main()
{	
	string s;
	environment env;
	//chushihua(env);
	//while (input(cin,s))
	//{
	//	line(s).print(cout);
	//}
    return 0;
}

