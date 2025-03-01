#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "file.h"

#define DEPOSIT_TYPE  1
#define WITHDRAWAL_TYPE 2  // 交易类型定义
#define TRANSFER_TYPE 3


// 取款函数
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>

void get_cash(int current_account_index) {
    // 重新加载最新账户数据
    if (!read_account("accounts.txt")) {
        fprintf(stderr, "[错误] 账户数据加载失败，无法进行取款操作\n");
        return;
    }

    // 验证账户索引有效性
    if (current_account_index < 0 || current_account_index >= account_count) {
        fprintf(stderr, "[错误] 无效的账户索引\n");
        return;
    }

    Account* current = &accounts[current_account_index];

    printf("\n=== 取款操作 ===\n");
    printf("当前可用余额: RMB %.2lf\n", current->money);

    double amount = 0;
    int input_attempts = 0;
    const int max_attempts = 3;

    // 带尝试限制的输入循环
    while (input_attempts < max_attempts) {
        printf("请输入取款金额（0取消操作）: ");

        // 清理输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // 获取用户输入
        char input[50];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("输入错误，请重新输入（剩余尝试次数: %d）\n", max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 移除输入字符串中的换行符
        input[strcspn(input, "\n")] = 0;

        // 取消操作
        if (strcmp(input, "0") == 0) {
            printf("取款操作已取消\n");
            return;
        }

        // 检查输入是否为有效的数字
        int is_valid = 1;
        int dot_count = 0;

        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] == '.') {
                dot_count++;
            }
            else if (!isdigit(input[i]) && input[i] != '.') {
                is_valid = 0;
                break;
            }
        }

        // 如果有多个小数点或者包含非法字符，输入无效
        if (dot_count > 1 || !is_valid) {
            printf("输入格式错误，请重新输入（剩余尝试次数: %d）\n",
                max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 将输入字符串转化为浮动金额
        if (sscanf(input, "%lf", &amount) != 1) {
            printf("输入格式错误，请重新输入（剩余尝试次数: %d）\n",
                max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 操作取消
        if (amount == 0) {
            printf("取款操作已取消\n");
            return;
        }

        // 金额验证
        if (amount <= 0) {
            printf("金额必须大于零（剩余尝试次数: %d）\n",
                max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 检查小数精度
        if (amount * 100 != (double)((int)(amount * 100))) {
            printf("金额最多支持两位小数（剩余尝试次数: %d）\n",
                max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        break;
    }

    // 超过最大尝试次数
    if (input_attempts >= max_attempts) {
        printf("\n错误：连续输入错误超过%d次，返回主菜单\n", max_attempts);
        return;
    }

    // 余额验证
    if (current->money < amount) {
        printf("\n错误：账户余额不足（当前余额: RMB %.2lf）\n", current->money);
        return;
    }

    // 执行扣款
    current->money -= amount;

    // 保存数据
    write_accounts_to_file();

    // 生成交易记录（金额显示为负数表示支出）
    generate_transaction(WITHDRAWAL_TYPE, NULL, -amount);

    // 操作结果
    printf("\n取款成功！\n");
    printf("实际取出金额: RMB %.2lf\n", amount);
    printf("最新账户余额: RMB %.2lf\n", current->money);

    // 打印交易凭条
    printf("\n=== 交易凭条 ===\n");
    printf("交易类型: 取款\n");
    printf("交易账号: %s\n", current->ID);
    printf("交易金额: RMB %.2lf\n", amount);
    printf("剩余余额: RMB %.2lf\n", current->money);
    printf("交易时间: ");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    printf("====================\n");
    Sleep(3000);
}







//存款函数
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

void input_cash(int current_account_index) {
    // 重新加载账户数据，确保数据最新
    if (!read_account("accounts.txt")) {
        fprintf(stderr, "[错误] 账户数据加载失败，无法进行存款操作\n");
        return;
    }

    // 验证账户索引有效性
    if (current_account_index < 0 || current_account_index >= account_count) {
        fprintf(stderr, "[错误] 无效的账户索引！\n");
        return;
    }

    Account* current = &accounts[current_account_index];
    printf("\n=== 存款操作 ===\n");
    printf("当前账户余额: RMB %.2lf\n", current->money);

    double amount = 0.0;
    int input_attempts = 0;
    const int MAX_ATTEMPTS = 3;

    // 输入循环（最多尝试3次）
    while (input_attempts < MAX_ATTEMPTS) {
        printf("请输入存款金额（0取消操作，最多两位小数）: ");

        // 清空输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // 获取用户输入的字符串
        char input[50];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("输入错误，请重新输入（剩余尝试次数: %d）\n",
                MAX_ATTEMPTS - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 去除换行符
        input[strcspn(input, "\n")] = 0;

        // 用户取消操作
        if (strcmp(input, "0") == 0) {
            printf("操作已取消。\n");
            return;
        }

        // 检查输入格式是否合法
        int is_valid = 1;
        int dot_count = 0;

        // 遍历输入，检查非法字符
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] == '.') {
                dot_count++;
            }
            else if (!isdigit(input[i])) {
                is_valid = 0;
                break;
            }
        }

        // 检查小数点数量及是否合法
        if (dot_count > 1 || !is_valid) {
            printf("输入格式错误，请重新输入（剩余尝试次数: %d）\n",
                MAX_ATTEMPTS - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 将字符串转为浮动金额
        if (sscanf(input, "%lf", &amount) != 1) {
            printf("输入格式错误，请重新输入（剩余尝试次数: %d）\n",
                MAX_ATTEMPTS - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 用户取消操作
        if (amount == 0) {
            printf("操作已取消。\n");
            return;
        }

        // 验证金额有效性
        if (amount <= 0) {
            printf("金额必须大于零（剩余尝试次数: %d）\n",
                MAX_ATTEMPTS - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 检查小数位数（最多两位）
        if (round(amount * 100) != amount * 100) {
            printf("金额最多支持两位小数（剩余尝试次数: %d）\n",
                MAX_ATTEMPTS - input_attempts - 1);
            input_attempts++;
            continue;
        }

        break; // 输入有效，退出循环
    }

    // 超过最大尝试次数
    if (input_attempts >= MAX_ATTEMPTS) {
        printf("\n错误：输入错误超过 %d 次，操作终止。\n", MAX_ATTEMPTS);
        return;
    }

    // 更新账户余额
    current->money += amount;

    // 保存数据
    write_accounts_to_file();

    // 生成存款交易记录（金额为正数）
    generate_transaction(DEPOSIT_TYPE, NULL, amount);

    // 用户反馈
    printf("\n存款成功！\n");
    printf("存入金额: RMB %.2lf\n", amount);
    printf("最新余额: RMB %.2lf\n", current->money);

    // 打印交易凭条
    printf("\n=== 交易凭条 ===\n");
    printf("账户: %s\n", current->ID);
    printf("类型: 存款\n");
    printf("金额: RMB %.2lf\n", amount);
    printf("余额: RMB %.2lf\n", current->money);
    printf("时间: ");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    printf("====================\n");
    Sleep(3000);
}




//转账函数
void trans_money(int current_account_index) {
    // 重新加载账户数据，确保数据最新
    if (!read_account("accounts.txt")) {
        fprintf(stderr, "[错误] 账户数据加载失败，无法进行转账操作\n");
        return;
    }

    // 验证账户索引有效性
    if (current_account_index < 0 || current_account_index >= account_count) {
        fprintf(stderr, "[错误] 无效的账户索引\n");
        return;
    }

    Account* current = &accounts[current_account_index];
    printf("\n=== 转账操作 ===\n");
    printf("当前账户余额: RMB %.2lf\n", current->money);

    // 输入转账目标账户ID
    char target_account_id[20];
    printf("请输入转账目标账户ID: ");
    if (scanf("%s", target_account_id) != 1) {
        printf("输入无效，请重新输入目标账户ID\n");
        return;
    }

    // 查找目标账户
    Account* target = NULL;
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].ID, target_account_id) == 0) {
            target = &accounts[i];
            break;
        }
    }

    if (target == NULL) {
        printf("错误：目标账户ID不存在\n");
        return;
    }

    double amount = 0;
    int input_attempts = 0;
    const int max_attempts = 3;

    // 带尝试限制的输入循环
    while (input_attempts < max_attempts) {
        printf("请输入转账金额（0取消操作，最多两位小数）: ");

        // 清理输入缓冲区
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // 获取用户输入的字符串
        char input[50];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("输入无效，请重新输入（剩余尝试次数: %d）\n", max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 去除换行符
        input[strcspn(input, "\n")] = 0;

        // 用户取消操作
        if (strcmp(input, "0") == 0) {
            printf("操作已取消。\n");
            return;
        }

        // 检查输入格式是否合法
        int is_valid = 1;
        int dot_count = 0;

        // 遍历输入，检查非法字符
        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] == '.') {
                dot_count++;
            }
            else if (!isdigit(input[i])) {
                is_valid = 0;
                break;
            }
        }

        // 检查小数点数量及是否合法
        if (dot_count > 1 || !is_valid) {
            printf("输入格式错误，请重新输入（剩余尝试次数: %d）\n", max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 将字符串转为浮动金额
        if (sscanf(input, "%lf", &amount) != 1) {
            printf("输入格式错误，请重新输入（剩余尝试次数: %d）\n", max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 用户取消操作
        if (amount == 0) {
            printf("操作已取消。\n");
            return;
        }

        // 验证金额有效性
        if (amount <= 0) {
            printf("金额必须大于零（剩余尝试次数: %d）\n", max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        // 检查小数位数（最多两位）
        if (round(amount * 100) != amount * 100) {
            printf("金额最多支持两位小数（剩余尝试次数: %d）\n", max_attempts - input_attempts - 1);
            input_attempts++;
            continue;
        }

        break; // 输入有效，退出循环
    }

    // 超过最大尝试次数
    if (input_attempts >= max_attempts) {
        printf("\n错误：连续输入错误超过 %d 次，返回主菜单\n", max_attempts);
        return;
    }

    // 余额验证
    if (current->money < amount) {
        printf("\n错误：账户余额不足（当前余额: RMB %.2lf）\n", current->money);
        return;
    }

    // 扣除转账金额
    current->money -= amount;
    target->money += amount; // 增加目标账户的余额

    // 保存数据
    write_accounts_to_file();

    // 生成转账交易记录（转账金额为负数表示支出）
    generate_transaction(TRANSFER_TYPE, target_account_id, amount);


    // 操作结果
    printf("\n转账成功！\n");
    printf("转账金额: RMB %.2lf\n", amount);
    printf("转账后余额: RMB %.2lf\n", current->money);

    // 打印交易凭条
    printf("\n=== 交易凭条 ===\n");
    printf("交易类型: 转账\n");
    printf("交易账号: %s\n", current->ID);
    printf("目标账户: %s\n", target->ID);
    printf("转账金额: RMB %.2lf\n", amount);
    printf("剩余余额: RMB %.2lf\n", current->money);
    printf("交易时间: ");
    time_t now = time(NULL);
    printf("%s", ctime(&now));
    printf("====================\n");
    Sleep(3000);
}
