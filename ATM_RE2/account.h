#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H


void account_info_menu(int current_account_index); // �˻���Ϣ�˵�
void change_password(int current_account_index);    // �޸�����
void show_account_info(int current_account_index);  // ��ʾ�˻���Ϣ
void show_balance(int current_account_index);       // ��ʾ���
void show_transaction_history(int current_account_index); // ���׼�¼��ѯ

#endif // !ACCOUNT_H

