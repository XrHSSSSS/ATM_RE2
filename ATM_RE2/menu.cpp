#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <string.h>
#include "transacation.h"
#include "menu.h"
#include "account.h"


// ���˵�����
void show_main_menu() {
    printf("\n===== ���˵� =====\n");
    printf("1. �˻�����\n");
    printf("2. ȡ��\n");
    printf("3. ���\n");
    printf("4. ת��\n");
    printf("5. �˳�\n");
    printf("��ѡ�����: ");
}

// һ���˵������˵�
void main_menu(int current_account_index) {
    int choice;
    while (1) {
        show_main_menu();  // ��ʾ���˵�
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            account_info_menu(current_account_index);// �˻�����˵�
            break;
        case 2:
            get_cash(current_account_index);// ȡ��˵�
            break;
        case 3:
            input_cash(current_account_index);// ���˵�
            break;
        case 4:
            trans_money(current_account_index); // ת�˲˵�
            break;
        case 5:
            printf("�˳�ϵͳ��\n");
            return;
        default:
            printf("��Ч��ѡ�����������롣\n");
        }
    }
}

// �˻�����˵�
void account_management_menu() {
    int choice;
    printf("\n===== �˻����� =====\n");

    printf("1. �޸�����\n");
    printf("2. �������˵�\n");
    printf("��ѡ�����: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        
        break;
    case 2:
        printf("�������˵���\n");
        break;
    default:
        printf("��Ч��ѡ�����������롣\n");
        break;
    }
}





