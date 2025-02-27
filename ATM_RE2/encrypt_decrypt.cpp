#include <stdio.h>
#include <string.h>
#include <ctype.h>


// �򵥵ļ��ܺ�����ÿ���ַ�����ƫ��ֵ
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

// ���ܺ������ָ�ԭʼ�ַ�
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