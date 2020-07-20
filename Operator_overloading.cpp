#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

class CBigInt {
public:
    CBigInt() {
        this->value = "0";
        this->sign='+';
    };

    CBigInt(const char *val);

    CBigInt(const int val) {
        if (val < 0) {
            this->sign = '-';
            this->value = to_string(val);
            this->value.erase(this->value.begin());
        } else {
            this->sign='+';
            this->value = to_string(val);
        }
    }

    CBigInt(const CBigInt &val) {
        this->value = strnum(this->strnum(val.value));
        if (this->value[0] == '-') {
            this->sign = '-';
            this->value.erase(this->value.begin());
        }
        else if (this->value[0]=='+'){
            this->sign = '+';
            this->value.erase(this->value.begin());
        }
        this->sign='+';
    }

    CBigInt &operator=(const CBigInt &rhs);

    friend CBigInt operator+(const CBigInt &lhs, const CBigInt &rhs);

    friend CBigInt operator*(const CBigInt &lhs, const CBigInt &rhs);

    CBigInt &operator+=(const CBigInt &rhs);

    CBigInt &operator*=(const CBigInt &rhs);

    friend ostream &operator<<(std::ostream &os, const CBigInt &x);

    friend istream &operator>>(std::istream &is, CBigInt &x);

    friend bool operator==(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator!=(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator<=(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator>=(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator<(const CBigInt &lhs, const CBigInt &rhs);

    friend bool operator>(const CBigInt &lhs, const CBigInt &rhs);

private:
    static string strnum(const string &in);

    static string count_sum(string str1_bak, string str2);

    static bool help_smaller(const string &str1, const string &str2);

    static string count_multiply(const string &str1, const string &str2);

    static string count_subtract(const string &str1, const string &str2, int smaller);

    string value="";
    char sign = ' ';
};
CBigInt::CBigInt(const char *val) {
    string out;
    string in = val;
    int begin=0;
    for (int i = 0, len = in.size(); i != len; ++i) {
        if ((in[i] == '0' && in[i + 1] == '0' && begin == 0) || (in[i] == '0' && in[i + 1] != '0' && isdigit(in[i + 1])&& begin ==0))
            continue;
        if ((in[i] == '-' && i==0) || isdigit(in[i]) || (in[i]=='+' && i==0)){
            out.push_back(in[i]);
            if(in[i]!='-' && in[i]!='+')begin=1;
        }
        else {
            throw invalid_argument("");
        }
    }
        this->value = out;
        if (this->value[0] == '-') {
            this->sign = '-';
            this->value.erase(this->value.begin());
        }
        else if (this->value[0]=='+'){
            this->sign='+';
            this->value.erase(this->value.begin());
        }
        else{
            this->sign='+';
        }

}

string CBigInt::strnum(const string &in) {
    string out;
    int trailing = 0;
    for (int i = 0, len = in.size(); i != len; ++i) {
        if (isspace(in[i]) || (in[i] == '0' && in[i + 1] == '0' && trailing == 0)) {
            if (trailing == 1 && in[i] == ' ') {
                out[i] = '\0';
                break;
            }
            continue;
        }
        if (in[i] != ' ' || in[i] == '-' || in[i]=='+'|| (in[i] == '0' && i == len - 1)) {
            if (in[i] == '0' && in[i + 1] != '0' && trailing == 0 && isdigit(in[i + 1]))continue;
            out.push_back(in[i]);
            trailing = 1;
        }
    }
    return out;
}

string CBigInt::count_sum(const string str1, const string str2) {
    string str1_bak = str1;
    string str2_bak = str2;

    if (str1_bak.length() > str2_bak.length())
        swap(str1_bak, str2_bak);
    string str = "";
    int n1 = str1_bak.length(), n2 = str2_bak.length();
    int diff = n2 - n1;
    int carry = 0;
    for (int i = n1 - 1; i >= 0; i--) {
        int sum = ((str1_bak[i] - '0') +
                   (str2_bak[i + diff] - '0') +
                   carry);
        str.push_back(sum % 10 + '0');
        carry = sum / 10;
    }
    for (int i = n2 - n1 - 1; i >= 0; i--) {
        int sum = ((str2_bak[i] - '0') + carry);
        str.push_back(sum % 10 + '0');
        carry = sum / 10;
    }
    if (carry)
        str.push_back(carry + '0');
    reverse(str.begin(), str.end());
    str=strnum(str);
    return str;
}

bool CBigInt::help_smaller(const string &str1, const string &str2) {
    int str1_bak = str1.length(), str2_bak = str2.length();
    if (str1_bak < str2_bak)
        return true;
    if (str2_bak < str1_bak)
        return false;

    for (int i = 0; i < str1_bak; i++)
        if (str1[i] < str2[i])
            return true;
        else if (str1[i] > str2[i])
            return false;

    return false;
}

string CBigInt::count_subtract(const string &str1, const string &str2, const int smaller) {
    string str1_bak = str1;
    string str2_bak = str2;
    if (smaller)swap(str1_bak, str2_bak);
    string str = "";
    int n1 = str1_bak.length(), n2 = str2_bak.length();
    int diff = n1 - n2;
    int carry = 0;
    for (int i = n2 - 1; i >= 0; i--) {
        int sub = ((str1_bak[i + diff] - '0') - (str2_bak[i] - '0') - carry);
        if (sub < 0) {
            sub = sub + 10;
            carry = 1;
        } else
            carry = 0;
        str.push_back(sub + '0');
    }
    for (int i = n1 - n2 - 1; i >= 0; i--) {
        if (str1_bak[i] == '0' && carry) {
            str.push_back('9');
            continue;
        }
        int sub = ((str1_bak[i] - '0') - carry);
        if (i > 0 || sub > 0) // remove preceding 0's
            str.push_back(sub + '0');
        carry = 0;
    }
    reverse(str.begin(), str.end());
    return str;
}

string CBigInt::count_multiply(const string &str1, const string &str2) {
    int len1 = str1.size();
    int len2 = str2.size();
    vector<int> result(len1 + len2, 0);
    int i_n1 = 0;
    int i_n2 = 0;
    for (int i = len1 - 1; i >= 0; i--) {
        int carry = 0;
        int n1 = str1[i] - '0';
        i_n2 = 0;
        for (int j = len2 - 1; j >= 0; j--) {
            int n2 = str2[j] - '0';
            int sum = n1 * n2 + result[i_n1 + i_n2] + carry;
            carry = sum / 10;
            result[i_n1 + i_n2] = sum % 10;
            i_n2++;
        }
        if (carry > 0)
            result[i_n1 + i_n2] += carry;
        i_n1++;
    }
    string s = "";
    int i = result.size() - 1;
    while (i >= 0 && result[i] == 0)
        i--;
    if (i == -1) {
        s = "0";
        return s;
    }
    while (i >= 0)
        s += to_string(result[i--]);
    s=strnum(s);
    return s;
}

CBigInt &CBigInt::operator=(const CBigInt &rhs) {
    this->sign=rhs.sign;
    this->value=rhs.value;
    return *this;
}

ostream &operator<<(std::ostream &os, const CBigInt &x) {
    if (x.sign == '-' && x.value != "0")
        os << '-' + CBigInt::strnum(x.value);
    else {
        os << CBigInt::strnum(x.value);
    }
    return os;
}

istream &operator>>(std::istream &is, CBigInt &x) {
    CBigInt input;
    input.value="";
    int trailing = 0;
    char valid_c;
    // Error handling and getting correct input in form(scanf( "%d")) would return correct input 
    if (!(isspace(is.peek())) && (!isdigit(is.peek())) && is.peek() != '-') {
        is.setstate(ios::failbit);
        return is;
    } else if ((is.peek() == '-') || isdigit(is.peek()) || isspace(is.peek())) {
        input.value = "";
        while (is >> noskipws >> valid_c) {
            if (isspace(valid_c) && trailing == 0)continue;
            else if ((valid_c == '0' && is.peek() == '0' && trailing==0) || (valid_c == '0' && is.peek() != '0' && trailing==0))
                continue;
            else if (valid_c == '-') {
                input.sign = valid_c;
                valid_c = is.peek();
                if (valid_c < '0' || valid_c > '9') {
                    is.setstate(ios::failbit);
                    return is;
                }
            } else if (isdigit(valid_c)) {
                trailing = 1;
                input.value.push_back(valid_c);
            } else if ((isspace(valid_c) && trailing == 1) || !isdigit(valid_c)) {
                break;
            }
        }
        if (x.value == "") {
            is.setstate(ios::failbit);
            return is;
        }
    }
    is.clear();
    x.value = input.value;
    x.sign = input.sign;
    return is;
}

CBigInt operator+(const CBigInt &lhs, const CBigInt &rhs) {
    CBigInt ret_sub_add;
    bool ret = CBigInt::help_smaller(lhs.value, rhs.value);
    if (lhs.sign == '-' && rhs.sign == '-') {
        ret_sub_add.value = CBigInt::count_sum(lhs.value, rhs.value);
        ret_sub_add.sign = '-';
    } else if (lhs.sign == '+' && rhs.sign == '+') {
        ret_sub_add.value = CBigInt::count_sum(lhs.value, rhs.value);
        ret_sub_add.sign = '+';
    } else if (lhs.sign == '-' && rhs.sign == '+') {
        ret_sub_add.value = CBigInt::count_subtract(lhs.value, rhs.value, ret);
        if (ret)ret_sub_add.sign = '+';
        else ret_sub_add.sign = '-';
    }z else if (lhs.sign == '+' && rhs.sign == '-') {
        ret_sub_add.value = CBigInt::count_subtract(lhs.value, rhs.value, ret);
        if (ret)ret_sub_add.sign = '-';
        else ret_sub_add.sign = '+';
    }
    ret_sub_add.value = CBigInt::strnum(ret_sub_add.value);
    return ret_sub_add;
}

CBigInt operator*(const CBigInt &lhs, const CBigInt &rhs) {
    CBigInt ret;
    if (lhs.value == "0" || rhs.value == "0") {
        ret.value = "0";
    } else if ((lhs.sign == '-' && rhs.sign == '+') || (lhs.sign == '+' && rhs.sign == '-')) {
        ret.sign = '-';
        ret.value = CBigInt::count_multiply(lhs.value, rhs.value);
    } else if ((lhs.sign == '+' && rhs.sign == '+') || (lhs.sign == '-' && rhs.sign == '-')) {
        ret.sign = '+';
        ret.value = CBigInt::count_multiply(lhs.value, rhs.value);
    }
    return ret;
}

CBigInt &CBigInt::operator*=(const CBigInt &rhs) {

    if (this->value == "0" || rhs.value == "0") {
        this->value = "0";
    } else if ((this->sign == '-' && rhs.sign == '+') || (this->sign == '+' && rhs.sign == '-')) {
        this->sign = '-';
        this->value = CBigInt::count_multiply(this->value, rhs.value);
    } else if ((this->sign == '+' && rhs.sign == '+') || (this->sign == '-' && rhs.sign == '-')) {
        this->sign = '+';
        this->value = CBigInt::count_multiply(this->value, rhs.value);
    }
    return *this;
}

CBigInt &CBigInt::operator+=(const CBigInt &rhs) {
    CBigInt ret_sub_add;
    bool ret = CBigInt::help_smaller(this->value, rhs.value);
    if (this->sign == '-' && rhs.sign == '-') {
        this->value = CBigInt::count_sum(this->value, rhs.value);
        this->sign = '-';
    } else if (this->sign == '+' && rhs.sign == '+') {
        this->value = CBigInt::count_sum(this->value, rhs.value);
        this->sign = '+';
    } else if (this->sign == '-' && rhs.sign == '+') {
        this->value = CBigInt::count_subtract(this->value, rhs.value, ret);
        if (ret)this->sign = '+';
        else this->sign = '-';
    } else if (this->sign == '+' && rhs.sign == '-') {
        this->value = CBigInt::count_subtract(this->value, rhs.value, ret);
        if (ret)this->sign = '-';
        else this->sign = '+';
    }
    return *this;
}

bool operator==(const CBigInt &lhs, const CBigInt &rhs) {
    if (lhs.value == "0" && rhs.value == "0")return true;
    return ((lhs.value == rhs.value) && (lhs.sign == rhs.sign));
}

bool operator!=(const CBigInt &lhs, const CBigInt &rhs) {
    if (lhs.value == "0" && rhs.value == "0")return false;
    return (lhs.value != rhs.value || rhs.sign != lhs.sign);
}

bool operator<(const CBigInt &lhs, const CBigInt &rhs) {
    if (lhs.value == "0" && rhs.value == "0")return false;
    if(lhs.value.length() != rhs.value.length())return lhs.value.length() < rhs.value.length();
    if(lhs.sign != rhs.sign)return lhs.sign > rhs.sign;
    return lhs.value < rhs.value;
}

bool operator>(const CBigInt &lhs, const CBigInt &rhs) {
    if (lhs.value == "0" && rhs.value == "0")return false;
    if(lhs.value.length() != rhs.value.length())return lhs.value.length() > rhs.value.length();
    if(lhs.sign != rhs.sign)return lhs.sign < rhs.sign;
    return lhs.value > rhs.value;
}

bool operator<=(const CBigInt &lhs, const CBigInt &rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>=(const CBigInt &lhs, const CBigInt &rhs) {
    return (lhs > rhs) || (lhs == rhs);
}




#ifndef __PROGTEST__

static bool equal(const CBigInt &x, const char *val) {
    ostringstream oss;
    oss << x;
    return oss.str() == val;
}

int main(void) {
    CBigInt a, b;
    istringstream is;

    a = 10;
    a += 20;
    assert ( equal ( a, "30" ) );
    a *= 5;
    assert ( equal ( a, "150" ) );
    b = a + 3;
    assert ( equal ( b, "153" ) );
    b = a * 7;
    assert ( equal ( b, "1050" ) );
    assert ( equal ( a, "150" ) );

    a = 10;
    a += -20;
    assert ( equal ( a, "-10" ) );
    a *= 5;
    assert ( equal ( a, "-50" ) );
    b = a + 73;
    assert ( equal ( b, "23" ) );
    b = a * -7;
    assert ( equal ( b, "350" ) );
    assert ( equal ( a, "-50" ) );


    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert ( equal ( a, "-87654321098765432109" ) );
    a *= "54321987654321987654";
    assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
    a *= 0;
    assert ( equal ( a, "0" ) );
    a = 10;
    b = a + "400";
    assert ( equal ( b, "410" ) );
    b = a * "15";
    assert ( equal ( b, "150" ) );
    assert ( equal ( a, "10" ) );

    is . clear ();
    is . str ( " 1234" );
    assert ( is >> b );
    assert ( equal ( b, "1234" ) );
    is . clear ();
    is . str ( " 12 34" );
    assert ( is >> b );
    assert ( equal ( b, "12" ) );
    is . clear ();
    is . str ( "999z" );
    assert ( is >> b );
    assert ( equal ( b, "999" ) );
    is . clear ();
    is . str ( "abcd" );
    assert ( ! ( is >> b ) );
    is . clear ();
    is . str ( "- 758" );
    assert ( ! ( is >> b ) );
    a = 42;

    try
    {
        a = "-xyz";
        assert ( "missing an exception" == NULL );
    }
    catch ( const invalid_argument & e )
    {
        assert ( equal ( a, "42" ) );
    }

    a = "73786976294838206464";
    assert ( a < "1361129467683753853853498429727072845824" );
    assert ( a <= "1361129467683753853853498429727072845824" );
    assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
    assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
    assert ( a != "1361129467683753853853498429727072845824" );
    assert ( ! ( a < "73786976294838206464" ) );
    assert ( a <= "73786976294838206464" );
    assert ( ! ( a > "73786976294838206464" ) );
    assert ( a >= "73786976294838206464" );
    assert ( a == "73786976294838206464" );
    assert ( ! ( a != "73786976294838206464" ) );
    assert ( a < "73786976294838206465" );
    assert ( a <= "73786976294838206465" );
    assert ( ! ( a > "73786976294838206465" ) );
    assert ( ! ( a >= "73786976294838206465" ) );
    assert ( ! ( a == "73786976294838206465" ) );
    assert ( a != "73786976294838206465" );
    a = "2147483648";
    assert ( ! ( a < -2147483648 ) );
    assert ( ! ( a <= -2147483648 ) );
    assert ( a > -2147483648 );
    assert ( a >= -2147483648 );
    assert ( ! ( a == -2147483648 ) );
    assert ( a != -2147483648 );



    return 0;
}

#endif /* __PROGTEST__ */
