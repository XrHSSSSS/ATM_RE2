#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include "transacation.h"
#include "menu.h"
#include "account.h"


// 主菜单函数
void show_main_menu() {
    printf("\n===== 主菜单 =====\n");
    printf("1. 账户管理\n");
    printf("2. 取款\n");
    printf("3. 存款\n");
    printf("4. 转账\n");
    printf("5. 退出\n");
    printf("请选择操作: ");
}

// 一级菜单：主菜单
void main_menu(int current_account_index) {
    int choice;
    while (1) {
        show_main_menu();  // 显示主菜单
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            account_info_menu(current_account_index);// 账户管理菜单
            break;
        case 2:
            get_cash(current_account_index);// 取款菜单
            break;
        case 3:
            input_cash(current_account_index);// 存款菜单
            break;
        case 4:
            trans_money(current_account_index); // 转账菜单
            break;
        case 5:
            printf("退出系统。\n");
            return;
        default:
            printf("无效的选择，请重新输入。\n");
        }
    }
}

// 账户管理菜单
void account_management_menu() {
    int choice;
    printf("\n===== 账户管理 =====\n");

    printf("1. 修改密码\n");
    printf("2. 返回主菜单\n");
    printf("请选择操作: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        
        break;
    case 2:
        printf("返回主菜单。\n");
        break;
    default:
        printf("无效的选择，请重新输入。\n");
        break;
    }
}





