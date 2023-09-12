#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include"Global.h"
#define getchdo if(getch()==1) return 1;
#define getsymdo if(getsym()==1) return 1;
using namespace std;
class WordAnalyzer {
public:
	WordAnalyzer() {}
	ifstream ifs;//读源代码文件
	ofstream ofs_word;//输出词法分析结果
	ofstream ofs_error;//词法分析错误
	char ch=' ';//当前字符
	int ll=0;//line_length,行缓冲区的长度
	int cc=0;//当前字符的位置
	bool isFileEnd = false;//是否读到文件末尾
	string line="";//行缓冲区
	string numstr = "";//数字字符串
	int getch();//获取当前字符
	int getsym();//获取当前符号
	bool isSpace(char ch);//判断当前字符是否为空格或者制表符或者换行符
	bool isNumber(char ch);//判断当前字符是否为数字
	bool isLetterOrUnderline(char ch);//判断当前字符是否字母或者下划线
	bool isPossibleNote(char ch);//判断可能存在的注释符号
	bool isStr(char);//判断是否为字符串
	void error(int n);
	string strvalue = "";//字符串的值
	string temp_word="";//临时符号的值
	string now_word = "";//当前符号的值
	string now_ident = "";//当前符号的值
	string fuhao = "";//临时符号的值
	enum symbol sym;//存放当前符号的类型
	enum symbol wsym[knum];//保留字对应的符号值
	enum symbol ssym[256];//保留单符号对应的符号值
	bool statbegsys[symnum];//表示语句开始的符号集
	bool constantsys[symnum];//表示常量的符号集
	bool datatype[symnum];//表示数据类型的符号集
	bool optsys[symnum];//表示运算符的符号集
	void toLower(string &str);//将字符串转换为小写
	void init();//初始化
	int Identifier();//处理标识符
	int Number();//处理数字
	int Note();//处理注释
	int Str();//处理字符串
	int OperatorOrSeparator();//处理运算符或者分隔符
	void testOutPut(string type, string value);//测试输出
	void outputwResult(string value,string type);//输出词法分析的结果
	void outputError(int n,int lpos,int cpos);//输出词法分析出现的错误
};