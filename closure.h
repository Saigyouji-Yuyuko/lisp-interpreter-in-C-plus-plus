#pragma once
#include<string>
#include<vector>
#include<stack>
#include<map>
#include"environment.h"
class closure
{
public:
	closure() = default;
	~closure();
private:
	std::vector<std::string>paragramt;
	std::string produce;
	environment env;
};

