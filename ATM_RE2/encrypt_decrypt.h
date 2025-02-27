#ifndef ENCRYPT_DECRYPT_H
#define ENCRYPT_DECRYPT_H

#include "encrypt_decrypt.h"

#define ENCRYPT_OFFSET 3


// 加密函数：对字符串中的每个字符加上指定的偏移量
void encrypt(char* str, int offset);

// 解密函数：对字符串中的每个字符减去指定的偏移量
void decrypt(char* str, int offset);
#endif

