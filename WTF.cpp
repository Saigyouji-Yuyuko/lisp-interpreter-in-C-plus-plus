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
line apply_primitive(const line& s, const string& op)
{
	string pp = "(";
	pp += op + " ";
	pp += s.ssymbol()+")";
	return line(pp);
}
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
	if (s.size() < 2)
		throw(runtime_error("It's two little arg for" + s.at(0).ssymbol()));
	line first = eval(s.at(1), env);
	if (s.at(0).ssymbol() == "print")
	{
		cout << first << endl;
		if (s.iscons())
			eval(apply_primitive(eval(s.at(2), env), "print"), env);
		return line("nothint", "()");
	}
	if (s.at(0).ssymbol() == "number?")
	{
		if (first.isnumber())
			return line("1");
		else return line("0");
	}
	if (s.at(0).ssymbol() == "symbol?")
	{
		if (first.issymbol())
			return line("1");
		else return line("0");
	}
	if (s.at(0).ssymbol() == "pair?")
	{
		if (first.iscons())
			return line("1");
		else return line("0");
	}
	if (s.at(0).ssymbol() == "null?"|| s.at(0).ssymbol() == "atom?")
	{
		if (first.iscons())
			return line("0");
		else return line("1");
	}
	if (s.at(0).ssymbol() == "car" && first.iscons())
		return eval(first.at(1),env);
	if (s.at(0).ssymbol() == "cdr"&& first.iscons())
		return eval(first.at(2), env);
	if(s.at(0).ssymbol() == "cdr" || s.at(0).ssymbol() == "car")
		throw(runtime_error("It's a wrong that use " + s.at(0).ssymbol()+"in not a cons"));
	if(s.size() < 3)
		throw(runtime_error("It's too little arg for"+s.at(0).ssymbol()));
	line second = eval(s.at(2), env);
	//cout << first.stoi() << " " << first.stoi() << endl;
	if(!first.isnumber()||!second.isnumber())
		throw(runtime_error("This op"+s.at(0).ssymbol()+"only need number"));
	if (s.at(0).ssymbol() == "+")
		return line(to_string(first.stoi() + second.stoi()));
	if (s.at(0).ssymbol() == "%")
		return line(to_string(first.stoi() % second.stoi()));
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
	else
		throw(runtime_error("It's wrong of the writer"));
	
}
line make_lambda(const line& s)
{
	line pp(s.at(1).at(1));
	string p = "(lambda ";
	for (int i = 2; i < s.at(1).size(); ++i)
		pp.addarg(s.at(1).at(i));
	if(pp.issymbol())
		p +="("+pp.ssymbol()+")";
	else 
		p += pp.ssymbol();
	p += s.at(2).ssymbol();
	p += ")";
	return line(p);
}
line eval_define(const line& s, environment& env)
{
	//cout << eval(s.at(2), env) << endl;
	if (s.at(1).isapplication())
	{
		env.insert(s.at(1).at(0).ssymbol(), eval(make_lambda(s),env));
	}
		
	else
	{
		//cout << s.at(1) << endl << s.at(2);
		env.insert(s.at(1).ssymbol(), eval(s.at(2), env));
	}
		
	return line("nothing", "()");
}
line eval_cons(const line& s, environment& env)
{
	string sa= "(cons ";
	line p1 = eval(s.at(1),env);
	line p2 = eval(s.at(2),env);
	sa += p1.ssymbol() +" "+ p2.ssymbol() + ")";
	return sa;
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
		return a.size() - b.size();
	}
	else
		throw(runtime_error("too more args"+a.ssymbol()+"  "+b.ssymbol()));
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
/*line cond_to_if(const line& s)
{
	string p = "(if ";
		for()
}*/
line eval_set(const line& s, environment env)
{
	if (env.search(s.at(1).ssymbol()) == s.at(1).ssymbol())
		throw(runtime_error("Can not find " + s.at(1).ssymbol() + "in environment"));
	return eval_define(s, env);
}
line apply(const line& s,const line& arg)
{
	if (!s.ispro())
		throw(runtime_error("it is not a procedure    " + s.ssymbol()));
	shared_ptr<environment> p = make_shared<environment>(*(s.getenv()));
	shared_ptr<map<string, line> > l = make_shared<map<string,line> >();
	int k = bind(s.at(1), arg, l);
	//cout << k << endl;
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
line eval(const line& s, environment& env)
{
	//cout << s << endl;
	if (s.isnumber() || s.ispro())
		return s;
	else if (s.iscons())
		return eval_cons(s, env);
	else if (s.isquote())
		return s.ssymbol();
	else if (s.issymbol())
		return env.search(s.ssymbol());
	else if (s.isjichu())
		return eval_jichu(s, env);
	else if (s.isdefine())
		return eval_define(s, env);
	else if (s.isset())
		return eval_set(s, env);
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
	environment env2;
	//chushihua(env);
	//line("(+)").print();
	while (input(cin,s))
	{
		env2 = env;
		try
		{
			//cout << s << endl;
			line p(s);
			//p.print();
			cout << endl << eval(p, env) << endl;
			env.print();
			
		}
		catch (runtime_error err)
		{
			cerr << err.what() << endl;
			env = env2;
			continue;
		}
	}
    return 0;
}