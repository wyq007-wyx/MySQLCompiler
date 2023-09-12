#pragma once
#include<string>
using namespace std;
#define knum 22 /*关键词个数*/
#define symnum 100/*符号数量*/
#define al 10 /*符号的最大长度*/
extern int line_count;//当前代码行数
extern int err;//编译错误数
enum Message {
	CORRECT,//正确
	FILEEND,//程序未结束
	NUMBER_ERR,//错误的数字
	MISS_CLOSEQ,//缺少右引号
	SBEGIN_ERR,//语句开始错误
	MISS_SEMI,//缺少分号
	COL_DEF_ERR,//字段定义的开始符号应为标识符
	DTYPE_UNKNOWN,//未知的字段类型
	MISS_LPAR,//缺少左括号
	MISS_RPAR,//缺少右括号
	NUM_POS,//符号应是整数
	KWORD_ERR,//关键字错误
	IDENT_POS,//应是标识符
	MISS_COMMA,//缺少逗号
	TABLE_NEXISTS,//表不存在
	SYM_UNKNOWN,//未知的符号
	CONSTANT_POS,//应是常量
	OPR_POS,//应是运算符
	DTYPE_ERR,//数据类型错误
	CONSTANT_NUM_ERR,//字段数量不匹配
	EQL_POS,//应是等号
	MAX_LEN,//单词过长
};
enum symbol {
	nul,//未知的符号
	ident,//标识符
	number,//数字
	Integer,
	Double,
	splus,
	sminus,
	times,//乘 
	slash,//除 
	mod,//求余数
	eql,
	neq,//不等于
	lss,//小于
	leq,//小于等于
	gtr,//大于
	geq,//大于等于
	lparen,//左括号
	rparen,//右括号
	semicolon,
	comma,
	strsym,//字符串
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
	doublesym,//double关键字
	intsym,//int关键字
};
extern string msg[100];
extern string keyword[knum];