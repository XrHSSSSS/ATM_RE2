#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H


void account_info_menu(int current_account_index); // 账户信息菜单
void change_password(int current_account_index);    // 修改密码
void show_account_info(int current_account_index);  // 显示账户信息
void show_balance(int current_account_index);       // 显示余额
void show_transaction_history(int current_account_index); // 交易记录查询

#endif // !ACCOUNT_H

