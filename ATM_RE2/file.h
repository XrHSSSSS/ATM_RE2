#ifndef FILE_H
#define FILE_H

#include <time.h>

// ���峣��
#define MAX_ACCOUNTS 100      // ����˻�����
#define MAX_TRIES 3           // ��������Դ���
#define TRANSACTION_TYPES 3   // �����������������/ȡ��/ת�ˣ�

// �˻��ṹ��
typedef struct {
    char ID[20];       // ����
    char name[20];     // ����
    int failed;        // ����������
    int password[6];   // ���ܺ�����루6λ��
    double money;      // ���
} Account;

// ������ˮ�ṹ��
typedef struct {
    char ID[20];           // ���ױ�ţ��� T0001��
    char accountID[20];    // �����˻�ID
    char time[20];         // ����ʱ�䣨��ʽ��YYYY-MM-DD HH:MM:SS��
    int type;              // �������ͣ�1:���, 2:ȡ��, 3:ת�ˣ�
    double money;          // ���׽��
    char toAccount[20];    // �Է��˻���ת��ʱʹ�ã�
} Statement;

// ȫ�ֱ�������
extern Account accounts[MAX_ACCOUNTS];  // �˻�����
extern int account_count;               // ʵ���˻�����
extern int transaction_counter;         // ������ˮ������

// ��������
// �˻�����
int read_account(const char* filename);           // ��ȡ�����˻�
int write_account(const char* filename);          // д�������˻�
int find_account_index(const char* account_id);   // ����ID�����˻�����

// ������ˮ����
int read_transactions(const char* filename);      // ��ȡ���׼�¼
int write_transaction(const char* filename, Statement* stmt);  // д�뵥������
void generate_transaction(int type, const char* target_account, double amount);  // ���ɽ��׼�¼

// ���ߺ���
void write_accounts_to_file();                    // ͳһд���˻�����

#endif // FILE_H