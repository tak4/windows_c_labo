#include <windows.h>
#include <cstdio>
#include <regex>
#include <string>
#include <codecvt>

using namespace std;

#define DEF_INPUT_FILE_PATH_SIZE	128
#define DEF_READBUF_SIZE			256


void _regex_search(string r_buf, regex pat)
{
	if (regex_search(r_buf, pat)) {
		printf("[!!match!!] %s", r_buf.c_str());
	}
	else {
		printf("%s", r_buf.c_str());
	}
}


void _regex_replace(string r_buf, regex pat, string replace)
{
	string result = regex_replace(r_buf, pat, replace);
	printf("%s", result.c_str());
}


int main(int argc, char* argv[])
{
	FILE* r_fp, w_fp;
	errno_t f_ret = 0;
	char input_file_path[DEF_INPUT_FILE_PATH_SIZE] = { 0 };
	char read_buf[DEF_READBUF_SIZE] = { 0 };

	//printf("wchar_t sizeof() = %zu, sizeof(readBuf) = %zu\n", sizeof(wchar_t), sizeof(read_buf));

	// 入力ファイルパスを取得
	if ( argc < 2 ) {
		printf("parameter error...");
		return -1;
	}
	// _TRUNCATE 指定でコピー先バッファの最後にNULLが入る
	strncpy_s(input_file_path, sizeof(input_file_path), argv[1], _TRUNCATE);

	// 正規表現
	regex pattern("([0-9]{4})");
	string replacement = "$1\t";

	//
	// オープン
	//
	f_ret = fopen_s(&r_fp, input_file_path, "r");
	if ((f_ret != 0) || (r_fp == 0)) {
		printf("fopen_s() fopen_errno = %d\n", f_ret);
		return -1;
	}

	printf("sizeof(wchar_t)=%zu, sizeof(readBuf)=%zu\n", sizeof(wchar_t), sizeof(read_buf));

	//
	// 読み込み
	//
	while(fgets(read_buf, DEF_READBUF_SIZE, r_fp) != NULL) {
		string r_buf_string(read_buf);
		//_regex_search(r_buf_string, pattern);
		_regex_replace(r_buf_string, pattern, replacement);
	}

	//
	// クローズ
	//
	fclose(r_fp);

	return 0;
}

