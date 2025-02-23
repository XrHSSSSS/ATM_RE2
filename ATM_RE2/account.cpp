#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "file.h"
#include "transacation.h"
#include "account.h" // 添加此行

#define DEPOSIT_TYPE  1
#define WITHDRAWAL_TYPE 2  // 交易类型定义
#define TRANSFER_TYPE 3

#define MAX_TRY 3
#define ACCOUNT_FILE "accounts.txt"
#define TRANSACTION_FILE "transactions.txt"

void account_info_menu(int current_account_index) {
    int choice;
    while (1) {
        printf("\n===== 账户信息管理 =====\n");
        printf("1. 修改密码\n");
        printf("2. 查看账户信息\n");
        printf("3. 查询余额\n");
        printf("4. 查看交易记录\n");
        printf("5. 返回主菜单\n");
        printf("请选择操作: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            change_password(current_account_index);
            break;
        case 2:
            show_account_info(current_account_index);
            break;
        case 3:
            show_balance(current_account_index);
            break;
        case 4:
            show_transaction_history(current_account_index);
            break;
        case 5:
            return; // 返回上级菜单
        default:
            printf("无效输入，请重新选择！\n");
        }
    }
}

/**
 * @brief 修改密码
 */
void change_password(int current_account_index) {
    // 重新加载账户数据
    if (!read_account(ACCOUNT_FILE)) {
        printf("错误：账户数据加载失败！\n");
        return;
    }

    Account* current = &accounts[current_account_index];
    int old_password[6], new_password[6], confirm_password[6];
    char input[7];

    // 输入旧密码
    printf("\n=== 修改密码 ===\n");
    printf("请输入旧密码（6位数字）: ");
    if (scanf("%6s", input) != 1) {
        printf("输入错误！\n");
        return;
    }
    for (int i = 0; i < 6; i++) {
        if (!isdigit(input[i])) {
            printf("密码必须为6位数字！\n");
            return;
        }
        old_password[i] = input[i] - '0';
    }

    // 验证旧密码
    if (memcmp(old_password, current->password, sizeof(current->password)) != 0) {
        printf("旧密码错误！\n");
        return;
    }

    // 输入新密码
    printf("请输入新密码（6位数字）: ");
    if (scanf("%6s", input) != 1) {
        printf("输入错误！\n");
        return;
    }
    for (int i = 0; i < 6; i++) {
        if (!isdigit(input[i])) {
            printf("密码必须为6位数字！\n");
            return;
        }
        new_password[i] = input[i] - '0';
    }

    // 确认新密码
    printf("请再次输入新密码: ");
    if (scanf("%6s", input) != 1) {
        printf("输入错误！\n");
        return;
    }
    for (int i = 0; i < 6; i++) {
        confirm_password[i] = input[i] - '0';
    }

    // 验证一致性
    if (memcmp(new_password, confirm_password, sizeof(new_password)) != 0) {
        printf("两次输入的密码不一致！\n");
        return;
    }

    // 更新密码
    memcpy(current->password, new_password, sizeof(current->password));
    write_accounts_to_file();
    printf("密码修改成功！\n");
}

/**
 * @brief 显示账户信息
 */
void show_account_info(int current_account_index) {
    Account* current = &accounts[current_account_index];
    printf("\n=== 账户信息 ===\n");
    printf("账号: %s\n", current->ID);
    printf("姓名: %s\n", current->name);
    printf("密码错误次数: %d\n", current->failed);
    printf("余额: ¥%.2lf\n", current->money);
    printf("================\n");
}

/**
 * @brief 显示余额
 */
void show_balance(int current_account_index) {
    Account* current = &accounts[current_account_index];
    printf("\n当前余额: ¥%.2lf\n", current->money);
}

/**
 * @brief 查询交易记录
 */
void show_transaction_history(int current_account_index) {
    Account* current = &accounts[current_account_index];
    printf("\n=== 交易记录 ===\n");

    // 加载交易记录
    FILE* file = fopen(TRANSACTION_FILE, "r");
    if (!file) {
        printf("暂无交易记录！\n");
        return;
    }

    Statement stmt;
    int count = 0;
    while (fscanf(file, "%s %s %s %d %lf %s",
        stmt.ID, stmt.accountID, stmt.time,
        &stmt.type, &stmt.money, stmt.toAccount) == 6)
    {
        if (strcmp(stmt.accountID, current->ID) == 0) {
            // 格式化输出
            printf("\n时间: %s\n", stmt.time);
            printf("类型: ");
            switch (stmt.type) {
            case DEPOSIT_TYPE:  printf("存款"); break;
            case WITHDRAWAL_TYPE: printf("取款"); break;
            case TRANSFER_TYPE: printf("转账至 %s", stmt.toAccount); break;
            default:            printf("未知");
            }
            printf("\n金额: ¥%.2lf\n", stmt.money);
            count++;
        }
    }

    if (count == 0) {
        printf("暂无相关交易记录！\n");
    }
    fclose(file);
}

