#include <stdio.h>
#include "menu.h"
#include "login.h"
// ������


int main() 
{
    // �û���¼
    int current_account = login();

    if (current_account != -1) 
    {
        main_menu(current_account);  // ���ݵ�ǰ�˻�����
    }
    return 0;
}