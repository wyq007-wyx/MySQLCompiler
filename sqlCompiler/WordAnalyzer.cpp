#include "WordAnalyzer.h"
//获取字符
int WordAnalyzer::getch() {
	if (cc == ll) {
		if (isFileEnd) {
			//读到文件末尾
			//error(FILEEND);
			return FILEEND;
			//return 1;
		}
		ll = 0;
		cc = 0;
		ch = ' ';
		line = "";
		//读文件的一行到行缓冲区
		while (ch != '\n') {
			ch = ifs.get();
			if (ch == EOF)
			{
				isFileEnd = true;
				break;
				//return FILEEND;
			}
			//printf("%c", ch);
			line+= ch;
			ll++;
		}
		line_count++;//行数+1
		if (ll == 0 && isFileEnd) {
			cc = 1;
			//error(FILEEND);
			//return -1;
			return FILEEND;
		}
	}
	ch = line[cc];
	cc++;
	return 0;
}
//获取符号
int WordAnalyzer::getsym() {
	while (isSpace(ch)) {
		getchdo;
	}
	//判断是否以字母或者下划线开头的标识符
	if(isLetterOrUnderline(ch)) {
		return Identifier();
	}
	//判断是否为数字
	else if(isNumber(ch)){
		return Number();
	}
	//判断是否为注释
	else if (isPossibleNote(ch)) {
		return Note();
	}
	//判断是否是字符串的开始
	else if (isStr(ch)) {
		return Str();
	}
	//其他情况为运算符或者分隔符
	else{
		return OperatorOrSeparator();
		//cout << ch<<endl;
		//getchdo;
	}
	return 0;
}
bool WordAnalyzer::isSpace(char ch) {
	if (ch == ' ' || ch == '\n' || ch == '\t') {
		return true;
	}
	else {
		return false;
	}
}
void WordAnalyzer::error(int n) {
	cout << "第" << line_count <<"行";
	cout << "第" << cc - 1 << "列附近:";
	cout << msg[n] << endl;
}
bool WordAnalyzer::isNumber(char ch) {
	if (ch >= '0'&& ch <= '9') return true;
	else {
		return false;
	}
}
bool WordAnalyzer::isStr(char ch) {
	if (ch == '\'') {
		return true;
	}
	else {
		return false;
	}
}
bool WordAnalyzer::isLetterOrUnderline(char ch) {
	if (ch >= 'a'&&ch <= 'z') return true;
	else if (ch >= 'A'&&ch <= 'Z') return true;
	else if (ch == '_') return true;
	else return false;
}
bool WordAnalyzer::isPossibleNote(char ch) {
	if (ch == '-' || ch == '/' || ch == '#') {
		return true;
	}
	else {
		return false;
	}
}
void WordAnalyzer::toLower(string &str) {
	for (int i = 0; i < str.size(); i++) {
		if (str[i] >= 'A'&&str[i] <= 'Z') {
			str[i] += 32;
		}
	}
}
//初始化
void WordAnalyzer::init()
{
	int i;
	for (i = 0; i <= 255; i++)
	{
		ssym[i] = nul;
	}
	ssym['+'] = splus;
	ssym['-'] = sminus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym[';'] = semicolon;
	ssym['.'] = point;
	ssym['%'] = mod;
	/*设置保留字名字,按照字母顺序,便于折半查找*/
	sort(keyword, keyword + knum);
	/*设置保留字符号*/
	wsym[0] = andsym;
	wsym[1] = betweensym;
	wsym[2] = charsym;
	wsym[3] = createsym;
	wsym[4] = datesym;
	wsym[5] = deletesym;
	wsym[6] = doublesym;
	wsym[7] = dropsym;
	wsym[8] = fromsym;
	wsym[9] = insertsym;
	wsym[10] = intsym;
	wsym[11] = intosym;
	wsym[12] = likesym;
	wsym[13] = orsym;
	wsym[14] = selectsym;
	wsym[15] = setsym;
	wsym[16] = tablesym;
	wsym[17] = timesym;
	wsym[18] = updatesym;
	wsym[19] = valuessym;
	wsym[20] = varcharsym;
	wsym[21] = wheresym;
	for (i = 0; i < symnum; i++) {
		statbegsys[i] = false;
		constantsys[i] = false;
		datatype[i] = false;
		optsys[i] = false;
	}
	statbegsys[createsym] = true;
	statbegsys[insertsym] = true;
	statbegsys[dropsym] = true;
	statbegsys[deletesym] = true;
	statbegsys[selectsym] = true;
	statbegsys[updatesym] = true;
	constantsys[Integer] = true;
	constantsys[Double] = true;
	constantsys[strsym] = true;
	datatype[intsym] = true;
	datatype[doublesym] = true;
	datatype[varcharsym] = true;
	datatype[datesym] = true;
	datatype[timesym] = true;
	datatype[charsym] = true;
}
int WordAnalyzer::Number() {
	numstr = "";
	numstr += ch;
	int state = 1;//有限自动机，初始状态为1
	int errornum = 0;//表示数字没出错
	while (state != 7 && errornum == 0) {
		getchdo;
		switch (state) {
		case 1: {
			//状态1：数字
			if (isNumber(ch)) {
				numstr += ch;
			}
			else if(ch=='.'){
				numstr += ch;
				state = 2;
			}
			else{
				state = 7;
				sym = Integer;
			}
			break;
		}
		case 2: {
			//状态2：数字.
			if (isNumber(ch)) {
				numstr += ch;
				state = 3;
			}
			else {
				errornum = 1;
				error(NUMBER_ERR);
				outputError(NUMBER_ERR, line_count, cc - 1);
				sym = nul;
			}
			break;
		}
		case 3: {
			//状态3：数字.数字
			if (isNumber(ch)) {
				//输入数字,保持原状态不变
				numstr += ch;
			}
			else if (ch == 'e' || ch == 'E') {
				//输入e或者E,进入状态4
				numstr += ch;
				state = 4;
			}
			else {
				//否则表示这个单词识别完毕，进入结束状态
				state = 7;
				//保存输出当前词语
				sym = Double;
			}
			break;
		}
		case 4: {
			//状态4：数字.数字e/E
			if (isNumber(ch)) {
				//输入数字进入状态
				state = 6;
				numstr += ch;
			}
			else if (ch == '+' || ch == '-') {
				//输入+或者-进入状态5
				state = 5;
				numstr += ch;
			}
			else {
				errornum = 1;
				error(NUMBER_ERR);
				outputError(NUMBER_ERR, line_count, cc - 1);
				sym = nul;
				//e/E后面输入非数字非正负号，编译报错
			}
			break;
		}
		case 5: {
			//状态5：数字.数字e/E+/-
			if (isNumber(ch)) {
				//输入数字，进入状态6
				state = 6;
				numstr += ch;
			}
			else {
				errornum = 1;
				error(NUMBER_ERR);
				outputError(NUMBER_ERR, line_count, cc - 1);
				sym = nul;
				//+-后面输入非数字，编译报错
			}
			break;
		}
		case 6: {
			//状态6：数字.数字e/E数字
			//状态6：数字.数字e/E+/-数字
			if (isNumber(ch)) {
				numstr += ch;
			}
			else {
				state = 7;
				//保存输出当前词语
				sym = Double;
			}
			break;
		}
		}
	}
	//如果有错误,读取下一个符号
	if (errornum == 1) {
		err++;
		getsymdo;
	}
	else {
		if (sym == Integer) {
			outputwResult("整数", numstr);
		}
		else {
			outputwResult("小数", numstr);
		}
	}
	return 0;
}
int WordAnalyzer::Identifier() {
	int i, j, k;
	k = 0;
	temp_word = "";
	now_word = "";
	do {
		if (k < al)
		{
			temp_word += ch;
			k++;
		}
		else {
			error(MAX_LEN);
			err++;
			outputError(MAX_LEN, line_count, cc - 1);
			sym = nul;
			getsymdo;
			return 0;
		}
		getchdo;
	} while (isLetterOrUnderline(ch)||isNumber(ch));
	now_ident = temp_word;
	now_word = temp_word;
	toLower(now_word);//变为小写
	i = 0;
	j = knum - 1;
	do {
		k = (i + j) / 2;
		if (now_word <= keyword[k])
		{
			j = k - 1;
		}
		if (now_word >= keyword[k])
		{
			i = k + 1;
		}

	} while (i <= j);
	if (i - 1 > j)
	{
		sym = wsym[k];
	}
	else
	{
		sym = ident;
	}
	if (sym == ident) {
		outputwResult("标识符", now_ident);
	}
	else {
		outputwResult("关键字", now_word);
	}
	return 0;
}
int WordAnalyzer::Note() {
	//有限自动机
	string str = "";
	str += ch;
	if (ch == '/') {
		int state = 1;
		while (state != 4) {
			getchdo;
			switch (state)
			{
			case 1: {
				//状态1：/
				if (ch == '*') {
					state = 2;
				}else {
					//说明这是一个除号
					outputwResult("运算符", str);
					fuhao = str;
					sym = slash;
					state = 4;
				}
				break;
			}
			case 2: {
				//状态2：/*
				if (ch == '*') {
					state = 3;
				}
				break;
			}
			case 3: {
				//状态3：/* *
				if (ch == '*') {
					//保持原状态
					state = 3;
				}
				else if (ch == '/') {
					//进入结束状态
					state = 4;
				}
				else {
					//返回状态2
					state = 2;
				}
				break;
			}
			default:
				break;
			}
		}
		if(state==4)getsymdo;
	}
	else if (ch == '-') {
		getchdo;
		if (ch == '-') {
			//--表示单行注释
			while (ch != '\n') {
				getchdo;
			}
			getsymdo;
		}else{
			sym = sminus;
			outputwResult("运算符",str);
		}
	}
	else {
		//#号表示单行注释
		while (ch != '\n') {
			getchdo;
		}
		getsymdo;
	}
	return 0;
}
int WordAnalyzer::OperatorOrSeparator() {
	fuhao = ""; 
	fuhao+=ch;
	switch (ch) {
		case '>': {
			getchdo;
			if (ch == '=') {
				fuhao += ch;
				sym = geq;
			}
			else if (ch == '>') {
				fuhao += ch;
				sym = neq;
			}
			else {
				sym = gtr;
			}
			getchdo;
			break;
		}
		case '<': {
			getchdo;
			if (ch == '=') {
				fuhao += ch;
				sym = leq;
			}
			else if(ch=='>'){
				fuhao += ch;
				sym = neq;
			}
			else {
				sym = lss;
			}
			getchdo;
			break;
		}
		case '!': {
			getchdo;
			if (ch == '=') {
				fuhao += ch;
				sym = neq;
			}
			else {
				sym = nul;
			}
			getchdo;
			break;
		}
		default: {
			//其余情况一律按单符号算
			if (ch < 0) {
				error(SYM_UNKNOWN);
				err++;
				sym = nul;
			}
			else {
				sym = ssym[ch];
				getchdo;
			}
		}
	}
	//testOutPut(fuhao, "运算符或分隔符");
	if (sym == splus || sym == sminus || sym == times || sym == slash || sym == eql
		|| sym == neq || sym == lss || sym == leq || sym == gtr || sym == geq || sym==mod)
	{
		outputwResult("运算符",fuhao);
	}
	else if(sym==nul){
		outputwResult("未知的符号", fuhao);
	}
	else {
		outputwResult("分隔符", fuhao);
	}
	return 0;
};
int WordAnalyzer::Str() {
	strvalue = "";
	while (ch != '\n') {
		getchdo;
		if (ch == '\'') {
			sym = strsym;
			outputwResult("字符串", strvalue);
			getchdo;
			break;
		}
		else if (ch == '\n') {
			error(MISS_CLOSEQ);
			outputError(MISS_CLOSEQ, line_count, cc - 1);
			err++;
			sym = nul;
			break;
		}
		else {
			if (ch != '\'') {
				strvalue += ch;
			}
		}
	}
	return 0;
}
void WordAnalyzer::testOutPut(string type,string value) {
	cout << "类型:" << type << endl;
	cout << "值:" << value << endl;
}
void WordAnalyzer::outputwResult(string type,string value) {
	if (!ofs_word.is_open()) return;
	ofs_word << "类型:" << type << endl;
	ofs_word << "值:" << value << endl;
	ofs_word << "--------------------" << endl;
}
void WordAnalyzer::outputError(int n,int lpos,int cpos) {
	if (!ofs_error.is_open()) return;
	ofs_error << "第" << lpos << "行," << "第" << cpos << "列附近:";
	ofs_error << msg[n] << endl;
}
