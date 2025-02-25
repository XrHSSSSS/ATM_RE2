#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include <Windows.h>
#include "file.h"
#include "logos.h"


int login() {
    char input_id[20];
    int input_password[6];
    char password_str[7];

    // ����˻����ݻ���
    account_count = 0;
    memset(accounts, 0, sizeof(accounts));

    // ��ӡ������Ϣ
    printf("��ǰ����Ŀ¼��");
    _getcwd(password_str, sizeof(password_str)); // ����password_str����
    printf("%s\n", password_str);
    display_bonjupt();
    // �����˺�
    printf("�������˺�: ");
    if (scanf("%19s", input_id) != 1) {
        fprintf(stderr, "[����] �˺�������Ч\n");
        return -1;
    }

    // ����������뻺����
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // �����˻�����
    if (!read_account("accounts.txt")) {
        fprintf(stderr, "[���ش���] �˻��ļ���ȡʧ��\n");
        return -1;
    }

    // ���ԣ���ӡ���ص��˻�
    printf("\n=== ������Ϣ ===\n");
    printf("�Ѽ����˻���: %d\n", account_count);
    for (int i = 0; i < account_count; i++) {
        printf("�˻�[%d] ID: %s\n", i, accounts[i].ID);
    }

    // �����˻�����
    int account_index = -1;
    for (int i = 0; i < account_count; i++) {
        if (strcmp(input_id, accounts[i].ID) == 0) {
            account_index = i;
            printf("�ҵ�ƥ���˻�[%d]\n", i); // ����
            break;
        }
    }

    if (account_index == -1) {
        printf("\n�����˺Ų�����\n");
        return -1;
    }
    printf("================\n");

    // �������봦��
    int attempts = 0;
    do {
        printf("������6λ�����������루ʣ�ೢ�Դ��� %d��: ",
            MAX_TRIES - attempts);

        if (fgets(password_str, sizeof(password_str), stdin) == NULL) {
            fprintf(stderr, "[����] ��������ʧ��\n");
            return -1;
        }

        // ȥ�����з�
        size_t len = strlen(password_str);
        if (len > 0 && password_str[len - 1] == '\n') {
            password_str[len - 1] = '\0';
            len--;
        }

        // ��֤����
        if (len != 6) {
            printf("�������Ϊ6λ���֣�\n");
            attempts++;
            continue;
        }

        // ת������
        int valid = 1;
        for (int i = 0; i < 6; i++) {
            if (!isdigit((unsigned char)password_str[i])) {
                valid = 0;
                break;
            }
            input_password[i] = password_str[i] - '0';
        }

        if (!valid) {
            printf("��������������ַ���\n");
            attempts++;
            continue;
        }

        // ������֤
        if (memcmp(input_password,
            accounts[account_index].password,
            sizeof(accounts[account_index].password)) != 0)
        {
            accounts[account_index].failed++;
            write_accounts_to_file();
            printf("�������\n");
            attempts++;
        }
        else {
            break; // ������ȷ
        }
    } while (attempts < MAX_TRIES);

    if (attempts >= MAX_TRIES) {
        printf("����������ޣ��˻���������\n");
        return -1;
    }

    // ��¼�ɹ�����
    accounts[account_index].failed = 0;
    write_accounts_to_file();

    printf("\n=== ��¼�ɹ� ===\n");
    printf("�˻�����: %d\n", account_index);
    printf("�˻�ID: %s\n", accounts[account_index].ID);
    printf("��ǰ���: %.2f\n", accounts[account_index].money);
    printf("================\n\n");

    return account_index;
}