#include <stdio.h>
#include "menu.h"
#include "login.h"
// 主函数


int main() 
{
    // 用户登录
    int current_account = login();

    if (current_account != -1) 
    {
        main_menu(current_account);  // 传递当前账户索引
    }
    return 0;
}