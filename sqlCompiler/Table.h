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
	string tablename;//表名
	vector <Item> items;//字段
	int itemnums=0;//字段总数
};