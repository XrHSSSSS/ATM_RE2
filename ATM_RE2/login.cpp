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

    // 清空账户数据缓存
    account_count = 0;
    memset(accounts, 0, sizeof(accounts));

    // 打印调试信息
    printf("当前工作目录：");
    _getcwd(password_str, sizeof(password_str)); // 复用password_str缓冲
    printf("%s\n", password_str);
    display_bonjupt();
    // 输入账号
    printf("请输入账号: ");
    if (scanf("%19s", input_id) != 1) {
        fprintf(stderr, "[错误] 账号输入无效\n");
        return -1;
    }

    // 彻底清空输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // 加载账户数据
    if (!read_account("accounts.txt")) {
        fprintf(stderr, "[严重错误] 账户文件读取失败\n");
        return -1;
    }

    // 调试：打印加载的账户
    printf("\n=== 调试信息 ===\n");
    printf("已加载账户数: %d\n", account_count);
    for (int i = 0; i < account_count; i++) {
        printf("账户[%d] ID: %s\n", i, accounts[i].ID);
    }

    // 查找账户索引
    int account_index = -1;
    for (int i = 0; i < account_count; i++) {
        if (strcmp(input_id, accounts[i].ID) == 0) {
            account_index = i;
            printf("找到匹配账户[%d]\n", i); // 调试
            break;
        }
    }

    if (account_index == -1) {
        printf("\n错误：账号不存在\n");
        return -1;
    }
    printf("================\n");

    // 密码输入处理
    int attempts = 0;
    do {
        printf("请输入6位连续数字密码（剩余尝试次数 %d）: ",
            MAX_TRIES - attempts);

        if (fgets(password_str, sizeof(password_str), stdin) == NULL) {
            fprintf(stderr, "[错误] 密码输入失败\n");
            return -1;
        }

        // 去除换行符
        size_t len = strlen(password_str);
        if (len > 0 && password_str[len - 1] == '\n') {
            password_str[len - 1] = '\0';
            len--;
        }

        // 验证长度
        if (len != 6) {
            printf("密码必须为6位数字！\n");
            attempts++;
            continue;
        }

        // 转换数字
        int valid = 1;
        for (int i = 0; i < 6; i++) {
            if (!isdigit((unsigned char)password_str[i])) {
                valid = 0;
                break;
            }
            input_password[i] = password_str[i] - '0';
        }

        if (!valid) {
            printf("密码包含非数字字符！\n");
            attempts++;
            continue;
        }

        // 密码验证
        if (memcmp(input_password,
            accounts[account_index].password,
            sizeof(accounts[account_index].password)) != 0)
        {
            accounts[account_index].failed++;
            write_accounts_to_file();
            printf("密码错误！\n");
            attempts++;
        }
        else {
            break; // 密码正确
        }
    } while (attempts < MAX_TRIES);

    if (attempts >= MAX_TRIES) {
        printf("错误次数超限，账户已锁定！\n");
        return -1;
    }

    // 登录成功处理
    accounts[account_index].failed = 0;
    write_accounts_to_file();

    printf("\n=== 登录成功 ===\n");
    printf("账户索引: %d\n", account_index);
    printf("账户ID: %s\n", accounts[account_index].ID);
    printf("当前余额: %.2f\n", accounts[account_index].money);
    printf("================\n\n");

    return account_index;
}