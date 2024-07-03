#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
  public:
    //---------------------------------------------------------------------------------------------
                             CDate                         ( int               y,
                                                             int               m,
                                                             int               d )
      : m_Year ( y ),
        m_Month ( m ),
        m_Day ( d )
    {
    }
    //---------------------------------------------------------------------------------------------
    int                      compare                       ( const CDate     & x ) const
    {
      if ( m_Year != x . m_Year )
        return m_Year - x . m_Year;
      if ( m_Month != x . m_Month )
        return m_Month - x . m_Month;
      return m_Day - x . m_Day;
    }
    //---------------------------------------------------------------------------------------------
    int                      year                          ( void ) const 
    {
      return m_Year;
    }
    //---------------------------------------------------------------------------------------------
    int                      month                         ( void ) const 
    {
      return m_Month;
    }
    //---------------------------------------------------------------------------------------------
    int                      day                           ( void ) const 
    {
      return m_Day;
    }
    //---------------------------------------------------------------------------------------------
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CDate     & x )
    {
      char oldFill = os . fill ();
      return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Day ) 
                << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
  private:
    int16_t                  m_Year;
    int8_t                   m_Month;
    int8_t                   m_Day;
};
#endif /* __PROGTEST__ */

void remove_middle_space(const string& name, string& tmp_name, size_t& i){
  while(i<name.size()){
    if (isspace(name.at(i))){
      tmp_name.push_back(' ');
      i++;
      while(i <name.size() && isspace(name.at(i))){
        i++;
      }
    }
    if(i<name.size()){
      tmp_name.push_back(std::tolower(name.at(i)));
      i++;
    }
  }
  if(isspace(tmp_name.back())){
    tmp_name.pop_back();
  }
}

void remove_extra_space(const string& name, string& tmp_name){
  size_t i=0;
  while(i<name.size() && isspace(name.at(i))){
    i++;
  }
  remove_middle_space(name, tmp_name, i);
}

class CInvoice
{
  public:
                             CInvoice                      ( const CDate     & date,
                                                             const string    & seller,
                                                             const string    & buyer,
                                                             unsigned int      amount,
                                                             double            vat );
    CDate                    date                          ( void ) const;
    string                   buyer                         ( void ) const;
    string                   seller                        ( void ) const;
    unsigned int             amount                        ( void ) const;
    double                   vat                           ( void ) const;
  private:
    // todo
    CDate m_date;
    size_t m_hsh_seller;
    string m_seller;
    size_t m_hsh_buyer;
    string m_buyer;
    unsigned int m_amount;
    double m_vat;

    size_t m_order;
    static inline size_t m_InsertCnt = 0;
    friend class CVATRegister;
    friend struct CMinInvoice;
};

CInvoice::CInvoice                                          ( const CDate     & date,
                                                             const string    & seller,
                                                             const string    & buyer,
                                                             unsigned int      amount,
                                                             double            vat ):m_date(date)
{
  string tmps;
  string tmpb;
  remove_extra_space(seller, tmps);
  m_hsh_seller = std::hash<string>{}(tmps);
  remove_extra_space(buyer, tmpb);
  m_hsh_buyer = std::hash<string>{}(tmpb);
  m_seller = seller;
  m_buyer = buyer;
  m_amount = amount;
  m_vat = vat;
  m_order = m_InsertCnt++;
}

CDate                    CInvoice::date                          ( void ) const{
  return m_date;
}
string                   CInvoice::buyer                         ( void ) const{
  return m_buyer;
}
string                   CInvoice::seller                        ( void ) const{
  return m_seller;
}
unsigned int             CInvoice::amount                        ( void ) const{
  return m_amount;
}
double                   CInvoice::vat                           ( void ) const{
  return m_vat;
}

class CSortOpt
{
  public:
    static const int         BY_DATE                       = 0;
    static const int         BY_BUYER                      = 1;
    static const int         BY_SELLER                     = 2;
    static const int         BY_AMOUNT                     = 3;
    static const int         BY_VAT                        = 4;
                             CSortOpt                      ( void );
    CSortOpt               & addKey                        ( int               key,
                                                             bool              ascending = true );
  private:
    // todo
    size_t m_priority;
    std::map<size_t, pair<int, bool>> priority_queue;
    friend struct cmpTwoCMinInvoice;
};

CSortOpt::CSortOpt(void){
  m_priority=0;
  priority_queue.clear();
}

