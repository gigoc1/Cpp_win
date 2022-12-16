//reference: https://zerous0.tistory.com/6
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/rand.h> //openssl의 헤더파일
#include <openssl/evp.h>  //이것도

#include <openssl/provider.h>

int main(int argc, const char *argv[])
{
     /*
    legacy algorithm(DES-CBC) provider setting
    reference: https://github.com/openssl/openssl/blob/master/README-PROVIDERS.md
    */
    OSSL_PROVIDER *legacy;
    OSSL_PROVIDER *deflt;

    /* Load Multiple providers into the default (NULL) library context */
    deflt = OSSL_PROVIDER_load(NULL, "default");
    if (deflt == NULL) {
        printf("Failed to load Default provider\n");
        OSSL_PROVIDER_unload(legacy);
        exit(EXIT_FAILURE);
    }

    legacy = OSSL_PROVIDER_load(NULL, "legacy");
    if (legacy == NULL) {
        printf("Failed to load Legacy provider\n");
        exit(EXIT_FAILURE);
    }
    

    /*
     *
     * Enctypt
     *
     *
     */
    // EVP API를 써서 des_cbc를 하기위해 초가화 과정
    const EVP_CIPHER *c = EVP_des_cbc();
    // const EVP_CIPHER *c = EVP_aes_128_cbc();
    EVP_CIPHER_CTX *ctx_enc = EVP_CIPHER_CTX_new();
    if (ctx_enc == NULL)
    {
        printf("EVP_CIPHER_CTX_new is NULL error!!\n");
        return -1;
    }
    if ((!EVP_EncryptInit_ex(ctx_enc, c, NULL, NULL, NULL)))
    {                                           //콘텍스트에 EVP_des_cbc를 사용한다고 말해준다!!
        printf("EVP_EncryptInit_ex error!!\n"); //그래서 초반에 EVP_EncryptInit_ex()함수를 실행해준다.
        return -1;
    }

    /**************************************************************************************************
     key, iv initialized
     Random key, iv use RAND_bytes()
     **************************************************************************************************/
    printf("\n");
    printf("****************************Key, IV initailized********************************\n");
    printf("\n");
    unsigned char *key = (unsigned char *)malloc(EVP_CIPHER_CTX_key_length(ctx_enc)); // key
    unsigned char *iv = (unsigned char *)malloc(EVP_CIPHER_CTX_iv_length(ctx_enc));   // iv
    if (!RAND_bytes(key, EVP_CIPHER_CTX_key_length(ctx_enc)))
    {
        printf("RAND_bytes function error!!\n");
        return -1;
    }
    if (!RAND_bytes(iv, EVP_CIPHER_CTX_iv_length(ctx_enc)))
    {
        printf("RAND_bytes function error!!\n");
        return -1;
    }
    printf("Use key value : ");
    for (int i = 0; i < (int)strlen((const char *)key); i++) // key값 출력
        printf("0x%02X ", key[i]);
    printf("\nkey length = %d\n", (int)strlen((const char *)key));
    printf("\n");
    printf("Use iv value : ");
    for (int i = 0; i < (int)strlen((const char *)iv); i++) // iv값 출력
        printf("0x%02X ", iv[i]);
    printf("\niv length = %d\n", (int)strlen((const char *)iv));
    printf("\n");

    unsigned char *plain_txt = (unsigned char *)"Hello hihi my name is honggildong nice to meet you  nice to meet you too!!."; // Plain_Text
    printf("Plane_txt length = %d\n", (int)strlen((char *)plain_txt));
    unsigned char *cipher_txt = (unsigned char *)malloc(strlen((char *)plain_txt) + EVP_CIPHER_CTX_block_size(ctx_enc)); // cipher_txt

    int outl = 0, numBytesWritten = 0; //인덱스 값이 되는out_length와 numBytesWritten 변수 설정!
    printf("input plain Text : \n \t\t%s \n", plain_txt);
    EVP_EncryptInit_ex(ctx_enc, c, NULL, key, iv); //초기화 아까 한번 선언했지만 지금 선언한 이유는 key와 iv값을 알려주기 위해서이다!!!!!

    // 8의 배수단위로 즉 8바이트(DES는 64bit씩 암호화) 단위로 5번째 매개변수 숫자만큼 암호호화 시킨다
    EVP_EncryptUpdate(ctx_enc, &cipher_txt[outl], &numBytesWritten, &plain_txt[outl], (int)strlen((char *)plain_txt));
    outl += numBytesWritten;

    //나머지(패딩된 값들 암호화!!!
    EVP_EncryptFinal_ex(ctx_enc, &cipher_txt[outl], &numBytesWritten);
    outl += numBytesWritten;

    //동적할당 해줬으니 할당 풀어주자
    EVP_CIPHER_CTX_free(ctx_enc);
    printf("Cipher Text is : \n \t\t");
    printf("%s", cipher_txt);
    printf("\n");
    /*
         *
         *  Decryption
         *
         */
    // Decrypyt 부분은 EVP API특성상 인터페이스가 같으니 여기서 설명생략 ^^ ㅎ.ㅎ

    printf("\n");
    printf("******************************Decrypyion**********************************\n");
    printf("\n");
    const EVP_CIPHER *c_dec = EVP_des_cbc();
    EVP_CIPHER_CTX *ctx_dec = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx_dec, c_dec, NULL, key, iv);
    int lenOfCiphertxt = outl; // cipher_txt_length
    unsigned char *dec_plain_txt = (unsigned char *)malloc(lenOfCiphertxt + EVP_CIPHER_CTX_block_size(ctx_dec) + 1);
    int outl_dec = 0, tmp_dec = 0, cipher_idx = 0;

    printf("lenOfCiphertxt : %d \n", lenOfCiphertxt);
    EVP_DecryptUpdate(ctx_dec, &dec_plain_txt[outl_dec], &tmp_dec, &cipher_txt[outl_dec], lenOfCiphertxt);
    outl_dec += tmp_dec;
    cipher_idx += tmp_dec;

    EVP_DecryptFinal_ex(ctx_dec, &dec_plain_txt[outl_dec], &tmp_dec);
    outl_dec += tmp_dec;
    dec_plain_txt[outl_dec] = '\0';

    printf("\n");
    printf("Decrypted: >>%s<<\n", dec_plain_txt);
    printf("\n");

    /* Rest of application */

    OSSL_PROVIDER_unload(legacy);
    OSSL_PROVIDER_unload(deflt);
    exit(EXIT_SUCCESS);

    return 0; //메인 끝나는 부분
}