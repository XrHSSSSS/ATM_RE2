#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "transacation.h"
#include "menu.h"
#include "account.h"
#include "logos.h"



// ���˵�����
void show_main_menu() {
    
    display_bonjupt();
    printf("\n\n\n\n");
    printf("           ****************************************\n");
    printf("           *          ===== ���˵� =====          *\n");
    printf("           *          1. �˻�����                 *\n");
    printf("           *          2. ȡ��                     *\n");
    printf("           *          3. ���                     *\n");
    printf("           *          4. ת��                     *\n");
    printf("           *          5. �˳�                     *\n");
    printf("           ****************************************\n");
    printf("��ѡ�����: ");
}

// һ���˵������˵�

void main_menu(int current_account_index) {
    int choice;
    while (1) {
        show_main_menu();  // ��ʾ���˵�

        // ������֤������û������Ƿ�Ϸ�
        if (scanf("%d", &choice) != 1) {
            // ���scanf����ֵ��Ϊ1��˵�����벻����Ч������
            while (getchar() != '\n');  // ��ջ�����
            system("cls");
            printf("��Ч�����룬������һ��������\n");
            continue;  // ���½���ѭ��
        }

        // ��������Ƿ��ںϷ���Χ��
        if (choice < 1 || choice > 5) {
            system("cls");
            printf("��Ч��ѡ�����������롣\n");
            continue;  // ���½���ѭ��
        }

        switch (choice) {
        case 1:
            system("cls");
            account_info_menu(current_account_index); // �˻�����˵�
            break;
        case 2:
            system("cls");
            get_cash(current_account_index); // ȡ��˵�
            break;
        case 3:
            system("cls");
            input_cash(current_account_index); // ���˵�
            break;
        case 4:
            system("cls");
            trans_money(current_account_index); // ת�˲˵�
            break;
        case 5:
            printf("ϵͳ����3����˳����뱣�ܺ������ֽ������п�����л��ʹ���������У�\n");
            Sleep(3000);
            return;
        }
    }
}







