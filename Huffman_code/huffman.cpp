#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
#include <bitset>
using namespace std;
#endif /* __PROGTEST__ */


struct leaf{
  u_int8_t num_bytes;
  u_int8_t val[4];
};

struct node{
  node* zero_node;
  node* one_node;
  leaf* zero_val;
  leaf* one_val;
};

struct Tree{
  node** head;
};

bool extract_bit(int i, u_int8_t a)
{
  const u_int8_t mask[8]={
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001};
  
  if ((mask[i] & a)==mask[i])
  {
    return true;
  }
  else
  {
    return false;
  }
}

void recur(node * n, node*& cur)
{
  if (n && n->one_val && n->zero_val)
  {
    return;
  }
  if (n && ((n->zero_node==NULL && n->zero_val==NULL) || (n->one_node==NULL && n->one_val==NULL)))
  {
    cur=n;
    // return;
  }
  if (n && n->zero_val==NULL) 
  {
    recur(n->zero_node, cur);
  }
  if (n && n->one_val==NULL) 
  {
    recur(n->one_node, cur);
  }
}

u_int8_t len_ch(u_int8_t a)
{
  const u_int8_t mask_1={0b10000000};
  const u_int8_t mask_2={0b11000000};
  const u_int8_t mask_3{0b11100000};
  const u_int8_t mask_4{0b11110000};
  const u_int8_t mask_5{0b11111000};
  const u_int8_t zero  {0b00000000};
  u_int8_t res   {0};
  if ((a & mask_1)==zero)
  {
    res =1;
    return res;
  }
  else if ((a & mask_5)==mask_5)
  {
    return res;
  }
  else if ((a & mask_4)==mask_4)
  {
    res =4;
    return res;
  }
  else if ((a & mask_3)==mask_3)
  {
    res =3;
    return res;
  }
  else if ((a & mask_2)==mask_2)
  {
    res =2;
    return res;
  }
  else
  {
    return res;
  }
}

bool readCode(ifstream& f, int& offset, vector<int>& code)
{
  char a;
  if (f.fail()) return false;
  f.get(a);
  u_int8_t a_a {(u_int8_t)a};
  u_int8_t a_b {0};
  u_int8_t r {0};
  if (offset!=0)
  {
    a_a = a_a << offset;
    a_b = (u_int8_t)f.peek();
    a_b = a_b >> (8-offset);
    r = (a_a | a_b);
  }
  else
  {
    r = a_a;
  }
  int i {0};
  while (extract_bit(i, r)==false)
  {
    code.push_back(0);
    i=(i+1)%8;
    offset=offset+1;
    if ((offset%8)==0)
    {
      if (f.fail()) return false;
      f.get(a);
      r=(u_int8_t)a;
      i=0;
      offset=(offset%8);
    }
  }
  code.push_back(1);
  offset=(offset+1)%8;
  if ((offset%8)==0)
  {
    f.get(a);
  }
  f.unget();
  return true;
}

