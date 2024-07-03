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
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

struct Stax;
struct Sname_addr;
struct SCompany;

struct Stax
{
  string a_taxID;
  SCompany* t_my_rev;
};

struct Sname_addr
{
  string a_name;
  string a_addr;
  SCompany* na_my_rev;
};

struct SCompany
{
  Stax* my_tax_id;
  unsigned int my_revenue;
  Sname_addr* my_name_addr;
};

class CVATRegister
{
  public:
                  CVATRegister   ( void );
                  ~CVATRegister  ( void );
    bool          newCompany     ( const string    & name,
                                   const string    & addr,
                                   const string    & taxID );
    bool          cancelCompany  ( const string    & name,
                                   const string    & addr );
    bool          cancelCompany  ( const string    & taxID );
    bool          invoice        ( const string    & taxID,
                                   unsigned int      amount );
    bool          invoice        ( const string    & name,
                                   const string    & addr,
                                   unsigned int      amount );
    bool          audit          ( const string    & name,
                                   const string    & addr,
                                   unsigned int    & sumIncome ) const;
    bool          audit          ( const string    & taxID,
                                   unsigned int    & sumIncome ) const;
    bool          firstCompany   ( string          & name,
                                   string          & addr ) const;
    bool          nextCompany    ( string          & name,
                                   string          & addr ) const;
    unsigned int  medianInvoice  ( void ) const;
  private:
  vector <SCompany*> m_revs;
  vector <Stax*> m_taxIDs;
  vector <Sname_addr*> m_name_addr;
  long long int b_search(const string& taxID) const;
  long long int b_search(const string& name, const string& addr) const;
  vector<unsigned int>m_inv_amounts;
    // todo
};

CVATRegister::CVATRegister(void)
{
  m_revs.clear();
  m_inv_amounts.clear();
  m_taxIDs.clear();
  m_name_addr.clear();
}

CVATRegister::~CVATRegister  ( void )
{
  for (size_t i=0; i<m_taxIDs.size(); i++)
  {
    delete m_taxIDs.at(i)->t_my_rev->my_name_addr;
    delete m_taxIDs.at(i)->t_my_rev;
    delete m_taxIDs.at(i);
  }
  m_name_addr.clear();
  m_taxIDs.clear();
  m_revs.clear();
  m_inv_amounts.clear();
}

long long int CVATRegister::b_search(const string& taxID) const
{
  long long int l=0;
  long long int r=m_taxIDs.size()-1;
  long long int m;
  while (l<=r)
  {
    m=(l+r)/2;
    int res = (m_taxIDs.at(m)->a_taxID).compare(taxID);
    if ( res<0)
    {
      l=m+1;
    }
    else if ( res >0)
    {
      r=m-1;
    }
    else return m;
  }
  return -1;
}

long long int  CVATRegister::b_search(const string& name, const string& addr) const
{
  long long int l=0;
  long long int r=m_name_addr.size()-1;
  long long int  m;
  while (l<=r)
  {
    m=(l+r)/2;
    int res = strcasecmp(m_name_addr.at(m)->a_name.c_str(), name.c_str());
    if ( res<0)
    {
      l=m+1;
    }
    else if ( res >0)
    {
      r=m-1;
    }
    else
    {
      res = strcasecmp(m_name_addr.at(m)->a_addr.c_str(), addr.c_str()); 
      if ( res <0)
      {
        l=m+1;
      }
      else if (res >0)
      {
        r=m-1;
      }
      else return m;
    }
  }
  return -1;
}

unsigned int CVATRegister::medianInvoice  ( void ) const
{
  if (m_inv_amounts.size()==0)
  {
    return 0;
  }
  vector<unsigned int> med {m_inv_amounts};
  sort (med.begin(), med.end());
  int res = med.at((med.size()/2));
  return res;
}


bool CVATRegister::invoice ( const string & taxID, unsigned int amount )
{
  int res {-1};
  res = b_search(taxID);
  if (res ==-1)
  {
    return false;
  }
  m_taxIDs.at(res)->t_my_rev->my_revenue += amount;
  // m_inv_amounts.insert(lower_bound(m_inv_amounts.begin(), m_inv_amounts.end(), amount), amount);
    m_inv_amounts.push_back(amount);
  // sort (m_inv_amounts.begin(), m_inv_amounts.end());
  return true;
}

bool CVATRegister::invoice ( const string & name, const string & addr, unsigned int amount )
{
  int res {-1};
  res = b_search(name, addr);
  if (res ==-1)
  {
    return false;
  }
  else
  {
    m_name_addr.at(res)->na_my_rev->my_revenue += amount;
  }
  // m_inv_amounts.insert(lower_bound(m_inv_amounts.begin(), m_inv_amounts.end(), amount), amount);
  m_inv_amounts.push_back(amount);
  // sort (m_inv_amounts.begin(), m_inv_amounts.end());
  return true;
}


