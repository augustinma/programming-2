#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect
{
  public:
                             CRect                         ( double            x,
                                                             double            y,
                                                             double            w,
                                                             double            h )
      : m_X ( x ),
        m_Y ( y ),
        m_W ( w ),
        m_H ( h )
    {
    }
    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CRect     & x )
    {
      return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double                   m_X;
    double                   m_Y;
    double                   m_W;
    double                   m_H;
};
#endif /* __PROGTEST__ */

int level=0;

class CComponent
{
  public:
    int m_id;
    CRect m_relPos;
    CRect m_absPos;
    // int* m_level;
    // int getID()const{return m_id;}
    // const CRect& get_relPos()const{return m_relPos;}
    // bool set_m_absPos(CRect& absPos){m_absPos=absPos;}
    CComponent(int id, const CRect& relPos) : m_id(id), m_relPos(relPos), m_absPos(CRect(-1, -1, -1, -1)){/*m_level = new int;*/}
    CComponent(const CComponent& other) // II. copy constructor
    : CComponent(other.m_id, other.m_relPos) { m_absPos=other.m_absPos;/* (*m_level)=*(other.m_level);*/ }
    virtual CComponent& operator=(const CComponent& other) // III. copy assignment
    {
        if (this == &other)
        {
          return *this;
        }
        
        int new_m_id(other.m_id);
        m_id=new_m_id;

        CRect new_m_relPos(other.m_relPos);
        m_relPos=new_m_relPos;
        
        CRect new_m_absPos(other.m_absPos);
        m_absPos=new_m_absPos;

        return *this;
    }
    friend ostream& operator << (ostream& os, const CComponent &base);
    friend ostream& print_last(ostream& os, const CComponent &base);
    virtual void last(ostream& os) const{
      os << flush;
    }
    virtual void serialize(ostream& os) const
    {

      os << this->m_relPos << '\n';
      // cout << this->m_relPos << '\n';
    }
    virtual CComponent* clone(){
      return new CComponent(*this);
    }
    virtual ~CComponent(){
      // delete m_level;
      // m_level=nullptr;
    }
};

class CButton : public CComponent
{
  string m_name;
  public:
                             CButton                       ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & name );
    CButton(const CButton& other) // II. copy constructor
    : CButton(other.m_id, other.m_relPos, other.m_name) {this->m_absPos=other.m_absPos;/* (*m_level)=*(other.m_level);*/}
    CButton& operator=(const CButton& other) // III. copy assignment
    {
        if (this == &other)
        {
          return *this;
        }
        int new_m_id(other.m_id);
        m_id=new_m_id;

        CRect new_m_relPos(other.m_relPos);
        m_relPos=new_m_relPos;
        
        CRect new_m_absPos(other.m_absPos);
        m_absPos=new_m_absPos;

        string new_m_name(other.m_name);
        m_name = new_m_name;
        return *this;
    }
    friend ostream& operator << (ostream& os, const CButton &base);
    virtual void serialize(ostream& os) const
    {
      if ((/*this->m_*/level)>0){
        os << "+- " << "[" << this->m_id << "] " << "Button \"" << this->m_name << "\" " << this->m_absPos << '\n';
        // cout  << "+- " << "[" << this->m_id << "] " << "Button \"" << this->m_name << "\"  " << this->m_absPos << '\n';
      }
      else{
        os << "[" << this->m_id << "] " << "Button \"" << this->m_name << "\" " << this->m_absPos << '\n';
        // cout  << "[" << this->m_id << "] " << "Button \"" << this->m_name << "\"  " << this->m_absPos << '\n';
      }
    }
    virtual CButton* clone(){
      return new CButton(*this);
    }
    ~CButton(){
      
    }
};

