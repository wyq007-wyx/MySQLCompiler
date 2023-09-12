#include "WordAnalyzer.h"
//��ȡ�ַ�
int WordAnalyzer::getch() {
	if (cc == ll) {
		if (isFileEnd) {
			//�����ļ�ĩβ
			//error(FILEEND);
			return FILEEND;
			//return 1;
		}
		ll = 0;
		cc = 0;
		ch = ' ';
		line = "";
		//���ļ���һ�е��л�����
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
		line_count++;//����+1
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
//��ȡ����
int WordAnalyzer::getsym() {
	while (isSpace(ch)) {
		getchdo;
	}
	//�ж��Ƿ�����ĸ�����»��߿�ͷ�ı�ʶ��
	if(isLetterOrUnderline(ch)) {
		return Identifier();
	}
	//�ж��Ƿ�Ϊ����
	else if(isNumber(ch)){
		return Number();
	}
	//�ж��Ƿ�Ϊע��
	else if (isPossibleNote(ch)) {
		return Note();
	}
	//�ж��Ƿ����ַ����Ŀ�ʼ
	else if (isStr(ch)) {
		return Str();
	}
	//�������Ϊ��������߷ָ���
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
	cout << "��" << line_count <<"��";
	cout << "��" << cc - 1 << "�и���:";
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
//��ʼ��
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
	/*���ñ���������,������ĸ˳��,�����۰����*/
	sort(keyword, keyword + knum);
	/*���ñ����ַ���*/
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
	int state = 1;//�����Զ�������ʼ״̬Ϊ1
	int errornum = 0;//��ʾ����û����
	while (state != 7 && errornum == 0) {
		getchdo;
		switch (state) {
		case 1: {
			//״̬1������
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
			//״̬2������.
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
			//״̬3������.����
			if (isNumber(ch)) {
				//��������,����ԭ״̬����
				numstr += ch;
			}
			else if (ch == 'e' || ch == 'E') {
				//����e����E,����״̬4
				numstr += ch;
				state = 4;
			}
			else {
				//�����ʾ�������ʶ����ϣ��������״̬
				state = 7;
				//���������ǰ����
				sym = Double;
			}
			break;
		}
		case 4: {
			//״̬4������.����e/E
			if (isNumber(ch)) {
				//�������ֽ���״̬
				state = 6;
				numstr += ch;
			}
			else if (ch == '+' || ch == '-') {
				//����+����-����״̬5
				state = 5;
				numstr += ch;
			}
			else {
				errornum = 1;
				error(NUMBER_ERR);
				outputError(NUMBER_ERR, line_count, cc - 1);
				sym = nul;
				//e/E������������ַ������ţ����뱨��
			}
			break;
		}
		case 5: {
			//״̬5������.����e/E+/-
			if (isNumber(ch)) {
				//�������֣�����״̬6
				state = 6;
				numstr += ch;
			}
			else {
				errornum = 1;
				error(NUMBER_ERR);
				outputError(NUMBER_ERR, line_count, cc - 1);
				sym = nul;
				//+-������������֣����뱨��
			}
			break;
		}
		case 6: {
			//״̬6������.����e/E����
			//״̬6������.����e/E+/-����
			if (isNumber(ch)) {
				numstr += ch;
			}
			else {
				state = 7;
				//���������ǰ����
				sym = Double;
			}
			break;
		}
		}
	}
	//����д���,��ȡ��һ������
	if (errornum == 1) {
		err++;
		getsymdo;
	}
	else {
		if (sym == Integer) {
			outputwResult("����", numstr);
		}
		else {
			outputwResult("С��", numstr);
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
	toLower(now_word);//��ΪСд
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
		outputwResult("��ʶ��", now_ident);
	}
	else {
		outputwResult("�ؼ���", now_word);
	}
	return 0;
}
int WordAnalyzer::Note() {
	//�����Զ���
	string str = "";
	str += ch;
	if (ch == '/') {
		int state = 1;
		while (state != 4) {
			getchdo;
			switch (state)
			{
			case 1: {
				//״̬1��/
				if (ch == '*') {
					state = 2;
				}else {
					//˵������һ������
					outputwResult("�����", str);
					fuhao = str;
					sym = slash;
					state = 4;
				}
				break;
			}
			case 2: {
				//״̬2��/*
				if (ch == '*') {
					state = 3;
				}
				break;
			}
			case 3: {
				//״̬3��/* *
				if (ch == '*') {
					//����ԭ״̬
					state = 3;
				}
				else if (ch == '/') {
					//�������״̬
					state = 4;
				}
				else {
					//����״̬2
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
			//--��ʾ����ע��
			while (ch != '\n') {
				getchdo;
			}
			getsymdo;
		}else{
			sym = sminus;
			outputwResult("�����",str);
		}
	}
	else {
		//#�ű�ʾ����ע��
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
			//�������һ�ɰ���������
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
	//testOutPut(fuhao, "�������ָ���");
	if (sym == splus || sym == sminus || sym == times || sym == slash || sym == eql
		|| sym == neq || sym == lss || sym == leq || sym == gtr || sym == geq || sym==mod)
	{
		outputwResult("�����",fuhao);
	}
	else if(sym==nul){
		outputwResult("δ֪�ķ���", fuhao);
	}
	else {
		outputwResult("�ָ���", fuhao);
	}
	return 0;
};
int WordAnalyzer::Str() {
	strvalue = "";
	while (ch != '\n') {
		getchdo;
		if (ch == '\'') {
			sym = strsym;
			outputwResult("�ַ���", strvalue);
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
	cout << "����:" << type << endl;
	cout << "ֵ:" << value << endl;
}
void WordAnalyzer::outputwResult(string type,string value) {
	if (!ofs_word.is_open()) return;
	ofs_word << "����:" << type << endl;
	ofs_word << "ֵ:" << value << endl;
	ofs_word << "--------------------" << endl;
}
void WordAnalyzer::outputError(int n,int lpos,int cpos) {
	if (!ofs_error.is_open()) return;
	ofs_error << "��" << lpos << "��," << "��" << cpos << "�и���:";
	ofs_error << msg[n] << endl;
}
