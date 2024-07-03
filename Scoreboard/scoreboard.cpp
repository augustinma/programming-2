#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */


template <typename M_>
class CContest
{
  public:
    // default constructor
    CContest ( ){
      m_lookupTable.clear();
      m_results.clear();
      // m_key_defeated_val.clear();
      // m_key_lost_val.clear();
      // m_key_draw_val.clear();
    }
    // destructor
    ~CContest ( ){

    }
    // addMatch ( contestant1, contestant2, result )
    CContest& addMatch ( const string& contestant1, const string& contestant2, const M_& result ){
      
      size_t hsh1 = hash<string>{}(contestant1);
      size_t hsh2 = hash<string>{}(contestant2);
      if(m_lookupTable.find(hsh1)==m_lookupTable.end()){
        m_lookupTable.insert({hsh1, contestant1});
        // m_key_defeated_val.insert({hsh1, {}});
        // m_key_lost_val.insert({hsh1, {}});
        // m_key_draw_val.insert({hsh1, {}});
      }
      if(m_lookupTable.find(hsh2)==m_lookupTable.end()){
        m_lookupTable.insert({hsh2, contestant2});
        // m_key_defeated_val.insert({hsh2, {}});
        // m_key_lost_val.insert({hsh2, {}});
        // m_key_draw_val.insert({hsh2, {}});

      }
      
      if(m_results.find({hsh1, hsh2})==m_results.end() and m_results.find({hsh2, hsh1})==m_results.end()){
        m_results.insert({{hsh1, hsh2}, {}});
      }
      else{
         throw std::logic_error("error addMatch");
      }
      m_results.at({hsh1, hsh2}).push_back(result);

      return *this;
    }
    // isOrdered ( comparator )
    template <typename Func>
    bool isOrdered ( Func func )const{
      std::map<size_t, set<size_t>> m_key_defeated_val;
      std::map<size_t, set<size_t>> m_key_lost_val;
      for(auto iter = m_lookupTable.begin(); iter!=m_lookupTable.end(); iter++){
        m_key_defeated_val.insert({(*iter).first, {}});
        m_key_lost_val.insert({(*iter).first, {}});
      }
      for(auto iter = m_results.begin(); iter != m_results.end(); iter++){
        for(auto x : (*iter).second){
          int res = INT16_MAX;
          res = func(x);
          size_t contestant1 = (*iter).first.first;
          size_t contestant2 = (*iter).first.second;
          if (res < 0){
            // contestant1 lost
            m_key_defeated_val.at(contestant2).insert(contestant1);
            m_key_lost_val.at(contestant1).insert(contestant2);
          }
          else if (res > 0){
            // contestant1 won
            m_key_defeated_val.at(contestant1).insert(contestant2);
            m_key_lost_val.at(contestant2).insert(contestant1);
          }
          else{
            // draw
            // m_key_draw_val.at(contestant1).insert(contestant2);
            // m_key_draw_val.at(contestant2).insert(contestant1);
            m_key_defeated_val.at(contestant1).insert(contestant2);
            m_key_defeated_val.at(contestant2).insert(contestant1);
            m_key_lost_val.at(contestant1).insert(contestant2);
            m_key_lost_val.at(contestant2).insert(contestant1);
          }
        }
      }
      set<size_t> sink;
      set<size_t> contestants;
      for(auto iter = m_key_defeated_val.begin(); iter!=m_key_defeated_val.end(); iter++){
        contestants.insert((*iter).first);
        if((*iter).second.empty()){
          sink.insert((*iter).first);
        }
      }
      if(sink.size()!=1){
        // cout << "new sinks: ";
        // for(auto x : sink){
        //   cout <<  m_lookupTable.at(x) << ", " << flush;
        // }
        // cout << endl;
        return false;
      }
      while(contestants.empty()==false){
        set<size_t> new_possible_sinks;
        for(auto iter = m_key_lost_val.at(*(sink.begin())).begin(); iter != m_key_lost_val.at(*(sink.begin())).end(); iter++){
          m_key_defeated_val.at(*iter).erase(*(sink.begin()));
          new_possible_sinks.insert(*iter);
        }
        contestants.erase(*(sink.begin()));
        sink.clear();
        for(auto iter = new_possible_sinks.begin(); iter!= new_possible_sinks.end(); iter++){
          if (m_key_defeated_val.at(*iter).empty()){
            sink.insert((*iter));
          }
        }
        if(contestants.empty()==false && sink.size()!=1){
          // cout << "new sinks: ";
          // for(auto x : sink){
          //   cout <<  m_lookupTable.at(x) << ", " << flush;
          // }
          // cout << endl;
          return false;
      }
    }
    return true;
  }
    // results ( comparator )
    template <typename Func>
  list<string> results ( Func func )const{
    std::map<size_t, set<size_t>> m_key_defeated_val;
    std::map<size_t, set<size_t>> m_key_lost_val;
    for(auto iter = m_lookupTable.begin(); iter!=m_lookupTable.end(); iter++){
      m_key_defeated_val.insert({(*iter).first, {}});
      m_key_lost_val.insert({(*iter).first, {}});
    }
    for(auto iter = m_results.begin(); iter != m_results.end(); iter++){
      for(auto x : (*iter).second){
        int res = INT16_MAX;
        res = func(x);
        size_t contestant1 = (*iter).first.first;
        size_t contestant2 = (*iter).first.second;
        if (res < 0){
          // contestant1 lost
          m_key_defeated_val.at(contestant2).insert(contestant1);
          m_key_lost_val.at(contestant1).insert(contestant2);
        }
        else if (res > 0){
          // contestant1 won
          m_key_defeated_val.at(contestant1).insert(contestant2);
          m_key_lost_val.at(contestant2).insert(contestant1);
        }
        else{
          // draw
          // m_key_draw_val.at(contestant1).insert(contestant2);
          // m_key_draw_val.at(contestant2).insert(contestant1);
          m_key_defeated_val.at(contestant1).insert(contestant2);
          m_key_defeated_val.at(contestant2).insert(contestant1);
          m_key_lost_val.at(contestant1).insert(contestant2);
          m_key_lost_val.at(contestant2).insert(contestant1);
        }
      }
    }
    set<size_t> sink;
    set<size_t> contestants;
    for(auto iter = m_key_defeated_val.begin(); iter!=m_key_defeated_val.end(); iter++){
      contestants.insert((*iter).first);
      if((*iter).second.empty()){
        sink.insert((*iter).first);
      }
    }
    list <size_t> hsh_scoreboard;
    if(sink.size()!=1){
      // cout << "new sinks: ";
      // for(auto x : sink){
      //   cout <<  m_lookupTable.at(x) << ", " << flush;
      // }
      // cout << endl;
      throw std::logic_error("error results");
    }
    
    while(contestants.empty()==false){
      set<size_t> new_possible_sinks;
      for(auto iter = m_key_lost_val.at(*(sink.begin())).begin(); iter != m_key_lost_val.at(*(sink.begin())).end(); iter++){
        m_key_defeated_val.at(*iter).erase(*(sink.begin()));
        new_possible_sinks.insert(*iter);
      }
      contestants.erase(*(sink.begin()));
      hsh_scoreboard.push_back(*(sink).begin());
      sink.clear();
      for(auto iter = new_possible_sinks.begin(); iter!= new_possible_sinks.end(); iter++){
        if (m_key_defeated_val.at(*iter).empty()){
          sink.insert((*iter));
        }
      }
      if(contestants.empty()==false && sink.size()!=1){
        // cout << "new sinks: ";
        // for(auto x : sink){
        //   cout <<  m_lookupTable.at(x) << ", " << flush;
        // }
        // cout << endl;
        throw std::logic_error("error results");
        
      }
    }
    list<string>scoreboard;
    for(auto x : hsh_scoreboard){
      scoreboard.push_front(m_lookupTable.at(x));
    }
    return scoreboard;
  }
  private:
    // todo
    std::map<size_t, string> m_lookupTable;
    std::map<pair<size_t, size_t>, vector<M_>> m_results;
    // std::map<size_t, set<size_t>> m_key_defeated_val;
    // std::map<size_t, set<size_t>> m_key_lost_val;
    // std::map<size_t, set<size_t>> m_key_draw_val;
    
};

#ifndef __PROGTEST__
struct CMatch
{
  public:
                             CMatch                        ( int               a, 
                                                             int               b )
      : m_A ( a ), 
        m_B ( b )
    {
    }
    
    int                      m_A;
    int                      m_B;
};

class HigherScoreThreshold
{
  public:
                             HigherScoreThreshold          ( int diffAtLeast )
      : m_DiffAtLeast ( diffAtLeast )
    {
    }
    int                      operator ()                   ( const CMatch & x ) const
    {
      return ( x . m_A > x . m_B + m_DiffAtLeast ) - ( x . m_B > x . m_A + m_DiffAtLeast );
    }
  private:
    int            m_DiffAtLeast;    
};

int                HigherScore                             ( const CMatch    & x )
{
  return ( x . m_A > x . m_B ) - ( x . m_B > x . m_A );
}

int                main                                    ( void )
{
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
