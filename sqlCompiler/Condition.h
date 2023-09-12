#pragma once
#include<string>
using namespace std;
class Condition{
public:
	string type = "";//and或or
	enum symbol op;//运算符
	string fieldname="";//那个字段
	string value="";
};