bool CVATRegister::newCompany(const string& name, const string& addr, const string & taxID)
{
  // string new_name {name};
  // string new_addr {addr};
  // string new_taxID {taxID};
  // unsigned int new_revenue {0};

  Stax* new_Stax = new Stax; 
  new_Stax->a_taxID = taxID;
  new_Stax->t_my_rev = NULL;

  Sname_addr* new_Sname_addr = new Sname_addr;
  new_Sname_addr->a_name = name;
  new_Sname_addr->a_addr = addr;
  new_Sname_addr->na_my_rev = NULL;

  SCompany* new_cmpny = new SCompany;
  new_cmpny->my_tax_id = new_Stax; 
  new_cmpny->my_revenue = 0; 
  new_cmpny->my_name_addr = new_Sname_addr;

  new_Stax->t_my_rev = new_cmpny;
  new_Sname_addr->na_my_rev = new_cmpny;
  if (b_search(taxID) !=-1)
  {
    delete new_Stax;
    delete new_cmpny;
    delete new_Sname_addr;
    return false;
  }
  if (b_search(name, addr)!=-1)
  {
    delete new_Stax;
    delete new_cmpny;
    delete new_Sname_addr;
    return false;
  }
  m_taxIDs.insert(lower_bound(m_taxIDs.begin(), m_taxIDs.end(), taxID, [](Stax*& inf, string value)
  {
    return inf->a_taxID < value;
  }), new_Stax);
  m_name_addr.insert(lower_bound(m_name_addr.begin(), m_name_addr.end(), new_Sname_addr, [](Sname_addr*& inf, Sname_addr* new_Sname_addr)
  {
    int res = strcasecmp( inf->a_name.c_str(), new_Sname_addr->a_name.c_str() );
    if ( res<0)
    {
      return true;
    }
    else if ( res>0)
    {
      return false;
    }
    else
    {
      res = strcasecmp( inf->a_addr.c_str(), new_Sname_addr->a_addr.c_str() );
      if(res<0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }), new_Sname_addr);
  m_revs.push_back(new_cmpny);
  return true;
}

bool CVATRegister::audit ( const string & taxID, unsigned int & sumIncome ) const
{
  int res {-1};
  res = b_search(taxID);
  if (res ==-1)
  {
    return false;
  }
  sumIncome=m_taxIDs.at(res)->t_my_rev->my_revenue;
  return true;
}

bool CVATRegister::audit ( const string & name, const string & addr, unsigned int & sumIncome ) const
{
  int res {-1};
  res = b_search(name, addr);
  if (res ==-1)
  {
    return false;
  }
  sumIncome=m_name_addr.at(res)->na_my_rev->my_revenue;
  return true;
}

bool CVATRegister::firstCompany ( string & name, string & addr ) const
{
  if (m_name_addr.size()==0)
  {
    return false;
  }
  name = m_name_addr.at(0)->a_name;
  addr = m_name_addr.at(0)->a_addr;
  return true;
}

bool CVATRegister::nextCompany ( string & name, string & addr ) const
{
  int res {-1};
  res = b_search(name, addr);
  if (res==(int)(m_name_addr.size()-1))
  {
    return false;
  }
  if (res==-1)
  {
    return false;
  }
  name = m_name_addr.at(res+1)->a_name;
  addr = m_name_addr.at(res+1)->a_addr;
  return true;
}

bool CVATRegister::cancelCompany ( const string & name, const string & addr )
{
  int res {-1};
  res = b_search(name, addr);
  if (res ==-1)
  {
    return false;
  }
  auto res_tax_indx = find(begin(m_taxIDs), end(m_taxIDs), m_name_addr.at(res)->na_my_rev->my_tax_id);
  delete m_name_addr.at(res)->na_my_rev->my_tax_id;
  m_name_addr.at(res)->na_my_rev->my_tax_id =NULL;
  m_taxIDs.erase(res_tax_indx);
  
  delete m_name_addr.at(res)->na_my_rev;
  m_name_addr.at(res)->na_my_rev=NULL;
  m_name_addr.at(res)->na_my_rev = m_revs.back();
  m_revs.pop_back();

  auto res_name_addr_indx = find(begin(m_name_addr), end(m_name_addr), m_name_addr.at(res));
  delete m_name_addr.at(res);
  m_name_addr.at(res)=NULL;
  m_name_addr.erase(res_name_addr_indx);
  return true;
}

bool CVATRegister::cancelCompany ( const string & taxID )
{
  int res {-1};
  res = b_search(taxID);
  if (res ==-1)
  {
    return false;
  }
  auto res_name_addr_indx = find(begin(m_name_addr), end(m_name_addr), m_taxIDs.at(res)->t_my_rev->my_name_addr);
  delete m_taxIDs.at(res)->t_my_rev->my_name_addr;
  m_taxIDs.at(res)->t_my_rev->my_name_addr =NULL;
  m_name_addr.erase(res_name_addr_indx);
  
  delete m_taxIDs.at(res)->t_my_rev;
  m_taxIDs.at(res)->t_my_rev=NULL;
  m_taxIDs.at(res)->t_my_rev = m_revs.back();
  m_revs.pop_back();

  auto res_tax_indx = find(begin(m_taxIDs), end(m_taxIDs), m_taxIDs.at(res));
  delete m_taxIDs.at(res);
  m_taxIDs.at(res)=NULL;
  m_taxIDs.erase(res_tax_indx);
  return true;
}

#ifndef __PROGTEST__
int               main           ( void )
{
  string name, addr;
  unsigned int sumIncome;

  CVATRegister b1;
  assert ( b1 . newCompany ( "ACME", "Thakurova", "666/666" ) );
  assert ( b1 . newCompany ( "ACME", "Kolejni", "666/666/666" ) );
  assert ( b1 . newCompany ( "Dummy", "Thakurova", "123456" ) );
  assert ( b1 . invoice ( "666/666", 2000 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "666/666/666", 3000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 4000 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "aCmE", "Kolejni", 5000 ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . audit ( "ACME", "Kolejni", sumIncome ) && sumIncome == 8000 );
  assert ( b1 . audit ( "123456", sumIncome ) && sumIncome == 4000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "ACME" && addr == "Kolejni" );
  assert ( b1 . nextCompany ( name, addr ) && name == "ACME" && addr == "Thakurova" );
  assert ( b1 . nextCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "ACME", "KoLeJnI" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . cancelCompany ( "666/666" ) );
  assert ( b1 . medianInvoice () == 4000 );
  assert ( b1 . invoice ( "123456", 100 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 300 ) );
  assert ( b1 . medianInvoice () == 3000 );
  assert ( b1 . invoice ( "123456", 200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 230 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 830 ) );
  assert ( b1 . medianInvoice () == 830 );
  assert ( b1 . invoice ( "123456", 1830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 1830 );
  assert ( b1 . invoice ( "123456", 2830 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . invoice ( "123456", 3200 ) );
  assert ( b1 . medianInvoice () == 2000 );
  assert ( b1 . firstCompany ( name, addr ) && name == "Dummy" && addr == "Thakurova" );
  assert ( ! b1 . nextCompany ( name, addr ) );
  assert ( b1 . cancelCompany ( "123456" ) );
  assert ( ! b1 . firstCompany ( name, addr ) );

  CVATRegister b2;
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . newCompany ( "Dummy", "Kolejni", "123456" ) );
  assert ( ! b2 . newCompany ( "AcMe", "kOlEjNi", "1234" ) );
  assert ( b2 . newCompany ( "Dummy", "Thakurova", "ABCDEF" ) );
  assert ( b2 . medianInvoice () == 0 );
  assert ( b2 . invoice ( "ABCDEF", 1000 ) );
  assert ( b2 . medianInvoice () == 1000 );
  assert ( b2 . invoice ( "abcdef", 2000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( b2 . invoice ( "aCMe", "kOlEjNi", 3000 ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . invoice ( "1234567", 100 ) );
  assert ( ! b2 . invoice ( "ACE", "Kolejni", 100 ) );
  assert ( ! b2 . invoice ( "ACME", "Thakurova", 100 ) );
  assert ( ! b2 . audit ( "1234567", sumIncome ) );
  assert ( ! b2 . audit ( "ACE", "Kolejni", sumIncome ) );
  assert ( ! b2 . audit ( "ACME", "Thakurova", sumIncome ) );
  assert ( ! b2 . cancelCompany ( "1234567" ) );
  assert ( ! b2 . cancelCompany ( "ACE", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Thakurova" ) );
  assert ( b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . medianInvoice () == 2000 );
  assert ( ! b2 . cancelCompany ( "abcdef" ) );
  assert ( b2 . newCompany ( "ACME", "Kolejni", "abcdef" ) );
  assert ( b2 . cancelCompany ( "ACME", "Kolejni" ) );
  assert ( ! b2 . cancelCompany ( "ACME", "Kolejni" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