bool readChar(ifstream& f, int& offset, leaf*& a_leaf)
{
  char a;
  u_int8_t a_a {0};
  u_int8_t a_b {0};
  u_int8_t r {0};
  int i= {0};
  do
  {
    if (f.fail()) return false;
    f.get(a);
    a_a = (u_int8_t)a;
    // cout << "a_a= " << bitset<8>(a_a) << endl;
    if (offset!=0)
    {
      a_a = a_a << offset;
      // cout << "a_a= " << bitset<8>(a_a) << endl;
      a_b = (u_int8_t)f.peek();
      // cout << "a_b= " << bitset<8>(a_b) << endl;
      a_b = a_b >> (8-offset);
      // cout << "a_b= " << bitset<8>(a_b) << endl;
      r = (a_a | a_b);
      // cout << "r= " << bitset<8>(r) << endl;
      // cout << (char)r << endl;
    }
    else
    {
      r=a_a;
    }
    if (i==0)
    {
      a_leaf->num_bytes = len_ch(r);
      if (a_leaf->num_bytes==0)
      {
        delete a_leaf;
        a_leaf=NULL;
        return false;
      }
    }
    if (i>0)
    {
      if (!((extract_bit(0, r)==true) && (extract_bit(1, r)==false)))
      {
        delete a_leaf;
        a_leaf=NULL;
        return false;
      }
    }
    a_leaf->val[i]=r;
    i++;
  } while (i<a_leaf->num_bytes);
  if (a_leaf->num_bytes==4)
  {
    u_int8_t mask_val0={0b00000111};
    u_int8_t mask_val1_2_3={0b00111111};
    u_int32_t tmp0 {0};
    u_int32_t tmp1 {0};
    u_int32_t tmp2 {0};
    u_int32_t tmp3 {0};
    tmp0 = (a_leaf->val[0] & mask_val0);
    tmp0= tmp0 << 18;
    tmp1 = (a_leaf->val[1] & mask_val1_2_3);
    tmp1 = tmp1 << 12;
    tmp2 = (a_leaf->val[2] & mask_val1_2_3);
    tmp2 = tmp2 << 6;
    tmp3 = (a_leaf->val[3] & mask_val1_2_3);
    u_int32_t res {0};
    res=(tmp0 | tmp1 |tmp2 |tmp3);
    // cout << bitset<32>(res) << endl;
    if (res > (u_int32_t)0x10ffff)
    {
      return false;
    }
  }
  return true;
}

