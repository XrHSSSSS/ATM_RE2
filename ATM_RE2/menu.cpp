#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "transacation.h"
#include "menu.h"
#include "account.h"
#include "logos.h"



// 主菜单函数
void show_main_menu() {
    
    display_bonjupt();
    printf("\n\n\n\n");
    printf("           ****************************************\n");
    printf("           *          ===== 主菜单 =====          *\n");
    printf("           *          1. 账户管理                 *\n");
    printf("           *          2. 取款                     *\n");
    printf("           *          3. 存款                     *\n");
    printf("           *          4. 转账                     *\n");
    printf("           *          5. 退出                     *\n");
    printf("           ****************************************\n");
    printf("请选择操作: ");
}

// 一级菜单：主菜单

void main_menu(int current_account_index) {
    int choice;
    while (1) {
        show_main_menu();  // 显示主菜单

        // 输入验证：检查用户输入是否合法
        if (scanf("%d", &choice) != 1) {
            // 如果scanf返回值不为1，说明输入不是有效的整数
            while (getchar() != '\n');  // 清空缓冲区
            system("cls");
            printf("无效的输入，请输入一个整数。\n");
            continue;  // 重新进入循环
        }

        // 检查输入是否在合法范围内
        if (choice < 1 || choice > 5) {
            system("cls");
            printf("无效的选择，请重新输入。\n");
            continue;  // 重新进入循环
        }

        switch (choice) {
        case 1:
            system("cls");
            account_info_menu(current_account_index); // 账户管理菜单
            break;
        case 2:
            system("cls");
            get_cash(current_account_index); // 取款菜单
            break;
        case 3:
            system("cls");
            input_cash(current_account_index); // 存款菜单
            break;
        case 4:
            system("cls");
            trans_money(current_account_index); // 转账菜单
            break;
        case 5:
            printf("系统将在3秒后退出，请保管好您的现金与银行卡，感谢您使用南邮银行！\n");
            Sleep(3000);
            return;
        }
    }
}







