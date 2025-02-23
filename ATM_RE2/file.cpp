#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "file.h"

/* ================== 全局变量定义 ================== */
Account accounts[MAX_ACCOUNTS];  // 账户数据存储数组
int account_count = 0;           // 有效账户计数器
int transaction_counter = 0;     // 交易流水号生成器

/* ================== 账户文件操作 ================== */

/**
 * 从文件加载所有账户信息到内存
 * filename 账户文件名
 * return 1=成功 0=失败
 */
int read_account(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "[错误] 无法打开账户文件: %s\n", filename);
        return 0;
    }

    account_count = 0;  // 重置计数器
    memset(accounts, 0, sizeof(accounts));  // 清空内存

    while (account_count < MAX_ACCOUNTS) {
        // 使用安全输入函数读取数据
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

        // 验证读取字段数量
        if (ret == EOF) break;  // 文件结束
        if (ret != 10) {
            fprintf(stderr, "[警告] 账户文件格式错误（第%d行）\n", account_count + 1);
            fclose(file);
            return 0;
        }
        account_count++;
    }

    fclose(file);
    printf("已加载 %d 个账户\n", account_count);
    return 1;
}

/**
 * 将内存中的账户数据写入文件
 * filename 账户文件名
 * return 1=成功 0=失败
 */
int write_account(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "[错误] 无法创建账户文件: %s\n", filename);
        return 0;
    }

    for (int i = 0; i < account_count; i++) {
        // 使用统一格式写入数据
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

/* ================== 交易流水操作 ================== */

/**
 * 加载交易流水记录
 * filename 交易流水文件名
 * return 1=成功 0=失败
 */
int read_transactions(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[注意] 交易文件不存在，将创建新文件\n");
        return 1;  // 允许文件不存在
    }

    transaction_counter = 0;  // 重置计数器
    Statement temp;

    while (fscanf_s(file, "%19s %19s %19s %d %lf %19s",
        temp.ID, sizeof(temp.ID),
        temp.accountID, sizeof(temp.accountID),
        temp.time, sizeof(temp.time),
        &temp.type,
        &temp.money,
        temp.toAccount, sizeof(temp.toAccount)) == 6)
    {
        // 解析交易流水号
        if (sscanf(temp.ID, "T%04d", &transaction_counter) != 1) {
            fprintf(stderr, "[警告] 无效交易流水号: %s\n", temp.ID);
        }
        transaction_counter++;  // 确保计数器大于文件中的最大值
    }

    fclose(file);
    printf("当前交易流水号: T%04d\n", transaction_counter);
    return 1;
}

/*
 * 写入单条交易记录
 * filename 交易流水文件名
 * stmt 交易记录指针
 * return 1=成功 0=失败
 */
int write_transaction(const char* filename, Statement* stmt) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        fprintf(stderr, "[错误] 无法打开交易文件: %s\n", filename);
        return 0;
    }

    // 生成时间戳（ISO 8601格式）
    time_t now = time(NULL);
    struct tm* tm = localtime(&now);
    strftime(stmt->time, sizeof(stmt->time), "%Y-%m-%dT%H:%M:%S", tm);

    // 格式化写入
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
 * 生成交易记录（核心业务逻辑）
 * 交易类型 1=存款 2=取款 3=转账
 * target_account 目标账户（转账时使用）
 * amount 交易金额
 */
void generate_transaction(int type, const char* target_account, double amount) {
    Statement stmt;

    // 生成交易ID
    transaction_counter++;
    snprintf(stmt.ID, sizeof(stmt.ID), "T%04d", transaction_counter);

    // 设置账户信息
    strncpy(stmt.accountID, accounts[0].ID, sizeof(stmt.accountID) - 1); // 假设当前账户

    // 设置交易类型
    stmt.type = type;
    stmt.money = amount;

    // 处理目标账户
    if (target_account && strlen(target_account) > 0) {
        strncpy(stmt.toAccount, target_account, sizeof(stmt.toAccount) - 1);
        stmt.toAccount[sizeof(stmt.toAccount) - 1] = '\0';
    }
    else {
        strcpy(stmt.toAccount, "N/A");
    }

    // 写入文件
    if (!write_transaction("transactions.txt", &stmt)) {
        fprintf(stderr, "[严重] 交易记录丢失！交易号：%s\n", stmt.ID);
    }
}

/* ================== 辅助函数 ================== */

//统一保存账户数据（供外部调用）
 
void write_accounts_to_file() {
    if (!write_account("accounts.txt")) {
        fprintf(stderr, "[严重] 账户数据保存失败，系统即将终止！\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * 根据账户ID查找索引
 * account_id 要查找的账户ID
 * return 账户索引（-1表示未找到）
 */
int find_account_index(const char* account_id) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(account_id, accounts[i].ID) == 0) {
            return i;
        }
    }
    return -1;
}