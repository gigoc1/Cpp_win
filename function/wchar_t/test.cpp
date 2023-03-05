//C언어 표준 라이브러리 함수 가이드
//size_t wcstombs(char *s, const wchar_t *pwcs, size_t n); 와이드 바이트 문자의 시퀀스를 멀티 문자의 시퀀스로 변환
//유니코드 스트링을 멀티 바이트 스트링으로 변환

#pragma warning(disable:4996)
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define target_name "Server_log_on"

int main(void )
{
    wchar_t wstr[]=L"abc가나다123";
    char str[100];
    int last;
        
    setlocale(LC_ALL, "Korean");//로케일 설정
    last = wcstombs(str,wstr, 100);//유니코드 스트링을 멀티바이트 스트링으로 변환
    str[last]=0;//종료 문자열 설정

    printf("%s\n",str);
    wprintf(L"%s\n",wstr);

    wprintf(L"%s\n", (wchar_t*)target_name);
    mbstowcs(wstr, target_name, sizeof(target_name));
    wprintf(L"%s\n",wstr);

    return 0;
}