class CLabel : public CComponent
{
  string m_label;
  public:
                             CLabel                       ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & label );
    CLabel(const CLabel& other) // II. copy constructor
    : CLabel(other.m_id, other.m_relPos, other.m_label) {this->m_absPos=other.m_absPos;/* (*m_level)=*(other.m_level);*/}
    CLabel& operator=(const CLabel& other) // III. copy assignment
    {
        if (this == &other)
        {
          return *this;
        }
        int new_m_id(other.m_id);
        m_id=new_m_id;

        CRect new_m_relPos(other.m_relPos);
        m_relPos=new_m_relPos;
        
        CRect new_m_absPos(other.m_absPos);
        m_absPos=new_m_absPos;

        string new_m_label(other.m_label);
        m_label = new_m_label;

        return *this;
    }
    friend ostream& operator << (ostream& os, const CLabel &base);
    virtual void serialize(ostream& os) const
    {
      if ((/*this->m_*/level)>0){
        os << "+- " << "[" << this->m_id << "] " << "Label \"" << this->m_label << "\" " << this->m_absPos << '\n';
        // cout  << "+- " << "[" << this->m_id << "] " << "Label \"" << this->m_label << "\"  " << this->m_absPos << '\n';
      }
      else{
        os <<  "[" << this->m_id << "] " << "Label \"" << this->m_label << "\" " << this->m_absPos << '\n';
        // cout  << "[" << this->m_id << "] " << "Label \"" << this->m_label << "\"  " << this->m_absPos << '\n';
      }
    }
    virtual CLabel* clone(){
      return new CLabel(*this);
    }
    ~CLabel(){
      
    }
};

class CInput : public CComponent
{
  string m_value;
  public:
                             CInput                        ( int               id,
                                                             const CRect     & relPos,
                                                             const string    & value );
    // setValue
    void setValue(const string& x){
      m_value = x;
    }
    // getValue
    string getValue(){
      return m_value;
    }
    CInput(const CInput& other) // II. copy constructor
    : CInput(other.m_id, other.m_relPos, other.m_value) {this->m_absPos=other.m_absPos;/* (*m_level)=*(other.m_level);*/}
    CInput& operator=(const CInput& other) // III. copy assignment
    {
        if (this == &other)
        {
          return *this;
        }
        int new_m_id(other.m_id);
        m_id=new_m_id;

        CRect new_m_relPos(other.m_relPos);
        m_relPos=new_m_relPos;
        
        CRect new_m_absPos(other.m_absPos);
        m_absPos=new_m_absPos;

        string new_m_value(other.m_value);
        m_value = new_m_value;

        return *this;
    }
    friend ostream& operator << (ostream& os, const CInput &base);
    virtual void serialize(ostream& os) const
    {
      if((/*this->m_*/level)>0){
        os << "+- " << "[" << this->m_id << "] " << "Input \"" << this->m_value << "\" " << this->m_absPos << '\n';
        // cout  << "+- " << "[" << this->m_id << "] " << "Input \"" << this->m_value << "\"  " << this->m_absPos << '\n';
      }
      else{
        os << "[" << this->m_id << "] " << "Input \"" << this->m_value << "\" " << this->m_absPos << '\n';
        // cout  << "[" << this->m_id << "] " << "Input \"" << this->m_value << "\"  " << this->m_absPos << '\n';
      }
          
    }
    virtual CInput* clone(){
      return new CInput(*this);
    }
    ~CInput(){
      
    }
};

class CComboBox : public CComponent
{
  public:
  vector<string> m_strs;
  size_t* m_slected;
  // bool m_isLast;
  
