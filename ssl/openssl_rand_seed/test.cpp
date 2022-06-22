/*
1. c_cpp_properties.json 파일에 includepath 추가
   : "C:\\Program Files\\OpenSSL-Win64\\include"
2. tasks.json 파일에 아래 내용 작성해야 컴파일 가능
"-IC:/Program Files/OpenSSL-Win64/include",
                        "-LC:/Program Files/OpenSSL-Win64/lib",
                        "-llibcrypto"
*/
#include <stdio.h>
#include <locale.h>
#include <tchar.h>
#include <openssl/err.h>
#include <openssl/rand.h>
int _tmain(int argc, _TCHAR *argv[])
{
    int retVal = 0;
    // 랜덤 수의 길이는 64로 한다.
    int length = 64;
    // PRNG에 공급할 seed 생성
    // 생성할 랜덤 수 길이만큼의 버퍼 생성
    unsigned char *buffer = (unsigned char *)
        malloc(sizeof(unsigned char) * (length));
    RAND_seed(buffer, length);
    // PRNG 실행
    retVal = RAND_bytes(buffer, length);
    if (retVal <= 0)
    { // 에러가 발생한 경우
        printf("랜덤 수 생성시 에러가 발생했습니다.");
        return 0;
    }
    setlocale(LC_ALL, "Korean");//로케일 설정
    // 랜덤 수를 화면에 표시한다.
    wprintf(L"랜덤 수는 = ");
    for (int i = 0; i < length; i++)
        printf("%02X", buffer[i]);
    return 1;
}