#pragma once
#include<string>
using namespace std;
class Condition{
public:
	string type = "";//and��or
	enum symbol op;//�����
	string fieldname="";//�Ǹ��ֶ�
	string value="";
};