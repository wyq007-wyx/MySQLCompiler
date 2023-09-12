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
	ifstream ifs;//��Դ�����ļ�
	ofstream ofs_word;//����ʷ��������
	ofstream ofs_error;//�ʷ���������
	char ch=' ';//��ǰ�ַ�
	int ll=0;//line_length,�л������ĳ���
	int cc=0;//��ǰ�ַ���λ��
	bool isFileEnd = false;//�Ƿ�����ļ�ĩβ
	string line="";//�л�����
	string numstr = "";//�����ַ���
	int getch();//��ȡ��ǰ�ַ�
	int getsym();//��ȡ��ǰ����
	bool isSpace(char ch);//�жϵ�ǰ�ַ��Ƿ�Ϊ�ո�����Ʊ�����߻��з�
	bool isNumber(char ch);//�жϵ�ǰ�ַ��Ƿ�Ϊ����
	bool isLetterOrUnderline(char ch);//�жϵ�ǰ�ַ��Ƿ���ĸ�����»���
	bool isPossibleNote(char ch);//�жϿ��ܴ��ڵ�ע�ͷ���
	bool isStr(char);//�ж��Ƿ�Ϊ�ַ���
	void error(int n);
	string strvalue = "";//�ַ�����ֵ
	string temp_word="";//��ʱ���ŵ�ֵ
	string now_word = "";//��ǰ���ŵ�ֵ
	string now_ident = "";//��ǰ���ŵ�ֵ
	string fuhao = "";//��ʱ���ŵ�ֵ
	enum symbol sym;//��ŵ�ǰ���ŵ�����
	enum symbol wsym[knum];//�����ֶ�Ӧ�ķ���ֵ
	enum symbol ssym[256];//���������Ŷ�Ӧ�ķ���ֵ
	bool statbegsys[symnum];//��ʾ��俪ʼ�ķ��ż�
	bool constantsys[symnum];//��ʾ�����ķ��ż�
	bool datatype[symnum];//��ʾ�������͵ķ��ż�
	bool optsys[symnum];//��ʾ������ķ��ż�
	void toLower(string &str);//���ַ���ת��ΪСд
	void init();//��ʼ��
	int Identifier();//�����ʶ��
	int Number();//��������
	int Note();//����ע��
	int Str();//�����ַ���
	int OperatorOrSeparator();//������������߷ָ���
	void testOutPut(string type, string value);//�������
	void outputwResult(string value,string type);//����ʷ������Ľ��
	void outputError(int n,int lpos,int cpos);//����ʷ��������ֵĴ���
};