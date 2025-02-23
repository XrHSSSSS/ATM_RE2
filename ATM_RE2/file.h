#ifndef FILE_H
#define FILE_H

#include <time.h>

// 定义常量
#define MAX_ACCOUNTS 100      // 最大账户数量
#define MAX_TRIES 3           // 密码最大尝试次数
#define TRANSACTION_TYPES 3   // 交易类型数量（存款/取款/转账）

// 账户结构体
typedef struct {
    char ID[20];       // 卡号
    char name[20];     // 姓名
    int failed;        // 密码错误次数
    int password[6];   // 加密后的密码（6位）
    double money;      // 余额
} Account;

// 交易流水结构体
typedef struct {
    char ID[20];           // 交易编号（如 T0001）
    char accountID[20];    // 交易账户ID
    char time[20];         // 交易时间（格式：YYYY-MM-DD HH:MM:SS）
    int type;              // 交易类型（1:存款, 2:取款, 3:转账）
    double money;          // 交易金额
    char toAccount[20];    // 对方账户（转账时使用）
} Statement;

// 全局变量声明
extern Account accounts[MAX_ACCOUNTS];  // 账户数组
extern int account_count;               // 实际账户数量
extern int transaction_counter;         // 交易流水计数器

// 函数声明
// 账户操作
int read_account(const char* filename);           // 读取所有账户
int write_account(const char* filename);          // 写入所有账户
int find_account_index(const char* account_id);   // 根据ID查找账户索引

// 交易流水操作
int read_transactions(const char* filename);      // 读取交易记录
int write_transaction(const char* filename, Statement* stmt);  // 写入单条交易
void generate_transaction(int type, const char* target_account, double amount);  // 生成交易记录

// 工具函数
void write_accounts_to_file();                    // 统一写入账户数据

#endif // FILE_H