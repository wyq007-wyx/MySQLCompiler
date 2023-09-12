#pragma once
#include<string>
using namespace std;
#define knum 22 /*�ؼ��ʸ���*/
#define symnum 100/*��������*/
#define al 10 /*���ŵ���󳤶�*/
extern int line_count;//��ǰ��������
extern int err;//���������
enum Message {
	CORRECT,//��ȷ
	FILEEND,//����δ����
	NUMBER_ERR,//���������
	MISS_CLOSEQ,//ȱ��������
	SBEGIN_ERR,//��俪ʼ����
	MISS_SEMI,//ȱ�ٷֺ�
	COL_DEF_ERR,//�ֶζ���Ŀ�ʼ����ӦΪ��ʶ��
	DTYPE_UNKNOWN,//δ֪���ֶ�����
	MISS_LPAR,//ȱ��������
	MISS_RPAR,//ȱ��������
	NUM_POS,//����Ӧ������
	KWORD_ERR,//�ؼ��ִ���
	IDENT_POS,//Ӧ�Ǳ�ʶ��
	MISS_COMMA,//ȱ�ٶ���
	TABLE_NEXISTS,//������
	SYM_UNKNOWN,//δ֪�ķ���
	CONSTANT_POS,//Ӧ�ǳ���
	OPR_POS,//Ӧ�������
	DTYPE_ERR,//�������ʹ���
	CONSTANT_NUM_ERR,//�ֶ�������ƥ��
	EQL_POS,//Ӧ�ǵȺ�
	MAX_LEN,//���ʹ���
};
enum symbol {
	nul,//δ֪�ķ���
	ident,//��ʶ��
	number,//����
	Integer,
	Double,
	splus,
	sminus,
	times,//�� 
	slash,//�� 
	mod,//������
	eql,
	neq,//������
	lss,//С��
	leq,//С�ڵ���
	gtr,//����
	geq,//���ڵ���
	lparen,//������
	rparen,//������
	semicolon,
	comma,
	strsym,//�ַ���
	createsym,
	deletesym,
	fromsym,
	selectsym,
	setsym,
	tablesym,
	updatesym,
	wheresym,
	andsym,
	orsym,
	valuessym,
	intosym,
	point,
	betweensym,
	likesym,
	dropsym,
	insertsym,
	varcharsym,
	charsym,
	datesym,
	timesym,
	doublesym,//double�ؼ���
	intsym,//int�ؼ���
};
extern string msg[100];
extern string keyword[knum];