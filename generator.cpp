#include <iostream>
#include <string>

using namespace std;

bool validate(string str)
{
    unsigned int str_size = str.size();
    for(int i = 0; i < str_size; i++)
    {
        if(!(str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z' || str[i] >= '0' && str[i] <= '9'))
            return false;
    }
    return true;
}

int ProcessLogin(string login)
{
    uint32_t result = 0xffffffff;
    uint32_t temp = 0;

    for(int i = 0; i < login.size(); i++)
    {
        result = result ^ login[i];

        for(int j = 7; j >= 0; j--)
        {
            temp = result;
            /// <some asm>
            temp = temp & 1;
            temp *= (-1);
            //assuming loginHash is result
            result = (uint32_t)result >> 1;
            temp = temp & 0xedb88320;
            /// </some asm>

            result = result ^ temp;
        }
    }
    result = result ^ 0xffffffff;
    result = result & 0xff;
    return result;
}

/// convert char to binary string
string toBin(int a)
{
        string res = "00000000";
        for(int i = 7; i >= 0; i--)
        {
                res[i] = a%2 + '0';
                a/=2;
        }
        return res;
}

void pass_generate(int login, int N)
{
    /// соотношение единицы с разрядом
    string pass_db[8] = {"YY", "Y", "9", "YI", "YQ", "XZ", "YIk", "YX"};
    string log_bin = toBin(login);
    int counter = 0;
    int i = 0;
    string res = "";
    cout << log_bin << endl;

    for(i = 1; i < 8; i++)
    {
        if(log_bin[i] == '1')
        {
            res += pass_db[i];
            counter++;
        }
    }
    /// выравнивание старшего разряда
    if((counter + log_bin[0] - '0')%2) res += pass_db[0];
    cout << "Your passwords:\n";
    cout << res << endl;
    string temp;
    for(i = 0; i < res.size() && i < N-1; i++)
    {
        temp = res;
        temp.insert(i, "YYYY");
        cout << temp << endl;
    }
    temp = res;
    for(;i < N-1; i++)
    {
        temp += "YYYY";
        cout << temp << endl;
    }
}

int main()
{
    string login;
    int N;

    cout << "Enter login:\n";
    cin >> login;
    if(!validate(login))
    {
        cout << "Invalid login\n";
        return -1;
    }
    cout << "Number of required passwords\n(you really don't need more than 10-15 passwords)\n";
    cin >> N;
    pass_generate(ProcessLogin(login), N);
}