CSortOpt& CSortOpt::addKey( int               key,
                            bool              ascending )
{
  priority_queue.insert({m_priority, make_pair(key, ascending)});
  m_priority++;
  return *this;
}

struct CMinInvoice{
  size_t m_hsh_buyer;
  size_t m_hsh_seller;
  CDate m_date;
  // int m_year;
  // int m_month;
  // int m_day;
  unsigned int m_amount;
  double m_vat;
  bool m_issued;
  bool m_accepted;
  size_t m_regOrder;
  CMinInvoice(const CInvoice& x):m_date(x.date()){
    m_hsh_buyer = x.m_hsh_buyer;
    m_hsh_seller = x.m_hsh_seller;
    // m_year = x.m_date.year();
    // m_month = x.m_date.month();
    // m_day = x.m_date.day();
    m_amount = x.amount();
    m_vat = x.vat();
    m_regOrder = x.m_order;
    m_issued=false;
    m_accepted=false;
  }
};

struct tless
{
bool operator () ( const CMinInvoice & a, const CMinInvoice & b ) const
  {

    int res = (a.m_amount == b.m_amount) ? 0 : ((a.m_amount<b.m_amount) ? -1 : 1);
    if (res != 0){
      return res < 0;
    }
    
    res = (std::abs(a.m_vat-b.m_vat)<std::abs(std::min(a.m_vat,b.m_vat))*std::numeric_limits<double>::epsilon()) ? 0 : ((a.m_vat < b.m_vat) ? -1 : 1);
    if (res!=0){
      return res < 0;
    }

    // res = (a.m_year==b.m_year) ? 0 : ((a.m_year<b.m_year) ? -1 : 1);
    // if (res !=0){
    //   return res < 0;
    // }

    // res = (a.m_month==b.m_month) ? 0 : ((a.m_month<b.m_month) ? -1 : 1);
    // if (res !=0){
    //   return res < 0;
    // }

    // res = (a.m_day==b.m_day) ? 0 : ((a.m_day<b.m_day) ? -1 : 1);
    res = a.m_date.compare(b.m_date);
    if (res !=0){
      return res < 0;
    }

    res = (a.m_hsh_buyer==b.m_hsh_buyer) ? 0 : ((a.m_hsh_buyer<b.m_hsh_buyer) ? -1 : 1);
    if (res !=0){
      return res < 0;
    }

    res = (a.m_hsh_seller==b.m_hsh_seller) ? 0 : ((a.m_hsh_seller<b.m_hsh_seller) ? -1 : 1);
    if (res !=0){
      return res < 0;
    }
    return false;
  }
};
class CVATRegister
{
  public:
                             CVATRegister                  ( void );
    bool                     registerCompany               ( const string    & name );
    bool                     addIssued                     ( const CInvoice  & x );
    bool                     addAccepted                   ( const CInvoice  & x );
    bool                     delIssued                     ( const CInvoice  & x );
    bool                     delAccepted                   ( const CInvoice  & x );
    list<CInvoice>           unmatched                     ( const string    & company,
                                                             const CSortOpt  & sortBy ) const;
  private:
    // todo
    std::map <size_t, string> m_officialCompanyName;
    std::set <CMinInvoice, tless> m_registered;
    std::map <size_t, std::set <CMinInvoice, tless>> unmatchedInCompany;
};

CVATRegister::CVATRegister                  ( void ){
  m_officialCompanyName.clear();
  m_registered.clear();
  unmatchedInCompany.clear();
}

bool CVATRegister::registerCompany(const string& name){
  string tmp_name;
  remove_extra_space(name, tmp_name);
  size_t hsh = std::hash<string>{}(tmp_name);
  if (m_officialCompanyName.insert({hsh, name}).second){
    unmatchedInCompany.insert({hsh, {}});
    return true;
  }
  return false;
}

bool                     CVATRegister::addIssued                     ( const CInvoice  & x ){
  if (x.m_hsh_buyer == x.m_hsh_seller){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_buyer)==m_officialCompanyName.end()){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_seller)==m_officialCompanyName.end()){
    return false;
  }
  CMinInvoice tmp (x);
  auto iter_pos = m_registered.find(tmp);
  if (iter_pos==m_registered.end()){
    tmp.m_issued=true;
    tmp.m_accepted=false;
    m_registered.insert(tmp);
    // add to each buyer and seller unmatched;
    unmatchedInCompany.at(x.m_hsh_buyer).insert(tmp);
    unmatchedInCompany.at(x.m_hsh_seller).insert(tmp);
  }
  else{
    if((*iter_pos).m_issued==true){
      return false;
    }
    else{
      tmp.m_issued=true;
      tmp.m_accepted = (*iter_pos).m_accepted;
      tmp.m_regOrder = (*iter_pos).m_regOrder;
      auto ex = m_registered.extract(iter_pos);
      m_registered.insert(tmp);
      // remove from buyer and seller unmatched
      unmatchedInCompany.at(tmp.m_hsh_buyer).erase(tmp);
      unmatchedInCompany.at(tmp.m_hsh_seller).erase(tmp);
    }
  }
  return true;
}

