#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "file.h"

/* ================== ȫ�ֱ������� ================== */
Account accounts[MAX_ACCOUNTS];  // �˻����ݴ洢����
int account_count = 0;           // ��Ч�˻�������
int transaction_counter = 0;     // ������ˮ��������

/* ================== �˻��ļ����� ================== */

/**
 * ���ļ����������˻���Ϣ���ڴ�
 * filename �˻��ļ���
 * return 1=�ɹ� 0=ʧ��
 */
int read_account(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "[����] �޷����˻��ļ�: %s\n", filename);
        return 0;
    }

    account_count = 0;  // ���ü�����
    memset(accounts, 0, sizeof(accounts));  // ����ڴ�

    while (account_count < MAX_ACCOUNTS) {
        // ʹ�ð�ȫ���뺯����ȡ����
        int ret = fscanf_s(file, "%19s %19s %d %d %d %d %d %d %d %lf",
            accounts[account_count].ID, sizeof(accounts[account_count].ID),
            accounts[account_count].name, sizeof(accounts[account_count].name),
            &accounts[account_count].failed,
            &accounts[account_count].password[0],
            &accounts[account_count].password[1],
            &accounts[account_count].password[2],
            &accounts[account_count].password[3],
            &accounts[account_count].password[4],
            &accounts[account_count].password[5],
            &accounts[account_count].money);

        // ��֤��ȡ�ֶ�����
        if (ret == EOF) break;  // �ļ�����
        if (ret != 10) {
            fprintf(stderr, "[����] �˻��ļ���ʽ���󣨵�%d�У�\n", account_count + 1);
            fclose(file);
            return 0;
        }
        account_count++;
    }

    fclose(file);
    printf("�Ѽ��� %d ���˻�\n", account_count);
    return 1;
}

/**
 * ���ڴ��е��˻�����д���ļ�
 * filename �˻��ļ���
 * return 1=�ɹ� 0=ʧ��
 */
int write_account(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "[����] �޷������˻��ļ�: %s\n", filename);
        return 0;
    }

    for (int i = 0; i < account_count; i++) {
        // ʹ��ͳһ��ʽд������
        fprintf(file, "%s %s %d %d %d %d %d %d %d %.2lf\n",
            accounts[i].ID,
            accounts[i].name,
            accounts[i].failed,
            accounts[i].password[0],
            accounts[i].password[1],
            accounts[i].password[2],
            accounts[i].password[3],
            accounts[i].password[4],
            accounts[i].password[5],
            accounts[i].money);
    }

    fclose(file);
    return 1;
}

/* ================== ������ˮ���� ================== */

/**
 * ���ؽ�����ˮ��¼
 * filename ������ˮ�ļ���
 * return 1=�ɹ� 0=ʧ��
 */
int read_transactions(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[ע��] �����ļ������ڣ����������ļ�\n");
        return 1;  // �����ļ�������
    }

    transaction_counter = 0;  // ���ü�����
    Statement temp;

    while (fscanf_s(file, "%19s %19s %19s %d %lf %19s",
        temp.ID, sizeof(temp.ID),
        temp.accountID, sizeof(temp.accountID),
        temp.time, sizeof(temp.time),
        &temp.type,
        &temp.money,
        temp.toAccount, sizeof(temp.toAccount)) == 6)
    {
        // ����������ˮ��
        if (sscanf(temp.ID, "T%04d", &transaction_counter) != 1) {
            fprintf(stderr, "[����] ��Ч������ˮ��: %s\n", temp.ID);
        }
        transaction_counter++;  // ȷ�������������ļ��е����ֵ
    }

    fclose(file);
    printf("��ǰ������ˮ��: T%04d\n", transaction_counter);
    return 1;
}

/*
 * д�뵥�����׼�¼
 * filename ������ˮ�ļ���
 * stmt ���׼�¼ָ��
 * return 1=�ɹ� 0=ʧ��
 */
int write_transaction(const char* filename, Statement* stmt) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        fprintf(stderr, "[����] �޷��򿪽����ļ�: %s\n", filename);
        return 0;
    }

    // ����ʱ�����ISO 8601��ʽ��
    time_t now = time(NULL);
    struct tm* tm = localtime(&now);
    strftime(stmt->time, sizeof(stmt->time), "%Y-%m-%dT%H:%M:%S", tm);

    // ��ʽ��д��
    fprintf(file, "%s %s %s %d %.2lf %s\n",
        stmt->ID,
        stmt->accountID,
        stmt->time,
        stmt->type,
        stmt->money,
        stmt->toAccount);

    fclose(file);
    return 1;
}

/**
 * ���ɽ��׼�¼������ҵ���߼���
 * �������� 1=��� 2=ȡ�� 3=ת��
 * target_account Ŀ���˻���ת��ʱʹ�ã�
 * amount ���׽��
 */
void generate_transaction(int type, const char* target_account, double amount) {
    Statement stmt;

    // ���ɽ���ID
    transaction_counter++;
    snprintf(stmt.ID, sizeof(stmt.ID), "T%04d", transaction_counter);

    // �����˻���Ϣ
    strncpy(stmt.accountID, accounts[0].ID, sizeof(stmt.accountID) - 1); // ���赱ǰ�˻�

    // ���ý�������
    stmt.type = type;
    stmt.money = amount;

    // ����Ŀ���˻�
    if (target_account && strlen(target_account) > 0) {
        strncpy(stmt.toAccount, target_account, sizeof(stmt.toAccount) - 1);
        stmt.toAccount[sizeof(stmt.toAccount) - 1] = '\0';
    }
    else {
        strcpy(stmt.toAccount, "N/A");
    }

    // д���ļ�
    if (!write_transaction("transactions.txt", &stmt)) {
        fprintf(stderr, "[����] ���׼�¼��ʧ�����׺ţ�%s\n", stmt.ID);
    }
}

/* ================== �������� ================== */

//ͳһ�����˻����ݣ����ⲿ���ã�
 
void write_accounts_to_file() {
    if (!write_account("accounts.txt")) {
        fprintf(stderr, "[����] �˻����ݱ���ʧ�ܣ�ϵͳ������ֹ��\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * �����˻�ID��������
 * account_id Ҫ���ҵ��˻�ID
 * return �˻�������-1��ʾδ�ҵ���
 */
int find_account_index(const char* account_id) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(account_id, accounts[i].ID) == 0) {
            return i;
        }
    }
    return -1;
}