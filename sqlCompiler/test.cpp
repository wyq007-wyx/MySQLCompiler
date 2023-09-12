//#include"WordAnalyzer.h"
//
//int main() {
//	WordAnalyzer ww;
//	ww.ifs.open("test.txt", ios::in);
//	ww.ofs_error.open("word_error.txt", ios::out);
//	ww.ofs_word.open("word_result.txt", ios::out);
//	if (!ww.ifs.is_open()) {
//		cout << "文件打开失败" << endl;
//	}
//	ww.init();
//	while (ww.getsym() != -1) {}
//	ww.ifs.close();
//	ww.ofs_error.close();
//	ww.ofs_word.close();
//}