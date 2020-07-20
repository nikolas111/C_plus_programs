#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <algorithm>

using namespace std;
#endif /* __PROGTEST__ */


/*Custom Person type with @parameters: Addr, Name, Account, income, outcome*/
struct Person {
    string Addr;
    string Name;
    string Account;
    int income;
    int outcome;
};


/*
 * Comparator function for comparing firstly names
 * if they are equal then compare addresses
 * else it's enough to compare by names
*/
static bool cmp1(const Person &a, const Person &b) {
    if (a.Name == b.Name) {
        return a.Addr < b.Addr;
    } else {
        return a.Name < b.Name;
    }
}

class CIterator {
public:
    CIterator(vector<Person> v) {
        v_c = v;
        it_c = v.begin();
        size = v.size();
    };

    bool AtEnd(void) const;

    void Next(void);

    string Name(void) const;

    string Addr(void) const;

    string Account(void) const;

private:
    vector<Person>::iterator it_c; // iterator for the vector of Person structures
    vector<Person> v_c; // vector of Persons
    int size = 0;
    int index = 0;
};

/* Check if it is the end of the vector */
bool CIterator::AtEnd() const {
    return index == size;
}
/* setting the iterator to next element */
void CIterator::Next() {
    ++index;
}
/* get Account at index in vector v_c */
string CIterator::Account() const {
    return v_c[index].Account;
}

/* get Addr at index in vector v_c */
string CIterator::Addr() const {
    return v_c[index].Addr;
}

/* get Name at index in vector v_c */
string CIterator::Name() const {
    return v_c[index].Name;
}


class CTaxRegister {
public:
    bool Birth(const string &name, const string &addr, const string &account);

    bool Death(const string &name, const string &addr);

    bool Income(const string &account, int amount);

    bool Income(const string &name, const string &addr, int amount);

    bool Expense(const string &account, int amount);

    bool Expense(const string &name, const string &addr, int amount);

    bool Audit(const string &name, const string &addr, string &account, int &sumIncome, int &sumExpense) const;

    ~CTaxRegister() {
        v.clear();
    }

    CIterator ListByName(void) const;

private:
    vector<Person> v;
};
/*
 * If there is already such account return false
 * if no such account was found and if at the point of iterator
 * name or addr is different or iterator points to the end of vector then
 * insert the instance to the vector and return true
 * if there is a person who already has such name and addr at the same time then return false
 * */
bool CTaxRegister::Birth(const string &name, const string &addr, const string &account) {
    Person person;
    person.Account = account;
    person.Addr = addr;
    person.Name = name;
    person.income = 0;
    person.outcome = 0;
    for (std::size_t i = 0, max = v.size(); i != max; ++i) {
        if (v[i].Account == account)
            return false;
    }
    auto it = lower_bound(v.begin(), v.end(), person, cmp1);
    if (it == v.end() || ((*it).Name != name || (*it).Addr != addr)) {
        v.insert(it, person);
        return true;
    }
    return false;
}



/*
 * Find a person according to the account if there is no such person return false
 * else return true and add the adequate amount money to his account
*/
bool CTaxRegister::Income(const string &account, int amount) {
    for (std::size_t i = 0, max = v.size(); i != max; ++i) {
        if (v[i].Account == account) {
            v[i].income += amount;
            return true;
        }
    }
    return false;
}


/*
 * Find a person according to the name and addr if there is no such person return false
 * or if addr or name doesn't match return false
 * else return true and add the adequate amount of money to his account of income
*/
bool CTaxRegister::Income(const string &name, const string &addr, int amount) {
    Person person;
    person.Addr = addr;
    person.Name = name;
    auto it = lower_bound(v.begin(), v.end(), person, cmp1);
    if (v.end() == it)
        return false;
    int index = it - v.begin();
    if (v[index].Addr != addr || v[index].Name != name)
        return false;
    v[index].income += amount;
    return true;
}


/*
 * Find a person according to the account if there is no such person return false
 * else return true and add the adequate amount of money to his account of outcome
*/
bool CTaxRegister::Expense(const string &account, int amount) {
    for (std::size_t i = 0, max = v.size(); i != max; ++i) {
        if (v[i].Account == account) {
            v[i].outcome += amount;
            return true;
        }
    }
    return false;
}

/*
 * Find a person according to the name and addr if there is no such person return false
 * or if addr or name doesn't match return false
 * else return true and add the adequate amount of money to his account of outcome
*/
bool CTaxRegister::Expense(const string &name, const string &addr, int amount) {
    Person person;
    person.Addr = addr;
    person.Name = name;
    auto it = lower_bound(v.begin(), v.end(), person, cmp1);
    if (v.end() == it)
        return false;
    int index = it - v.begin();
    if (v[index].Addr != addr || v[index].Name != name)
        return false;
    v[index].outcome += amount;
    return true;
}

/*
 * if such person doesn't exist in the vector return false
 * if it's not the end of the vector but the person isn't the right one return false
 * if the person found and is the right one then erase it and return true
 * */
