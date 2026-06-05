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

    ~StudentDB()
    {
        if (_db != nullptr)
        {
            sqlite3_close(_db);
        }
    }

    bool insertStudentInfo(const StudentInfo &studentInfo)
    {
        // 插入学生信息
        std::string insertSQL = R"(
            INSERT INTO Student (name, gender, age, gap)
            VALUES (?, ?, ?, ?);
        )";

        // 准备SQL语句
        sqlite3_stmt *stmt;
        int rc = sqlite3_prepare_v2(_db, insertSQL.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "准备语句失败：" << sqlite3_errmsg(_db) << std::endl;
            return false;
        }
    }

private:
    bool initDataBase()
    {
        const std::string createTableSQL = R"(
            CREATE TABLE IF NOT EXISTS Student (
                stuid INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT,
                gender TEXT,
                age INTEGER,
                gap REAL
            );
        )";
        int rc = sqlite3_exec(_db, createTableSQL.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "创建表失败：" << sqlite3_errmsg(_db) << std::endl;
            return false;
        }
        return true;
    }

private:
    sqlite3 *_db;
};

int main()
{

    return 0;
}