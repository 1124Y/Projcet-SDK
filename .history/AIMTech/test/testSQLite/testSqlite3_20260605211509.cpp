#include <sqlite3.h>
#include <string>
#include <iostream>
struct StudentInfo
{
    std::string name;
    std::string gender;
    int age;
    double gap;

    StudentInfo(const std::string &name, const std::string &gender, int age, double gap)
        : name(name), gender(gender), age(age), gap(gap)
    {
    }
};

class StudentDB
{
public:
    StudentDB(const std::string &dbName)
    {
        // 创建并打开数据库
        int rc = sqlite3_open(dbName.c_str(), &_db);
        if (rc != SQLITE_OK)
        {
            std::cerr << "打开数据库失败：" << sqlite3_errmsg(_db) << std::endl;
            sqlite3_close(_db);
        }
        // 初始化数据库表 - 创建学生信息表
        if (!initDataBase())
        {
            sqlite3_close(_db);
        }
    }

private:
    sqlite3 *_db;
};

int main()
{

    return 0;
}