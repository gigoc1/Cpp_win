#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// 읽어 들이는 바이트 수
#define KEY_LENGTH 2048
#define PUB_EXP 3

// void ErrorCheck_malloc();

int main(int argc, char **argv)
{
    // keypair생성!!
    // RSA_generate_key(<#int bits#>, <#unsigned long e#>, <#void (*callback)(int, int, void *)#>, <#void *cb_arg#>)
    RSA *keypair = RSA_generate_key(KEY_LENGTH, PUB_EXP, NULL, NULL);
    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());

    if (keypair == NULL)
    {
        printf("RSA_keygenerate_key error!! \n");
        return -1;
    }

    if (pri == NULL)
    {
        printf("allocated error!!\n");
        return -1;
    }

    if (pub == NULL)
    {
        printf("allocated error!!\n");
        return -1;
    }

    // keypair에게 pri와 pub의 존재를 알려주는 역할들!!
    // PEM_write_bio_RSAPrivateKey(<#BIO *bp#>, <#RSA *x#>, <#const EVP_CIPHER *enc#>, <#unsigned char *kstr#>, <#int klen#>, <#pem_password_cb *cb#>, <#void *u#>)
    // PEM_write_bio_RSAPublicKey(<#BIO *bp#>, <#const RSA *x#>)

    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, keypair);
    size_t pri_len = BIO_pending(pri);
    size_t pub_len = BIO_pending(pub);
    char *pri_key = (char *)malloc(pri_len + 1);
    char *pub_key = (char *)malloc(pub_len + 1);
    BIO_read(pri, pri_key, (int)pri_len);
    BIO_read(pub, pub_key, (int)pub_len);
    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    printf("\n%s\n%s\n", pri_key, pub_key);
    return 0;
}