#ifndef USER_H
#define USER_H

#include "common.h"

void userRegister(void);
int userLogin(void);
void loadUsers(void);
void saveUsers(void);
void userInfo(void);
void userModifyPassword(void);
void userDelete(void);

#endif