bool CTaxRegister::Death(const string &name, const string &addr) {
    Person person;
    person.Addr = addr;
    person.Name = name;
    auto it = lower_bound(v.begin(), v.end(), person, cmp1);
    if (v.end() == it)
        return false;
    int index = it - v.begin();
    if (v[index].Addr != addr || v[index].Name != name)
        return false;
    v.erase(v.begin() + index);
    return true;
}


/*
 * if the person doesn't exist return false
 * if addr or name doesn't match return false
 * otherwise set the sumIncome and  sumExpense with the found element and return true
 */
bool CTaxRegister::Audit(const string &name, const string &addr, string &account, int &sumIncome, int &sumExpense) const {
    Person person;
    person.Addr = addr;
    person.Name = name;
    auto it = lower_bound(v.begin(), v.end(), person, cmp1);
    if (v.end() == it)
        return false;
    int index = it - v.begin();
    if (v[index].Addr != addr || v[index].Name != name)
        return false;
    account = v[index].Account;
    sumIncome = v[index].income;
    sumExpense = v[index].outcome;
    return true;
}
// return CIterator by reference that is initialized with vector v
CIterator CTaxRegister::ListByName() const {
    CIterator it_c(v);
    return it_c;
}

#ifndef __PROGTEST__

int main(void) {
    string acct;
    int sumIncome, sumExpense;
    CTaxRegister b1;
    assert (b1.Birth("John Smith", "Oak Road 23", "123/456/789"));
    assert (b1.Birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
    assert (b1.Birth("Peter Hacker", "Main Street 17", "634oddT"));
    assert (b1.Birth("John Smith", "Main Street 17", "Z343rwZ"));
    assert (b1.Income("Xuj5#94", 1000));
    assert (b1.Income("634oddT", 2000));
    assert (b1.Income("123/456/789", 3000));
    assert (b1.Income("634oddT", 4000));
    assert (b1.Income("Peter Hacker", "Main Street 17", 2000));
    assert (b1.Expense("Jane Hacker", "Main Street 17", 2000));
    assert (b1.Expense("John Smith", "Main Street 17", 500));
    assert (b1.Expense("Jane Hacker", "Main Street 17", 1000));
    assert (b1.Expense("Xuj5#94", 1300));
    assert (b1.Audit("John Smith", "Oak Road 23", acct, sumIncome, sumExpense));
    assert (acct == "123/456/789");
    assert (sumIncome == 3000);
    assert (sumExpense == 0);
    assert (b1.Audit("Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense));
    assert (acct == "Xuj5#94");
    assert (sumIncome == 1000);
    assert (sumExpense == 4300);
    assert (b1.Audit("Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense));
    assert (acct == "634oddT");
    assert (sumIncome == 8000);
    assert (sumExpense == 0);
    assert (b1.Audit("John Smith", "Main Street 17", acct, sumIncome, sumExpense));
    assert (acct == "Z343rwZ");
    assert (sumIncome == 0);
    assert (sumExpense == 500);
    CIterator it = b1.ListByName();
    assert (!it.AtEnd()
            && it.Name() == "Jane Hacker"
            && it.Addr() == "Main Street 17"
            && it.Account() == "Xuj5#94");
    it.Next();
    assert (!it.AtEnd()
            && it.Name() == "John Smith"
            && it.Addr() == "Main Street 17"
            && it.Account() == "Z343rwZ");
    it.Next();
    assert (!it.AtEnd()
            && it.Name() == "John Smith"
            && it.Addr() == "Oak Road 23"
            && it.Account() == "123/456/789");
    it.Next();
    assert (!it.AtEnd()
            && it.Name() == "Peter Hacker"
            && it.Addr() == "Main Street 17"
            && it.Account() == "634oddT");
    it.Next();
    assert (it.AtEnd());

    assert (b1.Death("John Smith", "Main Street 17"));

    CTaxRegister b2;
    assert (b2.Birth("John Smith", "Oak Road 23", "123/456/789"));
    assert (b2.Birth("Jane Hacker", "Main Street 17", "Xuj5#94"));
    assert (!b2.Income("634oddT", 4000));
    assert (!b2.Expense("John Smith", "Main Street 18", 500));
    assert (!b2.Audit("John Nowak", "Second Street 23", acct, sumIncome, sumExpense));
    assert (!b2.Death("Peter Nowak", "5-th Avenue"));
    assert (!b2.Birth("Jane Hacker", "Main Street 17", "4et689A"));
    assert (!b2.Birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
    assert (b2.Death("Jane Hacker", "Main Street 17"));
    assert (b2.Birth("Joe Hacker", "Elm Street 23", "Xuj5#94"));
    assert (b2.Audit("Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense));
    assert (acct == "Xuj5#94");
    assert (sumIncome == 0);
    assert (sumExpense == 0);
    assert (!b2.Birth("Joe Hacker", "Elm Street 23", "AAj5#94"));

    return 0;
}

#endif /* __PROGTEST__ */