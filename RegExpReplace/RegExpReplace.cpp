#include <windows.h>
#include <cstdio>
#include <regex>
#include <string>
#include <codecvt>

using namespace std;

#define DEF_FILE_PATH_SIZE	128
#define DEF_READBUF_SIZE	256

//
// �����񌟍�
//
void _regex_search(string r_buf, regex pat)
{
	if (regex_search(r_buf, pat)) {
		printf("[!!match!!] %s", r_buf.c_str());
	}
	else {
		printf("%s", r_buf.c_str());
	}
}


//
// ������u��
//
void _regex_replace(string r_buf, regex pat, string replace, string &out_string)
{
	out_string = regex_replace(r_buf, pat, replace);
}

//
// �t�@�C���t���p�X���p�X�ƃt�@�C�����ɕ�������
//
void split_file_and_path(
	string	&string_in_file_path,
	string	&out_file_path,
	string	&out_file_name
)
{
	// ��ԍŌ�� \ �̈ʒu���m�F����
	size_t pos = string_in_file_path.find_last_of("\\");
	printf("pos=%zd\n", pos);

	if ( pos != -1 ) {
		// ��ԍŌ�� \ �܂ł��p�X�Ƃ���
		out_file_path = string_in_file_path.substr(0, pos+1);
		//strncpy_s(out_file_path, out_file_path_size, string_in_file_path.c_str(), pos + 1);
		// ��ԍŌ�� \ ������t�@�C�����Ƃ���
		out_file_name = string_in_file_path.substr(pos+1);
		//strncpy_s(out_file_name, out_file_name_path_size, string_in_file_path, out_file_name_path_size);
	}
}


//
// ���̓t�@�C���̍s�����擾����
//
unsigned int get_file_line(string file_path)
{
	FILE *r_fp;
	errno_t f_ret = 0;
	char read_buf[DEF_READBUF_SIZE] = { 0 };
	unsigned int total_lines = 0;

	//
	// �I�[�v��
	//
	f_ret = fopen_s(&r_fp, file_path.c_str(), "r");
	if ((f_ret != 0) || (r_fp == 0)) {
		printf("fopen_s() fopen_errno = %d\n", f_ret);
		return -1;
	}
	//
	// �ǂݍ���
	//
	while (fgets(read_buf, DEF_READBUF_SIZE, r_fp) != NULL) {
		total_lines++;
	}

	//
	// �N���[�Y
	//
	fclose(r_fp);

	return total_lines;
}


int main(int argc, char* argv[])
{
	// �t�@�C�������֘A
	FILE *r_fp = NULL, *w_fp = NULL;
	errno_t r_fret = 0, w_fret = 0;
	//char in_file_path[DEF_FILE_PATH_SIZE]	= { 0 };
	string in_file_path;
	string out_file_path;
	string path;
	string file_name;
	char read_buf[DEF_READBUF_SIZE]			= { 0 };

	// �����i�����\���֘A
	unsigned int current_line	= 0;
	unsigned int total_lines	= 0;
	float rate_of_progress		= 0.0f;
	COORD coord;	// COORD�\���̂ŕ\���ʒu���w��

	// ���̓t�@�C���p�X���擾
	if ( argc < 2 ) {
		printf("parameter error...");
		return -1;
	}
	// _TRUNCATE �w��ŃR�s�[��o�b�t�@�̍Ō��NULL������
	//strncpy_s(in_file_path, sizeof(in_file_path), argv[1], _TRUNCATE);
	in_file_path = argv[1];
	split_file_and_path(in_file_path, path, file_name);

	// �o�̓t�@�C�������쐬
	out_file_path = path;
	out_file_path = out_file_path + "out_";
	out_file_path = out_file_path + file_name;
	printf("in_file_path=%s\n", in_file_path.c_str());
	printf("path=%s, file_name=%s\n", path.c_str(), file_name.c_str());
	printf("out_file_path=%s\n", out_file_path.c_str());

	// ���̓t�@�C���s�����擾
	total_lines = get_file_line(in_file_path);

	//
	// �I�[�v��
	//
	r_fret = fopen_s(&r_fp, in_file_path.c_str(), "r");
	if ((r_fret != 0) || (r_fp == 0)) {
		printf("[read] fopen_s() fopen_errno = %d\n", r_fret);
		return -1;
	}
	w_fret = fopen_s(&w_fp, out_file_path.c_str(), "w");
	if ((w_fret != 0) || (w_fp == 0)) {
		printf("[write] fopen_s() fopen_errno = %d\n", r_fret);
		return -1;
	}

	//
	// �ǂݍ���
	//

	// ���K�\��
	regex pattern("([0-9]{4})");
	string replacement = "$1\t";

	while(fgets(read_buf, DEF_READBUF_SIZE, r_fp) != NULL) {

		//
		// �V�Η��\��
		//
		current_line++;
		rate_of_progress = ((float)current_line / (float)total_lines) * 100;
		// �J�[�\���ʒu���w��
		coord.X = 0;
		coord.Y = 0;
		//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		printf("Processing... %3.0f%%\n", rate_of_progress);

		//
		// �u��
		//
		string r_buf_string(read_buf);
		string out_str;
		//_regex_search(r_buf_string, pattern);
		_regex_replace(r_buf_string, pattern, replacement, out_str);
		printf("%s(%zd,%zd) > %s", 
			r_buf_string.c_str(), 
			r_buf_string.length(),
			r_buf_string.size(),
			out_str.c_str()
		);
		fputs(out_str.c_str(), w_fp);

		Sleep(2000);
	}

	//
	// �N���[�Y
	//
	fclose(r_fp);
	fclose(w_fp);

	return 0;
}

