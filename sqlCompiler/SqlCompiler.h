#pragma once
#include"WordAnalyzer.h"
#include"Table.h"
#include<vector>
#include<cstdio>
#include<string>
#include<sstream>
#include"Condition.h"
#define getsymdoo  if(wtool.getsym()==1) return 1;
#define coldeflistdo if(-1==coldeflist()) return -1;
#define collistdo     if(-1==collist()) return -1;
//#define matchtokendo(a,b,c,d)  if(matchtoken(a,b,c,d)) return -1;
#define wheredo       if(-1==wherec()) return -1;
#define conditiondo   if(-1==condition()) return -1;
#define constantlistdo    if(-1==constantlist()) return -1;
#define colvdo            if(-1==colv()) return -1;
class SqlCompiler {
public:
	Table t_table;
	vector<Table> tables;
	WordAnalyzer wtool;
	ofstream out;
	ifstream in;

	bool allitems = false;//表示所有的字段
	int depth = 0;
	void error(int n);//错误处理 n表示错误类型
	int statement();//语句
	void add(Condition c,vector<Item>,int pos,bool*selected);
	void start();
	int stringToInt(string str);//string转整数
	string gtree = "";//语法树
	void buildTree(int n,string content);//语法树的构建
	int coldeflist();//<字段定义列表>
	int constantlist();//<常量列表>
	int wherec();//where从句
	int condition();//条件表达式
	int TableExist(string tname);//检查表是否存在,返回值为表元素的位置，如果不存在，则返回-1
	void deleteTable(int pos);//删除pos处的表
	int collist();//字段列表
	int colv();//字段赋值列表
	int colExist(Table t,string cname);//判断列名是否存在,返回列的索引，如果不存在，则返回-1
	bool MatchColAndItsType(Table t, int pos, enum symbol sym);//检查列类型是否匹配
	int matchtoken(enum symbol expected, int n,int depth,string content);
	vector<string> selectItems;//存放挑选出来的列
	vector<Item> insertItems;//要插入的列
	vector<Item> updateItems;//要更新的列
	vector<vector<Item>> records;//被筛选的记录
	vector<Condition> cdts;//要满足的条件
	Condition c;//当前条件
	bool inSelectedItems(Item item);//判断是否为被筛选出的列
};



