#include<openssl/rsa.h>

int main()
{
    RSA *key;
    unsigned char cipher_text[256];
    unsigned char plain_text_sender[]="This is RSA test Program";
    unsigned char plain_text_receiver[256];
    unsigned int num;

    

//RSA자료구조 생성과  key생성 

    key=RSA_new();
    key=RSA_generate_key(1024,3,NULL,NULL);

//key유효성 검사

    if(RSA_check_key(key)==1)
        printf("validate key\n");
    

    printf("plaintext=%s\n",plain_text_sender);
    

// 암/복호화 실행 

    num=RSA_public_encrypt(sizeof(plain_text_sender)-1, plain_text_sender, cipher_text, key, RSA_PKCS1_PADDING);
    num=RSA_private_decrypt(num, cipher_text, plain_text_receiver, key, RSA_PKCS1_PADDING);
    

    plain_text_receiver[num]='\0';
    printf("plaintext=%s\n",plain_text_receiver);
}