bool                     CVATRegister::addAccepted                   ( const CInvoice  & x )
{
  if (x.m_hsh_buyer == x.m_hsh_seller){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_buyer)==m_officialCompanyName.end()){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_seller)==m_officialCompanyName.end()){
    return false;
  }
  CMinInvoice tmp (x);
  auto iter_pos = m_registered.find(tmp);
  if (iter_pos==m_registered.end()){
    tmp.m_issued=false;
    tmp.m_accepted=true;
    m_registered.insert(tmp);
    // add to each buyer and seller unmatched;
    unmatchedInCompany.at(x.m_hsh_buyer).insert(tmp);
    unmatchedInCompany.at(x.m_hsh_seller).insert(tmp);
  }
  else{
    if((*iter_pos).m_accepted==true){
      return false;
    }
    else{
      tmp.m_accepted=true;
      tmp.m_issued = (*iter_pos).m_issued;
      tmp.m_regOrder = (*iter_pos).m_regOrder;
      auto ex = m_registered.extract(iter_pos);
      m_registered.insert(tmp);
      // remove from buyer and seller unmatched
      unmatchedInCompany.at(tmp.m_hsh_buyer).erase(tmp);
      unmatchedInCompany.at(tmp.m_hsh_seller).erase(tmp);
    }
  }
  return true;
}

bool                     CVATRegister::delIssued                     ( const CInvoice  & x )
{
  if (x.m_hsh_buyer == x.m_hsh_seller){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_buyer)==m_officialCompanyName.end()){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_seller)==m_officialCompanyName.end()){
    return false;
  }
  CMinInvoice tmp (x);
  auto iter_pos = m_registered.find(tmp);
  if (iter_pos==m_registered.end()){
    return false;
  }
  else{
    if((*iter_pos).m_issued==false){
      return false;
    }
    else{
      if ((*iter_pos).m_accepted==true){
        tmp.m_accepted=(*iter_pos).m_accepted;
        tmp.m_issued =false;
        tmp.m_regOrder =(*iter_pos).m_regOrder;
        auto ex = m_registered.extract(iter_pos);
        m_registered.insert(tmp);
        unmatchedInCompany.at(tmp.m_hsh_buyer).insert(tmp);
        unmatchedInCompany.at(tmp.m_hsh_seller).insert(tmp);
      }
      else{
        auto ex = m_registered.extract(iter_pos);
        unmatchedInCompany.at(tmp.m_hsh_buyer).erase(tmp);
        unmatchedInCompany.at(tmp.m_hsh_seller).erase(tmp);
      }
    }
  }
  return true;
}

bool                     CVATRegister::delAccepted                   ( const CInvoice  & x )
{
  if (x.m_hsh_buyer == x.m_hsh_seller){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_buyer)==m_officialCompanyName.end()){
    return false;
  }
  if (m_officialCompanyName.find(x.m_hsh_seller)==m_officialCompanyName.end()){
    return false;
  }
  CMinInvoice tmp (x);
  auto iter_pos = m_registered.find(tmp);
  if (iter_pos==m_registered.end()){
    return false;
  }
  else{
    if((*iter_pos).m_accepted==false){
      return false;
    }
    else{
      if ((*iter_pos).m_issued==true){
        tmp.m_issued=(*iter_pos).m_issued;
        tmp.m_accepted =false;
        tmp.m_regOrder =(*iter_pos).m_regOrder;
        auto ex = m_registered.extract(iter_pos);
        m_registered.insert(tmp);
        unmatchedInCompany.at(tmp.m_hsh_buyer).insert(tmp);
        unmatchedInCompany.at(tmp.m_hsh_seller).insert(tmp);
      }
      else{
        auto ex = m_registered.extract(iter_pos);
        unmatchedInCompany.at(tmp.m_hsh_buyer).erase(tmp);
        unmatchedInCompany.at(tmp.m_hsh_seller).erase(tmp);
      }
    }
  }
  return true;
}

