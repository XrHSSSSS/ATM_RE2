#include <stdio.h>
#include <string.h>
#include <ctype.h>


// 简单的加密函数，每个字符加上偏移值
void encrypt(char* str, int offset) {
    while (*str) {
        if (isdigit(*str)) {
            int num = *str - '0';
            num = (num + offset) % 10;
            if (num < 0) num += 10;
            *str = '0' + num;
        }
        str++;
    }
}

// 解密函数，恢复原始字符
void decrypt(char* str, int offset) {
    while (*str) {
        if (isdigit(*str)) {
            int num = *str - '0';
            num = (num - offset) % 10;
            if (num < 0) num += 10;
            *str = '0' + num;
        }
        str++;
    }
}