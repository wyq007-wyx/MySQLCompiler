#include"SqlCompiler.h"


void SqlCompiler::start() {
	
	string filename;
	cout << "����Ҫ�������ļ���:";
	cin >> filename;
	wtool.ifs.open(filename, ios::in);
	wtool.ofs_error.open("word_error.txt", ios::out);
	wtool.ofs_word.open("word_result.txt", ios::out);
	if (!wtool.ifs.is_open()) {
		cout<<"�ļ���ʧ��"<<endl;
		return;
	}
	//��ʼ��
	wtool.init();
	wtool.ch = ' ';
	wtool.cc = wtool.ll = 0;
	buildTree(depth, "<����>");
	int result = statement();
	if (result!=-1) {
		//����﷨��
		if (err == 0 ) {
			if (wtool.fuhao == ";") {
				cout << "������ȷ" << endl;
				cout << "---�﷨��---" << endl;
				cout << gtree;
			}
			else {
				cout << "����δ����" << endl;
			}
			
		}
		else {
			cout << "�ʷ���������" << endl;
		}
	}
	else {
		cout << "sql������ִ���" << endl;
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
				getsymdoo;//��ȡ��һ������
				if (sym == tablesym) {
					buildTree(depth, "table");//create table
					getsymdoo;
					t_table = Table();
					if (sym == ident) {//create table xxx
						buildTree(depth, "<��ʶ��>");
						buildTree(depth+1, wtool.now_ident);
						t_table.tablename = wtool.now_ident;
						getsymdoo;
						if (sym == lparen) {//create table xxx(
							buildTree(depth, "(");
							buildTree(depth, "<�ֶζ����б�>");
							coldeflistdo;//�ݹ��½�
							if (sym != rparen) {
								error(MISS_RPAR);
								err++;
								return -1;
							}
							else {
								string filename = t_table.tablename+".txt";
								out.open(filename, ios::app);
								buildTree(depth, ")");//create table xxx(<�ֶζ����б�>)
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
							cout << "---ɾ������---" << endl;
							if (remove(filename.c_str())==0) {
								cout << "ɾ���ɹ�" << endl;
							}
							else {
								cout << "ɾ��ʧ��" << endl;
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
						error(IDENT_POS);//Ӧ�Ǳ�ʶ��
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
				buildTree(depth, "<�ֶ��б�>");
				collistdo;//<�ֶ��б�>
				if (sym == fromsym) {
					buildTree(depth,wtool.now_word);
					getsymdoo;
					if (sym == ident) {//����
						buildTree(depth, "<��ʶ��>");
						buildTree(depth+1, wtool.now_ident);
						int pos = TableExist(wtool.now_ident);
						if (pos != -1) {
							t_table=tables[pos];//��ñ�
							if (allitems) {
								for (int i = 0; i < t_table.items.size(); i++) {
									selectItems.push_back(t_table.items[i].fieldname);
								}
							}
							string filename = wtool.now_ident + ".txt";
							in.open(filename, ios::in);//�����м�¼���ļ��ж�����
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
							t_table = Table();//��ÿձ�
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
							cout << "---select�����---" << endl;
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
								buildTree(depth, "<�����б�>");
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
						t_table = tables[pos];//��ñ�
						string filename = wtool.now_ident + ".txt";
						in.open(filename, ios::in);//�����м�¼���ļ��ж�����
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
						buildTree(depth, "<�ֶθ�ֵ�б�>");
						colvdo;//�ֶθ�ֵ�б�
						wheredo;
						//ִ��
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
							in.open(filename, ios::in);//�����м�¼���ļ��ж�����
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
						//ִ��
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
				error(SBEGIN_ERR);//�������俪ʼ����
				return -1;
			}
		}
		if (wtool.statbegsys[sym]) {
			err++;
			error(MISS_SEMI);//���֮��ȱ�ٷֺ�
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
	cout << "��" << line_count << "��";
	cout << "��" << wtool.cc - 1 << "�и���:";
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
		if (count >= 1 && sym == rparen)break;//���� �ֶ� ���ͣ�)��������ĳ���
		if (sym == ident) {
			buildTree(depth, "<��ʶ��>");
			buildTree(depth+1, wtool.now_ident);
			item.fieldname = wtool.now_ident;
			getsymdoo;
			if (wtool.datatype[sym]) {
				buildTree(depth, "<�ֶ�����>");
				buildTree(depth+1, wtool.now_word);
				//item.fieldtype = sym;
				if (sym == varcharsym || sym == charsym) {
					item.fieldtype = strsym;//�ַ�������
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
				error(DTYPE_UNKNOWN);//δ֪���ֶ�����
				err++;
				return -1;
			}
		}
		else {
			error(COL_DEF_ERR);//�ֶζ���Ŀ�ʼ����Ӧ�Ǳ�ʶ��
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
	tables.push_back(t_table);//�����ܱ���
	depth--;
	return 0;
}
int SqlCompiler::wherec()
{
	cdts.clear();
	c = Condition();
	enum symbol &sym = wtool.sym;
	if (sym == wheresym) {
		buildTree(depth, "<where�Ӿ�>");
		buildTree(depth+1,wtool.now_word);
		depth++;
		buildTree(depth, "<��ѯ����>");
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
	buildTree(depth, "<�������ʽ>");
	enum symbol &sym = wtool.sym;
	getsymdoo;
	if (sym == ident) {//����
		buildTree(depth+1, "<��ʶ��>");
		buildTree(depth+2, wtool.now_ident);
		getsymdoo;
		if (sym == neq || sym == eql || sym == lss || sym == leq || sym == gtr || sym == geq) {
			buildTree(depth+1, wtool.fuhao);
			c.op = sym;
			getsymdoo;
			if (wtool.constantsys[sym]) {
				buildTree(depth+1, "<����>");
				int pos = colExist(t_table, wtool.now_ident);//��������Ƿ����
				if (pos != -1) {
					c.fieldname = wtool.now_ident;//����
					//���������������
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
						error(DTYPE_ERR);//�������ʹ���
						return -1;
					}
				}
				
			}
			else {
				err++;
				error(CONSTANT_POS);//Ӧ�ǳ���
				return -1;
			}
		}
		else {
			err++;
			error(OPR_POS);//Ӧ�������
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
		//�����*
		buildTree(depth, "*");
		allitems = true;
		getsymdoo;
	}
	else {
		if (sym == ident) {
			buildTree(depth, "<��ʶ��>");
			buildTree(depth+1, wtool.now_ident);
			selectItems.push_back(wtool.now_ident);//�Ȱ���ѡ���������ݴ�����
		}
		else {
			err++;
			error(IDENT_POS);//Ӧ�Ǳ�ʶ��
			return -1;
		}
		getsymdoo;
		if (sym == comma) {
			buildTree(depth, ",");
			getsymdoo;
			do{
				if (sym == ident) {
					buildTree(depth, "<��ʶ��>");
					buildTree(depth+1, wtool.now_ident);
					selectItems.push_back(wtool.now_ident);//�Ȱ���ѡ���������ݴ�����
				}
				else {
					err++;
					error(IDENT_POS);//Ӧ�Ǳ�ʶ��
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
			error(MISS_COMMA);//�ֶ�֮��ȱ�ٶ���
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
		getsymdoo;//��ȡ��һ���ַ�
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
	if (t.itemnums == 0) return true;//���Ǳ��δ�����
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
		buildTree(depth, "<����>");
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
			if (MatchColAndItsType(t_table, colnums, sym)) {//�ж������Ƿ�ƥ��
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
		error(CONSTANT_POS);//Ӧ�ǳ���
		return -1;
	}
	getsymdoo;
	while (sym == comma){
		buildTree(depth, ",");
		getsymdoo;
		if (wtool.constantsys[sym]) {
			buildTree(depth, "<����>");
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
				if (MatchColAndItsType(t_table, colnums, sym)) {//�ж������Ƿ�ƥ��
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
			error(CONSTANT_POS);//Ӧ�ǳ���
			return -1;
		}
	} 
	if (wtool.constantsys[sym]) {
		err++;
		error(MISS_COMMA);//����֮��ȱ�ٶ���
		return -1;
	}
	else {
		if (t_table.itemnums != 0) {//�Ǳ��δ����ı�
			if (colnums != t_table.itemnums) {
				err++;
				error(CONSTANT_NUM_ERR);//�ֶ�������ƥ��
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
		buildTree(depth, "<��ʶ��>");
		buildTree(depth+1, wtool.now_ident);
		int pos = colExist(t_table, wtool.now_ident);
		getsymdoo;
		if (sym == eql) {
			buildTree(depth, "=");
			getsymdoo;
			if (wtool.constantsys[sym]) {
				buildTree(depth, "<����>");
				if (sym == Integer || sym == Double) {
					buildTree(depth+1, wtool.numstr);
					item.fieldvalue = wtool.numstr;
				}
				else {
					buildTree(depth+1, wtool.strvalue);
					item.fieldvalue = wtool.strvalue;

				}
				if (t_table.itemnums != 0) {
					if (MatchColAndItsType(t_table, pos, sym)) {//�ж������Ƿ�ƥ��
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
			error(EQL_POS);//Ӧ�ǵȺ�
			return -1;
		}
	}
	else {
		err++;
		error(IDENT_POS);//Ӧ�Ǳ�ʶ��
		return -1;
	}
	getsymdoo;
	if (sym == comma) {
		buildTree(depth, ",");
		getsymdoo;
		do {
			Item item;
			if (sym == ident) {
				buildTree(depth, "<��ʶ��>");
				buildTree(depth+1, wtool.now_ident);
				int pos = colExist(t_table,wtool.now_ident);
				getsymdoo;
				if (sym == eql) {
					buildTree(depth, "=");
					getsymdoo;
					if (wtool.constantsys[sym]) {
						buildTree(depth, "<����>");
						if (sym == Integer || sym == Double) {
							buildTree(depth+1, wtool.numstr);
							item.fieldvalue = wtool.numstr;
						}
						else {
							buildTree(depth+1, wtool.strvalue);
							item.fieldvalue = wtool.strvalue;
						}
						if (t_table.itemnums != 0) {
							if (MatchColAndItsType(t_table, pos, sym)) {//�ж������Ƿ�ƥ��
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
					error(EQL_POS);//Ӧ�ǵȺ�
					return -1;
				}
			}
			else {
				err++;
				error(IDENT_POS);//Ӧ�Ǳ�ʶ��
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
		error(MISS_COMMA);//�ֶ�֮��ȱ�ٶ���
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
					default://С�ڵ���
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