struct cmpTwoCMinInvoice
{
  const std::map<size_t, std::string>& m_cmpName;
  const CSortOpt& m_opts;
  cmpTwoCMinInvoice(const std::map<size_t, std::string>& cmpName, const CSortOpt& sortOpts) : m_cmpName(cmpName), m_opts(sortOpts) {}
  bool operator()(const CMinInvoice& lhs, const CMinInvoice& rhs)const{
    if (m_opts.priority_queue.empty()){
      return lhs.m_regOrder < rhs.m_regOrder;
    }
    else {
      for(auto x : m_opts.priority_queue){
        int res;
        switch(x.second.first) {
          case 0:
            if (x.second.second){
              const auto& a=lhs;
              const auto& b=rhs;
              // res = (a.m_year==b.m_year) ? 0 : ((a.m_year<b.m_year) ? -1 : 1);
              // if (res !=0){
              //   return res < 0;
              // }
              
              // res = (a.m_month==b.m_month) ? 0 : ((a.m_month<b.m_month) ? -1 : 1);
              // if (res !=0){
              //   return res < 0;
              // }
              // res = (a.m_day==b.m_day) ? 0 : ((a.m_day<b.m_day) ? -1 : 1);
              res = a.m_date.compare(b.m_date);
              if (res !=0){
                return res < 0;
              }
            }
            else{
              const auto& a=rhs;
              const auto& b=lhs;
              // res = (a.m_year==b.m_year) ? 0 : ((a.m_year<b.m_year) ? -1 : 1);
              // if (res !=0){
              //   return res < 0;
              // }
              
              // res = (a.m_month==b.m_month) ? 0 : ((a.m_month<b.m_month) ? -1 : 1);
              // if (res !=0){
              //   return res < 0;
              // }
              // res = (a.m_day==b.m_day) ? 0 : ((a.m_day<b.m_day) ? -1 : 1);
              res = a.m_date.compare(b.m_date);
              if (res !=0){
                return res < 0;
              }
            }
            break;
          case 1:
            
            if (x.second.second){
              const auto& a=lhs;
              const auto& b=rhs;
              const string& a_str = m_cmpName.at(a.m_hsh_buyer);
              const string& b_str = m_cmpName.at(b.m_hsh_buyer);
              size_t smaller_str_size = min(a_str.size(), b_str.size());
              for(size_t i=0; i< smaller_str_size; i++){
                if (tolower(a_str.at(i)) == tolower(b_str.at(i))){
                  res =0;
                  continue;
                }
                else{
                  res = tolower(a_str.at(i)) < tolower(b_str.at(i))?-1:1;
                  break;
                }
              }
              if(res==0 && a_str.size() != b_str.size()){
                res = a_str.size() < b_str.size() ? -1 : 1;
              }
              if (res != 0){
                return res < 0;
              }
            }
            else{
              const auto& a=rhs;
              const auto& b=lhs;
              const string& a_str = m_cmpName.at(a.m_hsh_buyer);
              const string& b_str = m_cmpName.at(b.m_hsh_buyer);
              size_t smaller_str_size = min(a_str.size(), b_str.size());
              for(size_t i=0; i< smaller_str_size; i++){
                if (tolower(a_str.at(i)) == tolower(b_str.at(i))){
                  res =0;
                  continue;
                }
                else{
                  res = tolower(a_str.at(i)) < tolower(b_str.at(i))?-1:1;
                  break;
                }
              }
              if(res==0 && a_str.size() != b_str.size()){
                res = a_str.size() < b_str.size() ? -1 : 1;
              }
              
              if (res != 0){
                return res < 0;
              }
            }
            break;
          case 2:
            
            if (x.second.second){
              const auto& a=lhs;
              const auto& b=rhs;
              const string& a_str = m_cmpName.at(a.m_hsh_seller);
              const string& b_str = m_cmpName.at(b.m_hsh_seller);
              size_t smaller_str_size = min(a_str.size(), b_str.size());
              for(size_t i=0; i< smaller_str_size; i++){
                if (tolower(a_str.at(i)) == tolower(b_str.at(i))){
                  res =0;
                  continue;
                }
                else{
                  res = tolower(a_str.at(i)) < tolower(b_str.at(i))?-1:1;
                  break;
                }
              }
              if(res==0 && a_str.size() != b_str.size()){
                res = a_str.size() < b_str.size() ? -1 : 1;
              }
              
              if (res != 0){
                return res < 0;
              }
            }
            else{
              const auto& a=rhs;
              const auto& b=lhs;
              const string& a_str = m_cmpName.at(a.m_hsh_seller);
              const string& b_str = m_cmpName.at(b.m_hsh_seller);
              size_t smaller_str_size = min(a_str.size(), b_str.size());
              for(size_t i=0; i< smaller_str_size; i++){
                if (tolower(a_str.at(i)) == tolower(b_str.at(i))){
                  res =0;
                  continue;
                }
                else{
                  res = tolower(a_str.at(i)) < tolower(b_str.at(i))?-1:1;
                  break;
                }
              }
              if(res==0 && a_str.size() != b_str.size()){
                res = a_str.size() < b_str.size() ? -1 : 1;
              }
              if (res != 0){
                return res < 0;
              }
            }
            break;
          case 3:
            
            if (x.second.second){
              const auto& a=lhs;
              const auto& b=rhs;
              res = (a.m_amount==b.m_amount) ? 0 : ((a.m_amount<b.m_amount) ? -1 : 1);
              if (res != 0){
                return res < 0;
              }
            }
            else{
              const auto& a=rhs;
              const auto& b=lhs;
              res = (a.m_amount==b.m_amount) ? 0 : ((a.m_amount<b.m_amount) ? -1 : 1);
              if (res != 0){
                return res < 0;
              }
            }
            break;
          case 4:
            
            if (x.second.second){
              const auto& a=lhs;
              const auto& b=rhs;
              res = (std::abs(a.m_vat-b.m_vat)<std::abs(std::min(a.m_vat,b.m_vat))*std::numeric_limits<double>::epsilon()) ? 0 : ((a.m_vat < b.m_vat) ? -1 : 1);
              if (res != 0){
                return res < 0;
              }
            }
            else{
              const auto& a=rhs;
              const auto& b=lhs;
              res = (std::abs(a.m_vat-b.m_vat)<std::abs(std::min(a.m_vat,b.m_vat))*std::numeric_limits<double>::epsilon()) ? 0 : ((a.m_vat < b.m_vat) ? -1 : 1);
              if (res != 0){
                return res < 0;
              }
            }
            break;
          default:
            {}
        }
      }
      return lhs.m_regOrder < rhs.m_regOrder;
    }  // return m_cmpName.at(lhs.m_hsh_buyer) < m_cmpName.at(rhs.m_hsh_buyer);
  }
};


