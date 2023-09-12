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

	bool allitems = false;//��ʾ���е��ֶ�
	int depth = 0;
	void error(int n);//������ n��ʾ��������
	int statement();//���
	void add(Condition c,vector<Item>,int pos,bool*selected);
	void start();
	int stringToInt(string str);//stringת����
	string gtree = "";//�﷨��
	void buildTree(int n,string content);//�﷨���Ĺ���
	int coldeflist();//<�ֶζ����б�>
	int constantlist();//<�����б�>
	int wherec();//where�Ӿ�
	int condition();//�������ʽ
	int TableExist(string tname);//�����Ƿ����,����ֵΪ��Ԫ�ص�λ�ã���������ڣ��򷵻�-1
	void deleteTable(int pos);//ɾ��pos���ı�
	int collist();//�ֶ��б�
	int colv();//�ֶθ�ֵ�б�
	int colExist(Table t,string cname);//�ж������Ƿ����,�����е���������������ڣ��򷵻�-1
	bool MatchColAndItsType(Table t, int pos, enum symbol sym);//����������Ƿ�ƥ��
	int matchtoken(enum symbol expected, int n,int depth,string content);
	vector<string> selectItems;//�����ѡ��������
	vector<Item> insertItems;//Ҫ�������
	vector<Item> updateItems;//Ҫ���µ���
	vector<vector<Item>> records;//��ɸѡ�ļ�¼
	vector<Condition> cdts;//Ҫ���������
	Condition c;//��ǰ����
	bool inSelectedItems(Item item);//�ж��Ƿ�Ϊ��ɸѡ������
};