                             CComboBox                     ( int               id,
                                                             const CRect     & relPos );
    // add
    CComboBox& add(const string& str){
      string new_str(str);
       m_strs.push_back(new_str);
       return *this;
    }
    // setSelected
    void setSelected(size_t x) {
      (*m_slected)=x;
    }
    // getSelected
    size_t getSelected(){
      return *(this->m_slected);
    }
    CComboBox(const CComboBox& other) // II. copy constructor
    : CComboBox(other.m_id, other.m_relPos) {
      *(m_slected) = *(other.m_slected);
      // m_isLast = other.m_isLast;
      for(auto iter = other.m_strs.begin(); iter!=other.m_strs.end(); iter++){
        this->m_strs.push_back((*iter));
      }
      this->m_absPos=other.m_absPos;/* (*m_level)=*(other.m_level);*/
    }
    CComboBox& operator=(const CComboBox& other) // III. copy assignment
    {
        if (this == &other)
        {
          return *this;
        }
        int new_m_id(other.m_id);
        m_id=new_m_id;

        CRect new_m_relPos(other.m_relPos);
        m_relPos=new_m_relPos;
        
        CRect new_m_absPos(other.m_absPos);
        m_absPos=new_m_absPos;

        m_strs.clear();
        for(auto iter = other.m_strs.begin(); iter!=other.m_strs.end(); iter++){
          m_strs.push_back((*iter));
        }

        delete m_slected;
        m_slected = new size_t;
        (*m_slected) = *(other.m_slected);

        return *this;
    }
    friend ostream& operator << (ostream& os, const CComboBox &base);
    friend ostream& print_last(ostream& os, const CComboBox &base);
    virtual void last(ostream& os) const{
      // cout << "+- " << "[" << this->m_id << "] " << "ComboBox "  << this->m_absPos << '\n';
      os << "+- " << "[" << this->m_id << "] " << "ComboBox "  << this->m_absPos << '\n';
      for (size_t i = 0; i < m_strs.size(); i++){
        if (i==(*m_slected)){
          // cout << "   +->" << m_strs.at(i) << "<\n";
          os << "   +->" << m_strs.at(i) << "<\n";
        }
        else{
          // cout << "   +- " << m_strs.at(i) << '\n';
          os << "   +- " << m_strs.at(i) << '\n';
        }
      }
    }
    virtual void serialize(ostream& os) const
    {
      if ((/*this->m_*/level)>0){
        // cout << "+- " << "[" << this->m_id << "] " << "ComboBox "  << this->m_absPos << '\n';
        os << "+- " << "[" << this->m_id << "] " << "ComboBox "  << this->m_absPos << '\n';
        for (size_t i = 0; i < m_strs.size(); i++){
          if (i==(*m_slected)){
            // cout << "|  +->" << m_strs.at(i) << "<\n";
            os << "|  +->" << m_strs.at(i) << "<\n";
          }
          else{
            // cout << "|  +- " << m_strs.at(i) << '\n';
            os << "|  +- " << m_strs.at(i) << '\n';
          }
        }
      }else{
        // cout << "[" << this->m_id << "] " << "ComboBox "  << this->m_absPos << '\n';
        os << "[" << this->m_id << "] " << "ComboBox "  << this->m_absPos << '\n';
        for (size_t i = 0; i < m_strs.size(); i++){
          if (i==(*m_slected)){
            // cout << "+->" << m_strs.at(i) << "<\n";
            os << "+->" << m_strs.at(i) << "<\n";
          }
          else{
            // cout << "+- " << m_strs.at(i) << '\n';
            os << "+- " << m_strs.at(i) << '\n';
          }
        }
      }
          // cout  << "+- " << "[" << this->m_id << "] " << "ComboBox \"" << this->m_value << "\"  " << this->m_absPos << '\n';
    }
    virtual CComboBox* clone(){
      return new CComboBox(*this);
    }

    ~CComboBox(){
      delete m_slected;
      m_slected = nullptr;
    }
};

class CWindow
{
  int m_id;
  string m_title;
  CRect m_absPos;
  CComponent* m_combobox;
  multimap<int, CComponent*> cmpnts;
  friend void abs(CWindow& win, const CComponent& x, double& a, double& b, double& c, double& d);
  public:
                             CWindow                       ( int               id,
                                                             const string    & title,
                                                             const CRect     & absPos );
    CWindow(const CWindow& other) // II. copy constructor
    : CWindow(other.m_id, other.m_title, other.m_absPos) {
      for(auto iter = other.cmpnts.begin(); iter!=other.cmpnts.end(); iter++){
        auto z = this->cmpnts.insert({(*iter).first, (*iter).second->clone()});
        if ((other.m_combobox!=nullptr) && (other.m_combobox==(*iter).second)){
          this->m_combobox=z->second;
        }
      }
    }

    CWindow& operator=(const CWindow& other) // III. copy assignment
    {
        if (this == &other)
        {
          return *this;
        }
        int new_m_id(other.m_id);
        m_id=new_m_id;

        string new_m_title(other.m_title);
        m_title = new_m_title;

        CRect new_m_absPos(other.m_absPos);
        m_absPos=new_m_absPos;

        
        for(auto iter = this->cmpnts.begin(); iter!=this->cmpnts.end(); iter++){
          delete (*iter).second;
          (*iter).second = nullptr;
        }
        cmpnts.clear();
        for(auto iter = other.cmpnts.begin(); iter!=other.cmpnts.end(); iter++){
          auto z = this->cmpnts.insert({(*iter).first, (*iter).second->clone()});
          if ((other.m_combobox!=nullptr) && (other.m_combobox==(*iter).second)){
            this->m_combobox=z->second;
          }
        }

        

        return *this;
    }

