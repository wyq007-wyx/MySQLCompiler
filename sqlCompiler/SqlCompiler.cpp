#include"SqlCompiler.h"


void SqlCompiler::start() {
	
	string filename;
	cout << "输入要分析的文件名:";
	cin >> filename;
	wtool.ifs.open(filename, ios::in);
	wtool.ofs_error.open("word_error.txt", ios::out);
	wtool.ofs_word.open("word_result.txt", ios::out);
	if (!wtool.ifs.is_open()) {
		cout<<"文件打开失败"<<endl;
		return;
	}
	//初始化
	wtool.init();
	wtool.ch = ' ';
	wtool.cc = wtool.ll = 0;
	buildTree(depth, "<程序>");
	int result = statement();
	if (result!=-1) {
		//输出语法树
		if (err == 0 ) {
			if (wtool.fuhao == ";") {
				cout << "程序正确" << endl;
				cout << "---语法树---" << endl;
				cout << gtree;
			}
			else {
				cout << "程序未结束" << endl;
			}
			
		}
		else {
			cout << "词法分析错误" << endl;
		}
	}
	else {
		cout << "sql程序出现错误" << endl;
	}
	wtool.ofs_error.close();
	wtool.ofs_word.close();
	wtool.ifs.close();
}
int SqlCompiler::statement() {
	depth++;
	enum symbol &sym = wtool.sym;
	//getsymdo;
	do {
		getsymdoo;
		switch (sym) {
			case createsym: {
				buildTree(depth, "create");//create
				getsymdoo;//获取下一个符号
				if (sym == tablesym) {
					buildTree(depth, "table");//create table
					getsymdoo;
					t_table = Table();
					if (sym == ident) {//create table xxx
						buildTree(depth, "<标识符>");
						buildTree(depth+1, wtool.now_ident);
						t_table.tablename = wtool.now_ident;
						getsymdoo;
						if (sym == lparen) {//create table xxx(
							buildTree(depth, "(");
							buildTree(depth, "<字段定义列表>");
							coldeflistdo;//递归下降
							if (sym != rparen) {
								error(MISS_RPAR);
								err++;
								return -1;
							}
							else {
								string filename = t_table.tablename+".txt";
								out.open(filename, ios::app);
								buildTree(depth, ")");//create table xxx(<字段定义列表>)
								out.close();
								getsymdoo;
							}
						}
						else {
							error(MISS_LPAR);
							err++;
							return -1;
						}
					}
					else {
						err++;
						error(IDENT_POS);
						return -1;
					}
				}
				else {
					error(KWORD_ERR);
					err++;
					return -1;
				}
				break;
			}
			case dropsym: {
				buildTree(depth, "drop");//drop
				getsymdoo;
				if (sym == tablesym) {
					buildTree(depth, wtool.now_word);
					getsymdoo;
					if (sym == ident) {
						buildTree(depth, wtool.now_ident);
						int pos = TableExist(wtool.now_ident);
						if (pos != -1) {
							deleteTable(pos);
							string filename = wtool.now_ident+".txt";
							cout << "---删除表结果---" << endl;
							if (remove(filename.c_str())==0) {
								cout << "删除成功" << endl;
							}
							else {
								cout << "删除失败" << endl;
							}
							cout << "----------------" << endl;
						}
						/*else {
							err++;
							error(TABLE_NEXISTS);
							return -1;
						}*/
						getsymdoo;
					}
					else {
						error(IDENT_POS);//应是标识符
					}
				}
				else {
					error(KWORD_ERR);
					err++;
					return -1;
				}
				break; 
			}
			case selectsym: {
				buildTree(depth , wtool.now_word);
				records.clear();
				buildTree(depth, "<字段列表>");
				collistdo;//<字段列表>
				if (sym == fromsym) {
					buildTree(depth,wtool.now_word);
					getsymdoo;
					if (sym == ident) {//表名
						buildTree(depth, "<标识符>");
						buildTree(depth+1, wtool.now_ident);
						int pos = TableExist(wtool.now_ident);
						if (pos != -1) {
							t_table=tables[pos];//获得表
							if (allitems) {
								for (int i = 0; i < t_table.items.size(); i++) {
									selectItems.push_back(t_table.items[i].fieldname);
								}
							}
							string filename = wtool.now_ident + ".txt";
							in.open(filename, ios::in);//把所有记录从文件中读出来
							string str, line;
							while (getline(in, line))
							{
								istringstream stream(line);
								vector<Item> onerecord;
								int i = 0;
								while (stream >> str) {
									Item item;
									item.fieldname = t_table.items[i].fieldname;
									item.fieldtype = t_table.items[i].fieldtype;
									item.fieldvalue = str;
									item.length = t_table.items[i].length;
									onerecord.push_back(item);
									i++;
								}
								records.push_back(onerecord);
							}
							in.close();
						}
						else {
							t_table = Table();//获得空表
						}
						getsymdoo;
						wheredo;
						if (t_table.itemnums != 0) {
							bool selected[100] = { 0 };
							for (int i = 0; i < cdts.size(); i++) {
								if (cdts[i].type == "and") {
									for (int j = 0; j < records.size(); j++) {
										vector<Item> im = records[j];
										if (i == 0) {
											add(cdts[i], im, j, selected);
										}
										else {
											if(selected[j]==true) add(cdts[i], im, j, selected);
										}
									}
								}
								else {
									for (int j = 0; j < records.size(); j++) {
										vector<Item> im = records[j];
										if(selected[j]==false) add(cdts[i],im,j,selected);
									}
								}
							}
							cout << "---select语句结果---" << endl;
							for (int i = 0; i < records.size(); i++) {
								if (selected[i] == true) {
									for (int j = 0; j < records[i].size(); j++) {
										if (inSelectedItems(records[i][j])) {
											cout << records[i][j].fieldvalue << " ";
										}
									}
									cout << endl;
								}
							}
							cout << "---------------------" << endl;
						}
					}
					else {
						err++;
						error(IDENT_POS);
						return -1;
					}
				}
				else {
						err++;
						error(KWORD_ERR);
						return -1;
				}
				break; 
			}
			case insertsym: {
				records.clear();
				buildTree(depth, wtool.now_word);
				getsymdoo;
				if (sym == intosym) {
					buildTree(depth, wtool.now_word);
					getsymdoo;
					if (sym == ident) {
						buildTree(depth, wtool.now_ident);
						int pos = TableExist(wtool.now_ident);
						if (pos != -1) {
							t_table = tables[pos];
						}
						else {
							t_table = Table();
						}
						getsymdoo;
						if (sym == valuessym) {
							buildTree(depth, wtool.now_word);
							getsymdoo;
							if (sym == lparen) {
								buildTree(depth, "(");
								buildTree(depth, "<常量列表>");
								constantlistdo;
								if (sym == rparen) {
									buildTree(depth, ")");
									if (t_table.itemnums != 0) {
										string filename = t_table.tablename + ".txt";
										out.open(filename, ios::app);
										for (int i = 0; i < insertItems.size(); i++) {
											if(i!=insertItems.size()-1)out << insertItems[i].fieldvalue << " ";
											else out << insertItems[i].fieldvalue << endl;
										}
										out.close();
									}
									getsymdoo;
								}
								else {
									err++;
									error(KWORD_ERR);
									return -1;
								}
							}
							else {
								err++;
								error(KWORD_ERR);
								return -1;
							}
						}else {
							err++;
							error(KWORD_ERR);
							return -1;
						}
					}
					else {
						err++;
						error(IDENT_POS);
						return -1;
					}
				}
				else {
					err++;
					error(KWORD_ERR);
					return -1;
				}
				break;
			}
			case updatesym: {
				records.clear();
				buildTree(depth, wtool.now_word);
				getsymdoo;
				if (sym == ident) {
					buildTree(depth, wtool.now_ident);
					int pos = TableExist(wtool.now_ident);
					if (pos != -1) {
						t_table = tables[pos];//获得表
						string filename = wtool.now_ident + ".txt";
						in.open(filename, ios::in);//把所有记录从文件中读出来
						string str, line;
						while (getline(in, line))
						{
							istringstream stream(line);
							vector<Item> onerecord;
							int i = 0;
							while (stream >> str) {
								Item item;
								item.fieldname = t_table.items[i].fieldname;
								item.fieldtype = t_table.items[i].fieldtype;
								item.fieldvalue = str;
								item.length = t_table.items[i].length;
								onerecord.push_back(item);
								i++;
							}
							records.push_back(onerecord);
						}
						in.close();
					}
					else {
						t_table = Table();
					}
					getsymdoo;
					if (sym == setsym) {
						buildTree(depth, wtool.now_word);
						buildTree(depth, "<字段赋值列表>");
						colvdo;//字段赋值列表
						wheredo;
						//执行
						if (t_table.itemnums != 0) {
							bool selected[100] = { 0 };
							for (int i = 0; i < cdts.size(); i++) {
								if (cdts[i].type == "and") {
									for (int j = 0; j < records.size(); j++) {
										vector<Item> im = records[j];
										if (i == 0) {
											add(cdts[i], im, j, selected);
										}
										else {
											if (selected[j] == true) add(cdts[i], im, j, selected);
										}
									}
								}
								else {
									for (int j = 0; j < records.size(); j++) {
										vector<Item> im = records[j];
										if (selected[j] == false) add(cdts[i], im, j, selected);
									}
								}
							}
							for (int i = 0; i < records.size(); i++) {
								if (selected[i] == true) {
									for (int j = 0; j < records[i].size(); j++) {
										for (int k = 0; k < updateItems.size(); k++) {
											if (records[i][j].fieldname==updateItems[k].fieldname) {
												records[i][j].fieldvalue = updateItems[k].fieldvalue;
												break;
											}
										}
									}

								}
							}
							string filename = t_table.tablename + ".txt";
							out.open(filename, ios::out);
							for (int i = 0; i < records.size(); i++) {
								vector<Item> t = records[i];
								for (int j = 0; j < t.size(); j++) {
									if(j!=t.size()-1)out << t[j].fieldvalue << " ";
									else out << t[j].fieldvalue << endl;
								}
							}
							out.close();
						}
					}
					else {
						err++;
						error(KWORD_ERR);
						return -1;
					}
				}
				else {
					err++;
					error(IDENT_POS);
					return -1;
				}
				break; 
			}
			case deletesym: {
				records.clear();
				buildTree(depth, wtool.now_word);
				getsymdoo;
				if (sym == fromsym) {
					buildTree(depth, wtool.now_word);
					getsymdoo;
					if (sym == ident) {
						buildTree(depth, wtool.now_ident);
						int pos = TableExist(wtool.now_ident);
						if (pos != -1) {
							t_table = tables[pos];
							string filename = wtool.now_ident + ".txt";
							in.open(filename, ios::in);//把所有记录从文件中读出来
							string str, line;
							while (getline(in, line))
							{
								istringstream stream(line);
								vector<Item> onerecord;
								int i = 0;
								while (stream >> str) {
									Item item;
									item.fieldname = t_table.items[i].fieldname;
									item.fieldtype = t_table.items[i].fieldtype;
									item.fieldvalue = str;
									item.length = t_table.items[i].length;
									onerecord.push_back(item);
									i++;
								}
								records.push_back(onerecord);
							}
							in.close();
							
						}
						else {
							t_table = Table();
						}
						getsymdoo;
						wheredo;
						//执行
						if (t_table.itemnums != 0) {
							bool selected[100] = { 0 };
							for (int i = 0; i < cdts.size(); i++) {
								if (cdts[i].type == "and") {
									for (int j = 0; j < records.size(); j++) {
										vector<Item> im = records[j];
										if (i == 0) {
											add(cdts[i], im, j, selected);
										}
										else {
											if (selected[j] == true) add(cdts[i], im, j, selected);
										}
									}
								}
								else {
									for (int j = 0; j < records.size(); j++) {
										vector<Item> im = records[j];
										if (selected[j] == false) add(cdts[i], im, j, selected);
									}
								}
							}
							string filename = t_table.tablename + ".txt";
							out.open(filename, ios::out);
							for (int i = 0; i < records.size(); i++) {
								if (selected[i] == false) {
									vector<Item> t = records[i];
									for (int j = 0; j < t.size(); j++) {
										if (j != t.size() - 1)out << t[j].fieldvalue << " ";
										else out << t[j].fieldvalue << endl;
									}
								}
							}
							out.close();
						}
					}
					else {
						err++;
						error(IDENT_POS);
						return -1;
					}
				}
				else {
					err++;
					error(KWORD_ERR);
					return -1;
				}
				break; 
			}
			default: {
				err++;
				error(SBEGIN_ERR);//错误的语句开始符号
				return -1;
			}
		}
		if (wtool.statbegsys[sym]) {
			err++;
			error(MISS_SEMI);//语句之间缺少分号
			return -1;
		}
		if (sym == semicolon) {
			if(wtool.isFileEnd) buildTree(1, ";");
			else {
			   buildTree(depth, ";");
			}
		}
	} while (sym == semicolon);
	depth--;
	return 0;
}
void SqlCompiler::error(int n) {
	cout << "第" << line_count << "行";
	cout << "第" << wtool.cc - 1 << "列附近:";
	cout << msg[n] << endl;
}
void SqlCompiler::buildTree(int n,string content) {
	for (int i = 1; i < n; i++) {
		gtree += "   ";
	}
	for (int i = 1; i <=n; i++) {
		gtree += "-";
	}
	gtree += content;
	gtree+="\n";
}
int SqlCompiler::coldeflist() {
	depth++;
	enum symbol &sym = wtool.sym;
	int count = 0;
	do{
		Item item;
		//buildTree(depth, ",");
		getsymdoo;
		if (count >= 1 && sym == rparen)break;//允许 字段 类型，)这样情况的出现
		if (sym == ident) {
			buildTree(depth, "<标识符>");
			buildTree(depth+1, wtool.now_ident);
			item.fieldname = wtool.now_ident;
			getsymdoo;
			if (wtool.datatype[sym]) {
				buildTree(depth, "<字段类型>");
				buildTree(depth+1, wtool.now_word);
				//item.fieldtype = sym;
				if (sym == varcharsym || sym == charsym) {
					item.fieldtype = strsym;//字符串类型
					getsymdoo;
					if (sym == lparen) {
						buildTree(depth+1, "(");
						getsymdoo;
						if (sym == Integer) {
							buildTree(depth+1, wtool.numstr);
							item.length = stringToInt(wtool.numstr);
							getsymdoo;
							if (sym == rparen) {
								buildTree(depth+1, ")");
							}
							else {
								err++;
								error(MISS_RPAR);
								return -1;
							}
						}
						else {
							error(NUM_POS);
							err++;
							return -1;
						}
					}
					else {
						error(MISS_LPAR);
						err++;
						return -1;
					}
				}
				else if (sym == doublesym) {
					item.fieldtype = Double;
				}
				else if (sym == intsym) {
					item.fieldtype = Integer;
				}
				else {
					item.fieldtype = strsym;
				}
				getsymdoo;
				t_table.items.push_back(item);
				t_table.itemnums++;
			}
			else {
				error(DTYPE_UNKNOWN);//未知的字段类型
				err++;
				return -1;
			}
		}
		else {
			error(COL_DEF_ERR);//字段定义的开始符号应是标识符
			err++;
			return -1;
		}
		if (sym == ident) {
			error(MISS_COMMA);
			err++;
			return -1;
		}
		buildTree(depth, ",");
		count++;
	} while (sym == comma);
	tables.push_back(t_table);//放入总表中
	depth--;
	return 0;
}
int SqlCompiler::wherec()
{
	cdts.clear();
	c = Condition();
	enum symbol &sym = wtool.sym;
	if (sym == wheresym) {
		buildTree(depth, "<where从句>");
		buildTree(depth+1,wtool.now_word);
		depth++;
		buildTree(depth, "<查询条件>");
		c.type = "and";
		conditiondo;
		c = Condition();
		while (sym == andsym || sym == orsym) {
			if (sym == andsym) c.type = "and";
			else c.type = "or";
			buildTree(depth, wtool.now_word);
			conditiondo;
		}
		depth--;
	}
	return 0;
}
int SqlCompiler::condition()
{
	depth++;
	buildTree(depth, "<条件表达式>");
	enum symbol &sym = wtool.sym;
	getsymdoo;
	if (sym == ident) {//列名
		buildTree(depth+1, "<标识符>");
		buildTree(depth+2, wtool.now_ident);
		getsymdoo;
		if (sym == neq || sym == eql || sym == lss || sym == leq || sym == gtr || sym == geq) {
			buildTree(depth+1, wtool.fuhao);
			c.op = sym;
			getsymdoo;
			if (wtool.constantsys[sym]) {
				buildTree(depth+1, "<常量>");
				int pos = colExist(t_table, wtool.now_ident);//检查列名是否存在
				if (pos != -1) {
					c.fieldname = wtool.now_ident;//列名
					//语义分析常量类型
					if (MatchColAndItsType(t_table,pos, sym)) {
						if (sym == Integer || sym == Double) {
							c.value = wtool.numstr;
							buildTree(depth+2, wtool.numstr);
						}
						else {
							c.value = wtool.strvalue;
							buildTree(depth+2, wtool.strvalue);
						}
						cdts.push_back(c);
						getsymdoo;
					}
					else {
						err++;
						error(DTYPE_ERR);//数据类型错误
						return -1;
					}
				}
				
			}
			else {
				err++;
				error(CONSTANT_POS);//应是常量
				return -1;
			}
		}
		else {
			err++;
			error(OPR_POS);//应是运算符
		}
	}
	else {
		err++;
		error(IDENT_POS);
		return -1;

	}
	depth--;
	return 0;
}
int SqlCompiler::TableExist(string tname)
{
	for (int i = 0; i < tables.size(); i++) {
		if (tables[i].tablename == tname) {
			return i;
		}
	}
	return -1;
}
void SqlCompiler::deleteTable(int pos)
{
	tables.erase(tables.begin() + pos);
}
int SqlCompiler::collist()
{
	selectItems.clear();
	allitems = false;
	depth++;
	enum symbol &sym = wtool.sym;
	getsymdoo;
	if (sym == times) {
		//如果是*
		buildTree(depth, "*");
		allitems = true;
		getsymdoo;
	}
	else {
		if (sym == ident) {
			buildTree(depth, "<标识符>");
			buildTree(depth+1, wtool.now_ident);
			selectItems.push_back(wtool.now_ident);//先把挑选出来的列暂存起来
		}
		else {
			err++;
			error(IDENT_POS);//应是标识符
			return -1;
		}
		getsymdoo;
		if (sym == comma) {
			buildTree(depth, ",");
			getsymdoo;
			do{
				if (sym == ident) {
					buildTree(depth, "<标识符>");
					buildTree(depth+1, wtool.now_ident);
					selectItems.push_back(wtool.now_ident);//先把挑选出来的列暂存起来
				}
				else {
					err++;
					error(IDENT_POS);//应是标识符
					return -1;
				}
				getsymdoo;
				if (sym == comma) {
					buildTree(depth, ",");
				}
			} while (sym == comma);
		}
		if (sym == ident) {
			err++;
			error(MISS_COMMA);//字段之间缺少逗号
			return -1;
		}
	}
	depth--;
	return 0;
}
int SqlCompiler::colExist(Table t, string cname)
{
	for (int i = 0; i < t.items.size(); i++) {
		if (t.items[i].fieldname == cname) return i;
	}
	return -1;
}
int SqlCompiler::matchtoken(enum symbol expected, int n,int depth,string content)
{
	if (expected != wtool.sym) {
		error(n);
		err++;
		return -1;
	}
	else {
		buildTree(depth, content);
		getsymdoo;//获取下一个字符
		return 0;
	}
	
}
bool SqlCompiler::inSelectedItems(Item item)
{
	for (int i = 0; i < selectItems.size(); i++) {
		if (item.fieldname == selectItems[i]) return true;
	}
	return false;
}
int SqlCompiler::stringToInt(string str) {
	int num = 0;
	for (int i = 0; i < str.length(); i++) {
		num = num * 10 + str[i] - '0';
	}
	return num;
}
bool SqlCompiler::MatchColAndItsType(Table t, int pos, enum symbol sym) {
	if (pos >= t.itemnums) return false;
	if (t.itemnums == 0) return true;//表不是本次创建的
	else {
		if (t.items[pos].fieldtype == sym) {
			return true;
		}
		else {
			return false;
		}
	}
}
int SqlCompiler::constantlist() {
	insertItems.clear();
	depth++;
	int colnums = 0;
	enum symbol &sym = wtool.sym;
	getsymdoo;
	if (wtool.constantsys[sym]) {
		buildTree(depth, "<常量>");
		Item item;
		item.fieldtype = sym;
		if (sym == Integer|| sym == Double) {
			buildTree(depth+1, wtool.numstr);
			item.fieldvalue = wtool.numstr;
		}
		else {
			buildTree(depth+1, wtool.strvalue);
			item.fieldvalue = wtool.strvalue;
		}
		if (t_table.itemnums != 0) {
			if (MatchColAndItsType(t_table, colnums, sym)) {//判断类型是否匹配
				insertItems.push_back(item);
				//t_table.items[colnums]=item;
				colnums++;
			}
			else {
				err++;
				error(DTYPE_ERR);
				return -1;
			}
		}
		
	}
	else {
		err++;
		error(CONSTANT_POS);//应是常量
		return -1;
	}
	getsymdoo;
	while (sym == comma){
		buildTree(depth, ",");
		getsymdoo;
		if (wtool.constantsys[sym]) {
			buildTree(depth, "<常量>");
			Item item;
			item.fieldtype = sym;
			if (sym == Integer || sym == Double) {
				buildTree(depth+1, wtool.numstr);
				item.fieldvalue = wtool.numstr;
			}
			else {
				buildTree(depth+1, wtool.strvalue);
				item.fieldvalue = wtool.strvalue;
			}
			if (t_table.itemnums != 0) {
				if (MatchColAndItsType(t_table, colnums, sym)) {//判断类型是否匹配
					insertItems.push_back(item);
					colnums++;
				}
				else {
					err++;
					error(DTYPE_ERR);
					return -1;
				}
			}
			getsymdoo;
		}
		else {
			err++;
			error(CONSTANT_POS);//应是常量
			return -1;
		}
	} 
	if (wtool.constantsys[sym]) {
		err++;
		error(MISS_COMMA);//常量之间缺少逗号
		return -1;
	}
	else {
		if (t_table.itemnums != 0) {//是本次创建的表
			if (colnums != t_table.itemnums) {
				err++;
				error(CONSTANT_NUM_ERR);//字段数量不匹配
				return -1;
			}
		}
	}
	depth--;
	return 0;
}
int  SqlCompiler::colv() {
	depth++;
	enum symbol &sym = wtool.sym;
	updateItems.clear();
	getsymdoo;
	if (sym == ident) {
		Item item;
		buildTree(depth, "<标识符>");
		buildTree(depth+1, wtool.now_ident);
		int pos = colExist(t_table, wtool.now_ident);
		getsymdoo;
		if (sym == eql) {
			buildTree(depth, "=");
			getsymdoo;
			if (wtool.constantsys[sym]) {
				buildTree(depth, "<常量>");
				if (sym == Integer || sym == Double) {
					buildTree(depth+1, wtool.numstr);
					item.fieldvalue = wtool.numstr;
				}
				else {
					buildTree(depth+1, wtool.strvalue);
					item.fieldvalue = wtool.strvalue;

				}
				if (t_table.itemnums != 0) {
					if (MatchColAndItsType(t_table, pos, sym)) {//判断类型是否匹配
						item.fieldname = t_table.items[pos].fieldname;
						item.fieldtype = t_table.items[pos].fieldtype;
						item.length = t_table.items[pos].length;
						updateItems.push_back(item);
					}
					else {
						err++;
						error(DTYPE_ERR);
						return -1;
					}
				}
			}
			else {
				err++;
				error(CONSTANT_POS);
				return -1;
			}
		}
		else {
			err++;
			error(EQL_POS);//应是等号
			return -1;
		}
	}
	else {
		err++;
		error(IDENT_POS);//应是标识符
		return -1;
	}
	getsymdoo;
	if (sym == comma) {
		buildTree(depth, ",");
		getsymdoo;
		do {
			Item item;
			if (sym == ident) {
				buildTree(depth, "<标识符>");
				buildTree(depth+1, wtool.now_ident);
				int pos = colExist(t_table,wtool.now_ident);
				getsymdoo;
				if (sym == eql) {
					buildTree(depth, "=");
					getsymdoo;
					if (wtool.constantsys[sym]) {
						buildTree(depth, "<常量>");
						if (sym == Integer || sym == Double) {
							buildTree(depth+1, wtool.numstr);
							item.fieldvalue = wtool.numstr;
						}
						else {
							buildTree(depth+1, wtool.strvalue);
							item.fieldvalue = wtool.strvalue;
						}
						if (t_table.itemnums != 0) {
							if (MatchColAndItsType(t_table, pos, sym)) {//判断类型是否匹配
								item.fieldname = t_table.items[pos].fieldname;
								item.fieldtype = t_table.items[pos].fieldtype;
								item.length = t_table.items[pos].length;
								updateItems.push_back(item);
							}
							else {
								err++;
								error(DTYPE_ERR);
								return -1;
							}
						}
					}
					else {
						err++;
						error(CONSTANT_POS);
						return -1;
					}
				}
				else {
					err++;
					error(EQL_POS);//应是等号
					return -1;
				}
			}
			else {
				err++;
				error(IDENT_POS);//应是标识符
				return -1;
			}
			getsymdoo;
			if (sym == comma) {
				buildTree(depth, ",");
			}
		} while (sym == comma);
	}
	if (sym == ident) {
		err++;
		error(MISS_COMMA);//字段之间缺少逗号
		return -1;
	}
	depth--;
	return 0;
}
void SqlCompiler::add(Condition c,vector<Item> it,int pos,bool*selected) {
		for (int i = 0; i < it.size(); i++) {
			if (it[i].fieldname == c.fieldname) {
				switch (c.op) {
					case eql:
						if (it[i].fieldvalue==c.value) {
							selected[pos] = true;
						}
						else {
							selected[pos] = false;
						}
						break;
					case neq:
						if (it[i].fieldvalue != c.value) {
							selected[pos] = true;
						}
						else {
							selected[pos] = false;
						}
						break;
					case gtr:
						if (it[i].fieldtype == Integer) {
							if (stringToInt(it[i].fieldvalue) > stringToInt(c.value)) {
								selected[pos] = true;
								return;
							}
							else {
								selected[pos] = false;
							}
						}
						else {
							if (it[i].fieldvalue > c.value) {
								selected[pos] = true;
								return;
							}
							else {
								selected[pos] = false;
							}
						}
						break;
					case geq:
						if (it[i].fieldtype == Integer) {
							if (stringToInt(it[i].fieldvalue) >= stringToInt(c.value)) {
								selected[pos] = true;
							}
							else {
								selected[pos] = false;
							}
						}
						else {
							if (it[i].fieldvalue >= c.value) {
								selected[pos] = true;
							}
							else {
								selected[pos] = false;
							}
						}
						break;
					case lss:
						if (it[i].fieldtype == Integer) {
							if (stringToInt(it[i].fieldvalue) < stringToInt(c.value)) {
								selected[pos] = true;
							}
							else {
								selected[pos] = false;
							}
						}
						else {
							if (it[i].fieldvalue < c.value) {
								selected[pos] = true;
							}
							else {
								selected[pos] = false;
							}
						}
						break;
					default://小于等于
						if (it[i].fieldtype == Integer ) {
							if (stringToInt(it[i].fieldvalue) <= stringToInt(c.value)) {
								selected[pos] = true;
							}
							else {
								selected[pos] = false;
							}
						}
						else {
							if (it[i].fieldvalue <= c.value) {
								selected[pos] = true;
							}
							else {
								selected[pos] = false;
							}
						}
						break;
				}
				break;
			}
		}
}
