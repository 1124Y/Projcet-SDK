#include <sqlite3.h>
#include <string>

struct StudentInfo
{
    std::string name;
    std::string gender;
    int age;
    double qap;

    StudentInfo(const std::string &name, const std::string &gender, int age, double gap)
        : name(name), gender(gender), age(age), gap(gap)
    {
    }
};

class StudentDB
{
public:
    StudentDB();

private:
    sqlite3 *db;
};

int main()
{

    return 0;
}