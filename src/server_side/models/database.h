#include<iostream>
#include"Player.h"
#include"position.h"
#include<sqlite3.h>
#include<memory>

class DataBase{
    sqlite3* DB;
    std::string sql;
    int exit=0;
    char* messageError;
    static int friendsId;
public:
    void createTables();
    void insertPlayer(unsigned int);
    void verifyTable();
    void insertBoard(int,int);
    void insertFriend(int,int);
    void searchFriends(int);
    void reloadFile(std::string file);
};

