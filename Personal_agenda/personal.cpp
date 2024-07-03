#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

struct Employee
{
  string m_name;
  string m_surname;
  string m_email;
  unsigned int m_salary;
  // Employee* m_salary_ord;
  // Employee* m_email_ord;
  // Employee* m_fullname_ord;
};

class CPersonalAgenda
{
  public:
                  CPersonalAgenda  ( void );
                  ~CPersonalAgenda ( void );
    bool          add              ( const string    & name,
                                     const string    & surname,
                                     const string    & email,
                                     unsigned int      salary );
    bool          del              ( const string    & name,
                                     const string    & surname );
    bool          del              ( const string    & email );
    bool          changeName       ( const string    & email,
                                     const string    & newName,
                                     const string    & newSurname );
    bool          changeEmail      ( const string    & name,
                                     const string    & surname,
                                     const string    & newEmail );
    bool          setSalary        ( const string    & name,
                                     const string    & surname,
                                     unsigned int      salary );
    bool          setSalary        ( const string    & email,
                                     unsigned int      salary );
    unsigned int  getSalary        ( const string    & name,
                                     const string    & surname ) const;
    unsigned int  getSalary        ( const string    & email ) const;
    bool          getRank          ( const string    & name,
                                     const string    & surname,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          getRank          ( const string    & email,
                                     int             & rankMin,
                                     int             & rankMax ) const;
    bool          getFirst         ( string          & outName,
                                     string          & outSurname ) const;
    bool          getNext          ( const string    & name,
                                     const string    & surname,
                                     string          & outName,
                                     string          & outSurname ) const;
  private:
    // todo
    // vector<Employee> m_employees;
    // vector<Employee*> m_salary_list;
    vector<Employee*> m_email_list;
    vector<Employee*> m_fullname_list;
    // bool sorted;
    bool bin_search_email(const vector<Employee *>& arr, const string& email, size_t& pos)const;
    bool bin_search_fullname(const vector<Employee *>& arr, const string& name, const string& surname, size_t& pos)const;
    // size_t lo_bound(const vector<Employee *>& arr, unsigned int salary) const;
    // size_t up_bound(const vector<Employee *>& arr, unsigned int salary) const;
};

CPersonalAgenda::CPersonalAgenda  ( void ){
  // sorted = true;
  // m_salary_list.clear();
  m_email_list.clear();
  m_fullname_list.clear();
}
                  
                  
CPersonalAgenda::~CPersonalAgenda ( void ){
  for(size_t i=0; i<m_email_list.size(); i++){
    delete m_email_list[i];
  }
}

bool CPersonalAgenda::bin_search_fullname(const std::vector<Employee *>& arr, const string& name, const string& surname, size_t& pos)const{
  long long int l = 0;
  long long int r = (long long int)(arr.size()-1);
  while(l<=r){
    long long int m = (l+r)/2;
    auto res = arr[m]->m_surname.compare(surname);
    if (res == 0){
      res = arr[m]->m_name.compare(name);
    }
    if( res < 0){
      l = m+1;
    }
    else if (res > 0){
      r=m-1;
    }
    else{
      pos=(size_t)m;
      return true;
    }
  }
  pos = SIZE_MAX;
  return false;
}


bool CPersonalAgenda::bin_search_email(const vector<Employee *>& arr, const string& email, size_t& pos)const{
  long long int l = 0;
  long long int r = (long long int)(arr.size()-1);
  while(l<=r){
    long long int m = (l+r)/2;
    auto res = arr[m]->m_email.compare(email);
    if( res < 0){
      l = m+1;
    }
    else if (res > 0){
      r=m-1;
    }
    else{
      pos=(size_t)m;
      return true;
    }
  }
  pos = SIZE_MAX;
  return false;
}
struct Fullname
{
  string surname;
  string name;
};

bool CPersonalAgenda::add ( const string& name, const string& surname, const string& email, unsigned int salary ){
  size_t existing_pos_f;
  bool fullname_exists = bin_search_fullname(m_fullname_list, name, surname, existing_pos_f);
  if (fullname_exists==true){
    return false;
  }
  size_t existing_pos_e;
  bool email_exists = bin_search_email(m_email_list, email, existing_pos_e);
  if(email_exists==true){
    return false;
  }

  Employee* newEmployee = new Employee;
  // m_salary_list.push_back(newEmployee);
  // m_email_list.push_back(newEmployee);
  // m_fullname_list.push_back(newEmployee);
  newEmployee->m_email=email;
  newEmployee->m_name=name;
  newEmployee->m_surname=surname;
  newEmployee->m_salary=salary;
  if (m_email_list.size()==0){
    m_email_list.push_back(newEmployee);
  }
  else{
    auto email_upper = upper_bound(m_email_list.begin(), m_email_list.end(), email, [this](auto a, auto b)
                                    {
                                      return (a.compare((*b).m_email)<0);
                                    });
    size_t pos = distance(m_email_list.begin(), email_upper);
    m_email_list.push_back(m_email_list.back());
    for(size_t i=m_email_list.size()-1; i>pos; i--){
      m_email_list[i]=m_email_list[i-1];
    }
    m_email_list[pos]=newEmployee;
  }
  if(m_fullname_list.size()==0){
    m_fullname_list.push_back(newEmployee);
  }
  else{
    Fullname fn;
    fn.name = name;
    fn.surname = surname;
    auto fullname_upper = upper_bound(m_fullname_list.begin(), m_fullname_list.end(), fn, [this](auto a, auto b)
                                    {
                                      auto res = a.surname.compare((*b).m_surname);
                                      if (res == 0) {res = a.name.compare((*b).m_name);}
                                      return (res < 0);
                                    });
    size_t pos = distance(m_fullname_list.begin(), fullname_upper);
    m_fullname_list.push_back(m_fullname_list.back());
    for(size_t i=m_fullname_list.size()-1; i>pos; i--){
      m_fullname_list[i]=m_fullname_list[i-1];
    }
    m_fullname_list[pos]=newEmployee;
  }

  
  // auto iter = m_email_list.back();


  // sort(m_email_list.begin(), m_email_list.end(), [this](auto a, auto b)
  //                                 {
  //                                     return ((*a).m_email.compare((*b).m_email)<0);
  //                                 });



  // sort(m_fullname_list.begin(), m_fullname_list.end(), [this](auto a, auto b)
  //                                 {
  //                                   auto res = (*a).m_surname.compare((*b).m_surname);
  //                                   if (res == 0) {res = (*a).m_name.compare((*b).m_name);}
  //                                   return (res < 0);
  //                                 });
  // sort(m_salary_list.begin(), m_salary_list.end(), [this](auto a, auto b)
  //                               {
  //                                   return ((*a).m_salary < (*b).m_salary);
  //                               });
  return true; // to test
}

bool CPersonalAgenda::getFirst ( string& outName, string& outSurname ) const{
  if(m_fullname_list.size()==0){
    return false;
  }
  outName = m_fullname_list[0]->m_name;
  outSurname = m_fullname_list[0]->m_surname;
  return true;
}

bool CPersonalAgenda::getNext ( const string& name, const string& surname, string& outName, string& outSurname ) const{
  size_t pos;
  bool found_fullname = bin_search_fullname(m_fullname_list, name, surname, pos);
  if (found_fullname == false || pos == (m_fullname_list.size()-1)){
    return false;
  }
  outName=m_fullname_list[pos+1]->m_name;
  outSurname=m_fullname_list[pos+1]->m_surname;
  return true;
}

bool CPersonalAgenda::setSalary ( const string& email, unsigned int salary ){
  size_t pos;
  bool found_email = bin_search_email(m_email_list, email, pos);
  if (found_email==false){
    return false;
  }
  m_email_list[pos]->m_salary=salary;
  // sort(m_salary_list.begin(), m_salary_list.end(), [this](auto a, auto b)
  //                               {
  //                                   return ((*a).m_salary < (*b).m_salary);
  //                               });
  return true;
}

bool CPersonalAgenda::setSalary ( const string& name, const string& surname, unsigned int salary ){
  size_t pos;
  bool found_fullname = bin_search_fullname(m_fullname_list, name, surname, pos);
  if (found_fullname == false){
    return false;
  }
  m_fullname_list[pos]->m_salary=salary;
  // sort(m_salary_list.begin(), m_salary_list.end(), [this](auto a, auto b)
  //                               {
  //                                   return ((*a).m_salary < (*b).m_salary);
  //                               });
  return true;
}

unsigned int CPersonalAgenda::getSalary ( const string& email ) const{
  size_t pos;
  bool found_email = bin_search_email(m_email_list, email, pos);
  if (found_email==false){
    return 0;
  }
  unsigned int salary =  m_email_list[pos]->m_salary;
  return salary;
}

unsigned int CPersonalAgenda::getSalary ( const string& name, const string& surname ) const{
  size_t pos;
  bool found_fullname = bin_search_fullname(m_fullname_list, name, surname, pos);
  if(found_fullname==false){
    return 0;
  }
  unsigned int salary = m_fullname_list[pos]->m_salary;
  return salary;
}

// size_t CPersonalAgenda::lo_bound(const vector<Employee *>& arr, unsigned int salary) const{
//   auto count = arr.size();
//   size_t first=0;
//   size_t it;
//   size_t step;
//   while(count > 0){
//     it = first;
//     step = count/2;
//     it = it+step;
//     if (arr[it]->m_salary < salary){
//       it++;
//       first = it;
//       count -= step + 1; 
//     }
//     else{
//       count = step;
//     }
//   }
//   return first;
// }

// size_t CPersonalAgenda::up_bound(const vector<Employee *>& arr, unsigned int salary) const{
//   auto count = arr.size();
//   size_t first=0;
//   size_t it;
//   size_t step;
//   while(count>0){
//     it = first;
//     step = count / 2;
//     it = it+step;
//     if (!(salary < arr[it]->m_salary)){
//       it++;
//       first = it;
//       count -= step + 1; 
//     }
//     else{
//       count = step;
//     }
//   }
//   return first;
// }

bool CPersonalAgenda::getRank ( const string& email, int& rankMin, int& rankMax ) const{
  vector<Employee*> m_salary_list = m_email_list;
  sort(m_salary_list.begin(), m_salary_list.end(), [this](auto a, auto b)
                                {
                                    return ((*a).m_salary < (*b).m_salary);
                                });
  unsigned int salary = getSalary(email);
  if (salary==0){
    return false;
  }
  auto lower =  lower_bound(m_salary_list.begin(), m_salary_list.end(), salary, [this](auto a, auto b)
                                {
                                    return ((*a).m_salary < b);
                                });//lo_bound(m_salary_list, salary);
  auto higher = upper_bound(m_salary_list.begin(), m_salary_list.end(), salary, [this](auto a, auto b)
                                {
                                    return (a < (*b).m_salary);
                                });//up_bound(m_salary_list, salary);
  // if(higher>0){
  //   higher--;
  // }
  rankMin = distance(m_salary_list.begin(), lower);
  rankMax = distance(m_salary_list.begin(), higher);//higher;
  if (rankMax>0){
    rankMax--;
  }
  return true;
}

bool CPersonalAgenda::getRank ( const string& name, const string& surname, int& rankMin, int& rankMax ) const{
  vector<Employee*> m_salary_list = m_email_list;
  sort(m_salary_list.begin(), m_salary_list.end(), [this](auto a, auto b)
                                {
                                    return ((*a).m_salary < (*b).m_salary);
                                });

  unsigned int salary = getSalary(name, surname);
  if (salary==0){
    return false;
  }

  auto lower =  lower_bound(m_salary_list.begin(), m_salary_list.end(), salary, [this](auto a, auto b)
                                {
                                    return ((*a).m_salary < b);
                                });
  auto higher = upper_bound(m_salary_list.begin(), m_salary_list.end(), salary, [this](auto a, auto b)
                                {
                                    return (a < (*b).m_salary);
                                });
  // auto lower = lo_bound(m_salary_list, salary);
  // auto higher = up_bound(m_salary_list, salary);
  // if(higher>0){
  //   higher--;
  // }
  // rankMin = lower;
  // rankMax = higher;
  rankMin = distance(m_salary_list.begin(), lower);
  rankMax = distance(m_salary_list.begin(), higher);//higher;
  if (rankMax>0){
    rankMax--;
  }
  return true;

}

bool CPersonalAgenda::changeName ( const string& email, const string& newName, const string& newSurname ){
  size_t pos;
  bool found_fullname = bin_search_fullname(m_fullname_list, newName, newSurname, pos );
  if (found_fullname==true){
    return false;
  }
  pos=SIZE_MAX;
  bool found_email = bin_search_email(m_email_list, email, pos);
  if (found_email==false){
    return false;
  }

  // Fullname fn;
  // fn.name = newName;
  // fn.surname = newSurname;
  // auto res = fn.surname.compare(m_email_list[pos]->m_surname);
  // if (res == 0) {res = fn.name.compare(m_email_list[pos]->m_name);}
  // if (res < 0){
  //   auto iter = upper_bound(m_fullname_list.begin(), m_fullname_list.end(), fn, [this](auto a, auto b)
  //                                 {
  //                                   auto res = a.surname.compare((*b).m_surname);
  //                                   if (res == 0) {res = a.name.compare((*b).m_name);}
  //                                   return (res < 0);
  //                                 });
  //   auto new_pos = distance(m_fullname_list.begin(), iter);
  //   auto tmp = m_email_list[pos];
  //   bool newIsSmaller = true;
  //   for(size_t i=pos; i>new_pos; i--){
  //     m_fullname_list[i]=m_fullname_list[i-1];
  //   }
  //   m_fullname_list[new_pos]=tmp;
  //   m_fullname_list[new_pos]->m_name=newName;
  //   m_fullname_list[new_pos]->m_surname=newSurname;
  // } else {
  //   auto iter = lower_bound (m_fullname_list.begin(), m_fullname_list.end(), fn, [this](auto a, auto b)
  //                                 {
  //                                   auto res = (*a).m_surname.compare(b.surname);
  //                                   if (res == 0) {res = (*a).m_name.compare(b.name);}
  //                                   return (res < 0);
  //                                 });
  //   auto new_pos = distance(m_fullname_list.begin(), iter);
  //   bool newIsGreater =true;
  //   auto tmp = m_fullname_list[pos];
  //   for(size_t i=pos; i<new_pos-1; i++){
  //     m_fullname_list[i]=m_fullname_list[i+1];
  //   }
  //   m_fullname_list[new_pos-1]=tmp;
  //   m_fullname_list[new_pos-1]->m_name=newName;
  //   m_fullname_list[new_pos-1]->m_surname=newSurname;
  // }


  m_email_list[pos]->m_name = newName;
  m_email_list[pos]->m_surname = newSurname; 
  sort(m_fullname_list.begin(), m_fullname_list.end(), [this](auto a, auto b)
                                  {
                                    auto res = (*a).m_surname.compare((*b).m_surname);
                                    if (res == 0) {res = (*a).m_name.compare((*b).m_name);}
                                    return (res < 0);
                                  });

  return true;
}

bool CPersonalAgenda::changeEmail ( const string& name, const string& surname, const string& newEmail ){
  size_t pos;
  bool found_email = bin_search_email(m_email_list, newEmail, pos);
  if (found_email==true){
    return false;
  }
  pos=SIZE_MAX;
  bool found_fullname = bin_search_fullname(m_fullname_list, name, surname, pos );
  if (found_fullname==false){
    return false;
  }


  // auto res = newEmail.compare(m_fullname_list[pos]->m_email);
  // if (res < 0){
  //   auto iter = upper_bound(m_email_list.begin(), m_email_list.end(), newEmail, [this](auto a, auto b)
  //                                 {
  //                                   auto res = a.compare((*b).m_email);
  //                                   return (res < 0);
  //                                 });
  //   auto new_pos = distance(m_email_list.begin(), iter);
  //   auto tmp = m_fullname_list[pos];
  //   bool newIsSmaller = true;
  //   for(size_t i=pos; i>new_pos; i--){
  //     m_email_list[i]=m_email_list[i-1];
  //   }
  //   m_email_list[new_pos]=tmp;
  //   m_email_list[new_pos]->m_email=newEmail;
  // } else {
  //   auto iter = lower_bound (m_email_list.begin(), m_email_list.end(), newEmail, [this](auto a, auto b)
  //                                 {
  //                                   auto res = (*a).m_email.compare(b);
  //                                   return (res < 0);
  //                                 });
  //   auto new_pos = distance(m_email_list.begin(), iter);
  //   bool newIsGreater =true;
  //   auto tmp = m_email_list[pos];
  //   for(size_t i=pos; i<new_pos-1; i++){
  //     m_email_list[i]=m_email_list[i+1];
  //   }
  //   m_email_list[new_pos-1]=tmp;
  //   m_email_list[new_pos-1]->m_email=newEmail;
  // }


  m_fullname_list[pos]->m_email=newEmail;
  sort(m_email_list.begin(), m_email_list.end(), [this](auto a, auto b)
                                  {
                                      return ((*a).m_email.compare((*b).m_email)<0);
                                  });

  return true;
}

bool CPersonalAgenda::del ( const string& email ){
  size_t pos_email;
  bool found_email = bin_search_email(m_email_list, email, pos_email);
  if (found_email==false){
    return false;
  }
  string del_name = m_email_list[pos_email]->m_name;
  string del_surname = m_email_list[pos_email]->m_surname;
  size_t pos_fullname;
  bin_search_fullname(m_fullname_list, del_name, del_surname, pos_fullname );
  // size_t pos_salary;
  // for(size_t i=0; i<m_salary_list.size(); i++){
  //   if (m_salary_list[i]==m_fullname_list[pos_fullname]){
  //     pos_salary=i;
  //     break;
  //   }
  // }
  delete m_email_list[pos_email];
  m_email_list[pos_email]=nullptr;

  for(auto i=pos_email; i<m_email_list.size()-1; i++){
    m_email_list[i]=m_email_list[i+1];
  }
  m_email_list.pop_back();

  for(auto i=pos_fullname; i<m_fullname_list.size()-1; i++){
    m_fullname_list[i]=m_fullname_list[i+1];
  }
  m_fullname_list.pop_back();

  // delete tmp;
  // tmp=nullptr;
  // m_email_list[pos_email]=m_email_list.back();
  // m_fullname_list[pos_fullname]=m_fullname_list.back();
  // m_salary_list[pos_salary]=m_salary_list.back();
  // m_email_list.pop_back();
  // m_fullname_list.pop_back();
  // m_salary_list.pop_back();
  // sort(m_email_list.begin(), m_email_list.end(), [this](auto a, auto b)
  //                                 {
  //                                     return ((*a).m_email.compare((*b).m_email)<0);
  //                                 });

  // sort(m_fullname_list.begin(), m_fullname_list.end(), [this](auto a, auto b)
  //                                 {
  //                                   auto res = (*a).m_surname.compare((*b).m_surname);
  //                                   if (res == 0) {res = (*a).m_name.compare((*b).m_name);}
  //                                   return (res < 0);
  //                                 });

  // sort(m_salary_list.begin(), m_salary_list.end(), [this](auto a, auto b)
  //                               {
  //                                   return ((*a).m_salary < (*b).m_salary);
  //                               });
  return true;
}

bool CPersonalAgenda::del ( const string& name, const string& surname ){
  size_t pos_fullname;
  bool found_fullname = bin_search_fullname(m_fullname_list, name, surname, pos_fullname);
  if (found_fullname==false){
    return false;
  }
  string del_email=m_fullname_list[pos_fullname]->m_email;
  size_t pos_email;
  bin_search_email(m_email_list, del_email, pos_email);
  // size_t pos_salary;
  // for(size_t i=0; i<m_salary_list.size(); i++){
  //   if (m_salary_list[i]==m_fullname_list[pos_fullname]){
  //     pos_salary=i;
  //     break;
  //   }
  // }
  auto tmp = m_fullname_list[pos_fullname];
  // m_fullname_list[pos_fullname]=m_fullname_list.back();
  // m_email_list[pos_email]=m_email_list.back();
  // m_salary_list[pos_salary]=m_salary_list.back();
  // m_fullname_list.pop_back();
  // m_email_list.pop_back();
  // m_salary_list.pop_back();

  for(auto i=pos_email; i<m_email_list.size()-1; i++){
    m_email_list[i]=m_email_list[i+1];
  }
  m_email_list.pop_back();

  for(auto i=pos_fullname; i<m_fullname_list.size()-1; i++){
    m_fullname_list[i]=m_fullname_list[i+1];
  }
  m_fullname_list.pop_back();

  delete tmp;
  tmp=nullptr;

  // sort(m_email_list.begin(), m_email_list.end(), [this](auto a, auto b)
  //                                 {
  //                                     return ((*a).m_email.compare((*b).m_email)<0);
  //                                 });

  // sort(m_fullname_list.begin(), m_fullname_list.end(), [this](auto a, auto b)
  //                                 {
  //                                   auto res = (*a).m_surname.compare((*b).m_surname);
  //                                   if (res == 0) {res = (*a).m_name.compare((*b).m_name);}
  //                                   return (res < 0);
  //                                 });
  // sort(m_salary_list.begin(), m_salary_list.end(), [this](auto a, auto b)
  //                               {
  //                                   return ((*a).m_salary < (*b).m_salary);
  //                               });
  return true;
}

#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda a1;
  assert ( a1 . add ( "2", "2", "2", 2 ) );
  assert ( a1 . add ( "4", "4", "4", 4 ) );
  assert ( a1 . add ( "6", "6", "6", 6 ) );
  assert ( a1 . add ( "8", "8", "8", 8 ) );
  assert ( a1 . changeName ( "4", "7", "7" ) );

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . setSalary ( "john", 32000 ) );
  assert ( b1 . getSalary ( "john" ) ==  32000 );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . getSalary ( "john" ) ==  35000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  assert ( b1 . getSalary ( "peter" ) ==  23000 );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "james" ) ==  23000 );
  assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . getFirst ( outName, outSurname ) );
  assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . setSalary ( "joe", 90000 ) );
  assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . getSalary ( "joe" ) ==  0 );
  assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . getRank ( "joe", lo, hi ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . del ( "Joe", "Black" ) );
  assert ( ! b2 . del ( "joe" ) );
  assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . setSalary ( "peter", 37000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  0 );
  assert ( ! b2 . getRank ( "peter", lo, hi ) );
  assert ( ! b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . del ( "peter" ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */


