#pragma once
#include"Item.h"
#include<vector>
class Table {
public:
	Table() {
		tablename = "";
		items.clear();
		itemnums = 0;
	}
	string tablename;//����
	vector <Item> items;//�ֶ�
	int itemnums=0;//�ֶ�����
};