    // add
    CWindow& add(const CButton& x);
    CWindow& add(const CLabel& x);
    CWindow& add(const CInput& x);
    CWindow& add(const CComboBox& x);
    // search
    CComponent* search(int id) const {
      auto x = this->cmpnts.find(id);
      if (x==this->cmpnts.end()){
        return nullptr;
      }
      else{
        return x->second;
      }
    }
    // setPosition
    void setPosition(const CRect& newAbsPos){
      m_absPos.m_X= newAbsPos.m_X; 
      m_absPos.m_Y=newAbsPos.m_Y;
      m_absPos.m_W= newAbsPos.m_W;
      m_absPos.m_H=newAbsPos.m_H;
      for(auto iter=this->cmpnts.begin(); iter!=this->cmpnts.end(); iter++){
        abs(*this, *((*iter).second), ((*iter).second)->m_absPos.m_X, ((*iter).second)->m_absPos.m_Y, ((*iter).second)->m_absPos.m_W, ((*iter).second)->m_absPos.m_H);
      }
    }

    friend ostream         & operator <<                   ( ostream         & os,
                                                             const CWindow     & x )
    {
      // cout << "[" << x.m_id << "] " << "Window \"" << x.m_title << "\" " << x.m_absPos << '\n';
      os << "[" << x.m_id << "] " << "Window \"" << x.m_title << "\" " << x.m_absPos << '\n';
      level=1;
      for(auto y = x.cmpnts.begin(); y!=x.cmpnts.end(); y++){
        if ((x.m_combobox!=nullptr) &&(*y).second==x.m_combobox){
          print_last(os, *((*y).second));
        }
        else{
          os << *((*y).second) ;
        }
        // auto rectangle = y->getRelPos();
        // int a = x.m_absPos.m_W * rectangle.m_X + x.m_absPos.m_X;
        // int b = x.m_absPos.m_H * rectangle.m_Y + x.m_absPos.m_Y;
        // int c = x.m_absPos.m_W * rectangle.m_W;
        // int d = x.m_absPos.m_H * rectangle.m_H;
        // os << CRect(a, b, c, d) << '\n';
        // cout << CRect(a, b, c, d) << '\n';
      }
      level=0;
      return os;
    }

    ~CWindow(){
      for(auto iter = this->cmpnts.begin(); iter!=this->cmpnts.end(); iter++){
        delete (*iter).second;
        (*iter).second = nullptr;
      }
    }
};

CWindow::CWindow ( int               id,
                   const string    & title,
                   const CRect     & absPos ) : m_id(id), m_title(title), m_absPos(absPos)
{
  m_combobox=nullptr;
}

void abs(CWindow& win, const CComponent& x, double& a, double& b, double& c, double& d){
  a= win.m_absPos.m_W * x.m_relPos.m_X + win.m_absPos.m_X;
  b = win.m_absPos.m_H * x.m_relPos.m_Y + win.m_absPos.m_Y;
  c = win.m_absPos.m_W * x.m_relPos.m_W;
  d = win.m_absPos.m_H * x.m_relPos.m_H;
}

CWindow& CWindow::add(const CButton& x){ 
  double a, b, c, d;
  abs(*this, x, a, b, c, d);
  // y.m_absPos.m_X=a;
  // y.m_absPos.m_Y=b;
  // y.m_absPos.m_W=c;
  // y.m_absPos.m_H=d;
  auto y = cmpnts.insert({x.m_id, new CButton(x)});
  y->second->m_absPos.m_X=a;
  y->second->m_absPos.m_Y=b;
  y->second->m_absPos.m_W=c;
  y->second->m_absPos.m_H=d;
  // *(y->second->m_level)=1;
  this->m_combobox=nullptr;
  return *this;
}

CWindow& CWindow::add(const CLabel& x){ 
  double a, b, c, d;
  abs(*this, x, a, b, c, d);
  // y.m_absPos.m_X=a;
  // y.m_absPos.m_Y=b;
  // y.m_absPos.m_W=c;
  // y.m_absPos.m_H=d;
  auto y = cmpnts.insert({x.m_id, new CLabel(x)});
  y->second->m_absPos.m_X=a;
  y->second->m_absPos.m_Y=b;
  y->second->m_absPos.m_W=c;
  y->second->m_absPos.m_H=d;
  // *(y->second->m_level)=1;
  this->m_combobox=nullptr;
  return *this;
}

CWindow& CWindow::add(const CInput& x){ 
  double a, b, c, d;
  abs(*this, x, a, b, c, d);
  // y.m_absPos.m_X=a;
  // y.m_absPos.m_Y=b;
  // y.m_absPos.m_W=c;
  // y.m_absPos.m_H=d;
  auto y = cmpnts.insert({x.m_id, new CInput(x)});
  y->second->m_absPos.m_X=a;
  y->second->m_absPos.m_Y=b;
  y->second->m_absPos.m_W=c;
  y->second->m_absPos.m_H=d;
  // *(y->second->m_level)=1;
  this->m_combobox=nullptr;
  return *this;
}

