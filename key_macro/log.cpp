#include "stdafx.h"
/*
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include "log.h"


static int  _log_file_size = 10000000;
static char _log_file_name[MAX_PATH+1] = "log.txt";


void _limit_file_size(char *file)
{
	struct stat buf[1];

	if(0 <= stat(file,buf)){					// 파일의 상태를 읽어옴
		if(buf->st_size > _log_file_size){		// 파일이 제한값을 넘어갈 때
			char file_name_old[256];

			strcpy(file_name_old, file);
			strcat(file_name_old, ".old");			// .old 파일이름 생성
			
			remove(file_name_old);					// .old 파일이 존재하면 삭제
			rename(file,file_name_old);				// .old 파일로 복사
		}
	}
}

static int _get_current_time (char *text, int len)
{
	struct _timeb timebuffer;
	struct tm tt;

	_ftime( &timebuffer );
	tt = *localtime(&timebuffer.time);

	return _snprintf (text, len, "%02d:%02d:%02d.%03d ", 
		(int)tt.tm_hour, (int)tt.tm_min, (int)tt.tm_sec, (int)timebuffer.millitm);
}

void set_log_property(const char *log_file_name_, int max_file_size_)
{
	if (!log_file_name_) log_file_name_ = "";
	strncpy(_log_file_name, log_file_name_, MAX_PATH);
	_log_file_name[MAX_PATH] = '\0';
	_log_file_size = max_file_size_;

	if (_log_file_name[0]) {
		_limit_file_size(_log_file_name);
	}
}

void log (const char *format, ...)
{
	const int MAX_TXT = 1024;
	int n = 0;
	char log_text[MAX_TXT+1];
    va_list arg_list;

	if (_log_file_name[0]) {
		_limit_file_size(_log_file_name);
	}

	n += _get_current_time(log_text+n, MAX_TXT-n);
	va_start (arg_list,format);
	n += _vsnprintf (log_text+n, MAX_TXT-n, format, arg_list);
	va_end (arg_list);
	n += _snprintf (log_text+n, MAX_TXT-n, "\r\n");
	log_text[MAX_TXT] = '\0';

	if (_log_file_name[0]) {
		FILE *fp = fopen (_log_file_name, "ab");
		if(fp){
			fputs (log_text, fp);
			fclose(fp);
		}
	}
}
*/