bool is_end_tree(node**root)
{
  node * a_node {NULL};
  recur(*root, a_node);
  if (a_node==NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void del_bin_t(node* n)
{
  if (n!=NULL)
  {
    del_bin_t(n->zero_node);
    del_bin_t(n->one_node);
    if (n->zero_val)
    {
      delete n->zero_val;
      n->zero_val=NULL;
    }
    if(n->one_val)
    {
      delete n->one_val;
      n->one_val=NULL;
    }
    delete n;
    n=NULL;
  }
}

node** create_binary_tree(ifstream & f, int& offset)
{
  node **root = new node*;
  *root=NULL;
  bool end_bin_tree=false;
  vector<int> code;
  leaf * a_leaf {NULL};
  node* a_node {NULL};
  node* cur = NULL;
  while (end_bin_tree==false)
  {
    code.clear();
    a_leaf = new leaf;
    for(int i=0; i<4; i++)
    {
      a_leaf->val[i]=255;
    }
    if (readCode(f, offset, code)==false)
    {
      del_bin_t(*root);
      delete root;
      root=NULL;
      delete a_leaf;
      a_leaf=NULL;
      return NULL;
    } 
    if (readChar(f, offset, a_leaf)==false)
    {
      del_bin_t(*root);
      delete root;
      root=NULL;
      delete a_leaf;
      a_leaf=NULL;
      return NULL;
    }
    for (size_t i=0; i<code.size(); i++)
    {
      if (code.at(i)==0)
      {
        if ((*root)==NULL)
        {
          a_node = new node;
          a_node->one_node=NULL;
          a_node->one_val=NULL;
          a_node->zero_node=NULL;
          a_node->zero_val=NULL;
          // node* tmp {a_node};
          // a_node=NULL;
          *root=a_node;
          a_node=NULL;
          cur = *root;
        }
        else
        {
          a_node = new node;
          a_node->one_node=NULL;
          a_node->one_val=NULL;
          a_node->zero_node=NULL;
          a_node->zero_val=NULL;
          // node * tmp = a_node;
          // a_node=NULL;
          recur(*root, cur);
          if (cur->zero_node==NULL && cur->zero_val==NULL)
          {
            cur->zero_node=a_node;
            a_node=NULL;
            cur = cur->zero_node;
          }
          else if (cur->one_node==NULL && cur->one_val==NULL)
          {
            cur->one_node=a_node;
            a_node=NULL;
            cur = cur->one_node;
          }
          else
          {
            delete a_node;
            a_node=NULL;
            return NULL;
          }
        }
      }
      else
      {
        // while (1)
        // {
          recur(*root, cur);
          if (cur->zero_node==NULL && cur->zero_val==NULL)
          {
            cur->zero_val=a_leaf;
            // cout << (char)(a_leaf->val[0]) << endl;
            // break;
          }
          else if (cur->one_node==NULL && cur->one_val==NULL)
          {
            cur->one_val=a_leaf;
            // cout << (char)(a_leaf->val[0]) << endl;
            // break;
          }
          else
          {
            del_bin_t(*root);
            delete root;
            root=NULL;
            delete a_leaf;
            a_leaf=NULL;
            return NULL;
          }
          // else 
          // {
          // recur(*root, cur);
          // }
        // }
      }
    }
    end_bin_tree = is_end_tree(root);
  }
  return root;
}

bool readChunkFirstBit(ifstream & f, int& offset)
{
  char a;
  if (f.fail()) return false;
  f.get(a);
  u_int8_t a_a {(u_int8_t)a};
  u_int8_t a_b {0};
  u_int8_t r {0};
  if (offset!=0)
  {
    a_a = a_a << offset;
    a_b = (u_int8_t)f.peek();
    a_b = a_b >> (8-offset);
    r = (a_a | a_b);
  }
  else
  {
    r = a_a;
  }
  int i {0};
  bool result = extract_bit(i, r);
  offset=offset+1;
  if ((offset%8)==0)
  {
    f.get(a);
    offset=(offset%8);
  }
  f.unget();
  return result;
}

unsigned int pow(int base, int exponent)
{
  unsigned int result {1};
  while (exponent != 0) 
  {
    result *= base;
    --exponent;
  }
  return result;
}

unsigned int num_final_chars(ifstream& f, int& offset)
{
  vector<int>code;
  char a;
  if (f.fail()) return INT_MAX;
  f.get(a);
  u_int8_t a_a {(u_int8_t)a};
  u_int8_t a_b {0};
  u_int8_t r {0};
  if (offset!=0)
  {
    a_a = a_a << offset;
    a_b = (u_int8_t)f.peek();
    a_b = a_b >> (8-offset);
    r = (a_a | a_b);
  }
  else
  {
    r = a_a;
  }
  int i {0};
  for (int j=0; j<12; j++)
  {
    code.push_back((int)extract_bit(i, r));
    i=(i+1)%8;
    offset=offset+1;
    if ((offset%8)==0)
    {
      if (f.fail()) return INT_MAX;
      f.get(a);
      r=(u_int8_t)a;
      i=0;
      offset=(offset%8);
    }
  } 
  f.unget();
  unsigned int res{0};
  for (unsigned int j=0; j<code.size(); j++)
  {
    if (code.at(j)==1)
    {
      res += pow(2,(11-j));
    }
  }
  return res;
}

bool readCompChar(ifstream& f, int& offset, vector<u_int8_t>& out_str, node**& bin_t)
{
  node* cur=*bin_t;
  char a;
  if (f.fail()) 
  {
    return false;
  }
  f.get(a);
  u_int8_t a_a {(u_int8_t)a};
  u_int8_t a_b {0};
  u_int8_t r {0};
  if (offset!=0)
  {
    a_a = a_a << offset;
    a_b = (u_int8_t)f.peek();
    a_b = a_b >> (8-offset);
    r = (a_a | a_b);
  }
  else
  {
    r = a_a;
  }
  bool isLeaf=false;
  int i {0};
  bool dir;
  while (isLeaf==false)
  {
    dir = extract_bit(i, r);
    i=(i+1)%8;
    offset=offset+1;
    if ((offset%8)==0)
    {
      if (f.fail()) 
      {
        return false;
      }
      f.get(a);
      r=(u_int8_t)a;
      i=0;
      offset=(offset%8);
    }
    if (dir)
    {
      if (cur->one_node)
      {
        cur=cur->one_node;
      }
      else if (cur->one_val)
      {
        isLeaf=true;
        for (int j=0; j<cur->one_val->num_bytes; j++)
        {
          out_str.push_back(cur->one_val->val[j]);
        }
      }
      else 
      {
        return false;
      }
    }
    else
    {
      if (cur->zero_node)
      {
        cur=cur->zero_node;
      }
      else if (cur->zero_val)
      {
        isLeaf=true;
        for (int j=0; j<cur->zero_val->num_bytes; j++)
        {
          out_str.push_back(cur->zero_val->val[j]);
        }
      }
      else 
      {
        return false;
      }
    }
  }
  f.unget();
  return true;
}
bool readFinalChars(ifstream& f, int& offset, vector<u_int8_t>& out_str, node**& bin_t)
{
  unsigned int num_chars = num_final_chars(f, offset);
  if (num_chars==INT_MAX) return false;
  for (unsigned int i=0; i<num_chars; i++)
  {
    // if (i==1401)
    // {
    //   cout << "1401" <<endl;
    // }
    if (readCompChar(f, offset, out_str, bin_t)==false) 
    return false;
  }
  return true;
}

bool read4096chars(ifstream& f, int& offset, vector<u_int8_t>& out_str, node**& bin_t)
{
  for (int i=0; i<4096; i++)
  {
    if (readCompChar(f, offset, out_str, bin_t)==false) return false;
  }
  return true;
}

bool send_to_out(vector<u_int8_t>& out_str, const char * outFileName, int i)
{
  if (i==0)
  {
    ofstream wf(outFileName, ios::out | ios::binary);
    if (wf.fail()) return false;
    for(unsigned int i = 0; i < out_str.size(); i++)
    {
      wf.write(((char*)&out_str[i]), 1);
    }
    wf.close();
    if(!wf.good()) 
    {
      return false;
    }
  }
  else
  {
    ofstream wf(outFileName, ios::out | ios::binary | ios::app);
    if (wf.fail()) return false;
    for(unsigned int i = 0; i < out_str.size(); i++)
    {
      wf.write(((char*)&out_str[i]), 1);
    }
    wf.close();
    if(!wf.good()) 
    {
      return false;
    }
  }
  return true;
}

bool decode_chunk(ifstream & f, int& offset, node**& bin_t, const char * outFileName)
{
  vector<u_int8_t>out_str;
  int i{0};
  while (readChunkFirstBit(f, offset)==true)
  {
    out_str.clear();
    if (read4096chars(f, offset,out_str, bin_t)==false)return false;
    if (send_to_out(out_str, outFileName, i)==false )return false;
    i++;
  }
  out_str.clear();
  if (readFinalChars(f, offset, out_str, bin_t)==false)return false;
  if (send_to_out(out_str, outFileName, i)==false)return false;
  return true;
}


bool decompressFile ( const char * inFileName, const char * outFileName )
{
  ifstream f (inFileName, ios::binary | ios::in);
  if (f.fail()) return false;
  int offset {0};
  node** bin_t = create_binary_tree(f, offset);
  if (bin_t==NULL) return false;
  if (decode_chunk(f, offset, bin_t, outFileName)==false)
  {
    del_bin_t(*bin_t);
    delete bin_t;
    bin_t=NULL;
    return false;
  }
  del_bin_t(*bin_t);
  // if (*bin_t)
  // {
  //   delete * bin_t;
  //   *bin_t=NULL;
  // }
  delete bin_t;
  bin_t=NULL;
  return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
  // keep this dummy implementation (no bonus) or implement the compression (bonus)
  return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
  // todo
  return false;
}

int main ( void )
{
  assert ( decompressFile ( "tests/test0.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/test0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test1.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/test1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test2.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/test2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test3.huf", "tempfile" ) );
  // assert ( identicalFiles ( "tests/test3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/test4.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/test4.orig", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test5.huf", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test6.huf", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test7.huf", "tempfile" ) );

  assert ( ! decompressFile ( "tests/test8.huf", "tempfile" ) );

  assert ( decompressFile ( "tests/extra0.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra0.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra1.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra1.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra2.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra2.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra3.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra3.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra4.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra4.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra5.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra5.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra6.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra6.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra7.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra7.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra8.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra8.orig", "tempfile" ) );

  assert ( decompressFile ( "tests/extra9.huf", "tempfile" ) );
  // // assert ( identicalFiles ( "tests/extra9.orig", "tempfile" ) );

  return 0;
}
#endif /* __PROGTEST__ */