list<CInvoice>           CVATRegister::unmatched                     ( const string    & company,
                                                             const CSortOpt  & sortBy ) const
{
  string tmp_comp;
  remove_extra_space(company, tmp_comp);
  size_t hsh = std::hash<string>{}(tmp_comp);
  std::vector<CMinInvoice> tmp_list;
  list<CInvoice> res;
  if (m_officialCompanyName.find(hsh) == m_officialCompanyName.end()){
    return res;
  }
  for(auto x:unmatchedInCompany.at(hsh)){
    tmp_list.push_back(x);
  }
  sort(tmp_list.begin(), tmp_list.end(), cmpTwoCMinInvoice(this->m_officialCompanyName, sortBy));
  for(auto x : tmp_list){
    res.push_back(CInvoice(x.m_date, m_officialCompanyName.at(x.m_hsh_seller), m_officialCompanyName.at(x.m_hsh_buyer), x.m_amount, x.m_vat));
  }
  return res;
}
#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
  // todo
  if (a.size()!=b.size()){
    return false;
  }
  auto iter_a = a.begin();
  auto iter_b = b.begin();
  while(iter_a!=a.end() && iter_b!=b.end()){
    if(iter_a->date().compare(iter_b->date())!=0){
      return false;
    }
    if(iter_a->amount()!=iter_b->amount()){
      return false;
    }
    if((std::abs(iter_a->vat()-iter_b->vat())<std::abs(std::min(iter_a->vat(),iter_b->vat()))*std::numeric_limits<double>::epsilon()) ? 0 : ((iter_a->vat() < iter_b->vat()) ? -1 : 1)!=0){
      return false;
    }
    if (iter_a->buyer().compare(iter_b->buyer())!=0){
      return false;
    }
    if (iter_a->seller().compare(iter_b->seller())!=0){
      return false;
    }
    iter_a++;
    iter_b++;
  }
  return true;
}

int main ( void )
{
  CVATRegister r;
  assert ( r . registerCompany ( "first Company" ) );
  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