CWindow& CWindow::add(const CComboBox& x){ 
  double a, b, c, d;
  abs(*this, x, a, b, c, d);
  // y.m_absPos.m_X=a;
  // y.m_absPos.m_Y=b;
  // y.m_absPos.m_W=c;
  // y.m_absPos.m_H=d;
  auto y = cmpnts.insert({x.m_id, new CComboBox(x)});
  y->second->m_absPos.m_X=a;
  y->second->m_absPos.m_Y=b;
  y->second->m_absPos.m_W=c;
  y->second->m_absPos.m_H=d;
  // *(y->second->m_level)=1;
  this->m_combobox=y->second;
  return *this;
}

CButton::CButton ( int               id, 
                   const CRect     & relPos,
                   const string    & name ) : CComponent(id, relPos), m_name(name)
{

}

CLabel::CLabel                       ( int               id,
                                      const CRect     & relPos,
                                      const string    & label ) : CComponent(id, relPos), m_label(label)
{

}

CInput::CInput                       ( int               id,
                                      const CRect     & relPos,
                                      const string    & value ) : CComponent(id, relPos), m_value(value)
{

}

CComboBox::CComboBox                     ( int               id,
                                           const CRect     & relPos ) : CComponent(id, relPos)
{
  m_strs.clear();
  m_slected=new size_t(0);
  // m_isLast=false;
}

// output operators
ostream& print_last(ostream& os, const CComponent &base)
{
  base.last(os);
  return os;
}
ostream& operator << (ostream& os, const CComponent &base)
{
    base.serialize(os);
    return os;
}

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  // cout <<  ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) ) << endl;
  // cout << sizeof ( CButton ) << " - "<< sizeof ( string ) << " = " << sizeof ( CButton ) - sizeof ( string ) << " " << sizeof ( CComboBox ) << " - " << sizeof ( vector<string> ) << " = " << sizeof ( CComboBox ) - sizeof ( vector<string> )  << endl;
  // cout << sizeof ( CInput ) - sizeof ( string ) << endl;
  assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
  CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
  a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
  a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
  a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
  a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  CWindow b = a;
  assert ( toString ( *b . search ( 20 ) ) ==
    "[20] ComboBox (70,154,480,48)\n"
    "+->Karate<\n"
    "+- Judo\n"
    "+- Box\n"
    "+- Progtest\n" );
  assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
  dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
  assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
  dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
  b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (70,250,480,48)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (10,10,600,480)\n"
    "+- [1] Button \"Ok\" (70,394,180,48)\n"
    "+- [2] Button \"Cancel\" (370,394,180,48)\n"
    "+- [10] Label \"Username:\" (70,58,120,48)\n"
    "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
    "+- [20] ComboBox (70,154,480,48)\n"
    "   +->Karate<\n"
    "   +- Judo\n"
    "   +- Box\n"
    "   +- Progtest\n" );
  b . setPosition ( CRect ( 20, 30, 640, 520 ) );
  assert ( toString ( b ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );

  CWindow c = b;
  a=c;
  assert ( toString ( a ) ==
    "[0] Window \"Sample window\" (20,30,640,520)\n"
    "+- [1] Button \"Ok\" (84,446,192,52)\n"
    "+- [2] Button \"Cancel\" (404,446,192,52)\n"
    "+- [10] Label \"Username:\" (84,82,128,52)\n"
    "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
    "+- [20] ComboBox (84,186,512,52)\n"
    "|  +- Karate\n"
    "|  +- Judo\n"
    "|  +- Box\n"
    "|  +->Progtest<\n"
    "+- [21] ComboBox (84,290,512,52)\n"
    "   +->PA2<\n"
    "   +- OSY\n"
    "   +- Both\n" );

  CButton x1 = CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" );
  CButton y1 = CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" );

  x1=y1;

  CComboBox x2 =  CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" );
  CComboBox y2 =  CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" );
  
  y2=x2;

  CLabel x3 = CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" );
  CLabel y3 = CLabel ( 20, CRect ( 0.1, 0.1, 0.1, 0.1 ), "Username78okjhbvcft:" );

  x3 = y3;

  CInput x4 = CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" );
  CInput y4 = CInput ( 5, CRect ( 0.2, 0.8, 0.3, 0.5 ), "chucknorrisghjhgfgh" );

  x4 = y4;

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
