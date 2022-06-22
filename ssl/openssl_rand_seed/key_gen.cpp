#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/rand.h>
int _tmain(int argc, _TCHAR *argv[])
{
    // salt를 저장할 버퍼 길이는 8로 한다.
    unsigned char salt[8];
    // 암호화 구조체 EVP_CIPHER의 포인터 변수 생성
    const EVP_CIPHER *cipher = NULL;
    // 패스워드를 저장할 포인터, 패스워드는“aaaa”로 넣는다.
    char *password = "aaaa";
    // 키와 IV가 저장될 변수를 정의하고 길이는 OpenSSL에서 알아서 정해준다.
    unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
    int ret = 0;
    // PRNG를 통해 랜덤 수를 만들고 그 값을 Salt에 저장한다. 길이는 8
    ret = RAND_pseudo_bytes(salt, 8);
    // PRNG에서 에러가 발생할 경우 에러 메시지 출력하고 프로그램을 종료한다.
    if (ret < 0)
    {
        printf("랜덤 수를 생성할 수 없습니다.");
        return 0;
    }
    // 암호화 구조체의 인스턴스를 생성. 여기서는 DES의 ECB 모드의 암호화 구조체 생성.
    cipher = EVP_des_ecb();
    // 키와 IV를 생성함. 인자는 암호화 구조체, 다이제스트 구조체, salt 값, 패스워드
    // 카운트는 생성될 키와 IV를 저장할 변수
    // 다이제스트 구조체는 EVP_md5()함수를 통해 생성. 카운트는 한번
    EVP_BytesToKey(cipher, EVP_md5(), salt,
                   (unsigned char *)password, strlen(password), 1, key, iv);
    // salt 값을 화면에 표시
    for (int i = 0; i < sizeof(salt); i++)
    {
        printf("salt is :%02X\n", salt[i]);
    }
    // 키 값을 화면에 표시
    if (EVP_CIPHER_key_length(cipher) > 0)
    {
        for (int i = 0; i < EVP_CIPHER_key_length(cipher); i++)
        {
            printf("key is :%02X\n", key[i]);
        }
    }
    // IV 값을 화면에 표시
    if (EVP_CIPHER_iv_length(cipher) > 0)
    {
        for (int i = 0; i < EVP_CIPHER_iv_length(cipher); i++)
        {
            printf("iv is :%02X\n", iv[i]);
        }
    }
    return 0;
}