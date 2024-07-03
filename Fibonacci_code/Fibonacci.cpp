#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
// #include <bitset>
using namespace std;
#endif /* __PROGTEST__ */

class Queue
{
private:
  unsigned char stream[64];
  size_t m_capacity;
  size_t m_size;
  int m_start;
  int m_back;
public:
  Queue(/* args */);
  // ~Queue();
  bool push_back(unsigned char);
  bool dequeue();
  size_t size() const;
  bool empty() const;
  unsigned char& at(size_t);
  unsigned char& front();
  void print();
};

Queue::Queue(){
  this->m_capacity=64;
  this->m_size=0;
  this->m_start=0;
  this->m_back=-1;
}

bool Queue::push_back(unsigned char inChar){
  if (this->m_size>=m_capacity){
    return false;
  } 
  else {
    m_back++;
    m_back = m_back%m_capacity;
    this->stream[m_back] = inChar;
    this->m_size++;
    return true;
  }
}

bool Queue::dequeue(){
  if(this->m_size==0){
    return false;
  } else {
    this->m_start++;
    this->m_start = this->m_start%m_capacity;
    m_size--;
    return true;
  }
}

size_t Queue::size()const {
  return this->m_size;
}

bool Queue::empty() const{
  if (this->size()==0){
    return true;
  } else {
    return false;
  }
}

unsigned char& Queue::at(size_t n){
  if(n<this->size()){
    return this->stream[(n+m_start)%m_capacity];
  } else {
    try {
      throw (n);
    } catch (size_t pos){
      cout << n << " out_of_range" << endl;
    }
    return this->stream[(n+m_start)%m_capacity];
  }
}

unsigned char& Queue::front(){
  return stream[m_start] ;
}

void Queue::print(){
  for(size_t i=0; i<this->size(); i++){
    cout << this->at(i);
  }
}

int numberBytesToRead(uint8_t d){
  uint8_t oneByte = 0b00000000;
  uint8_t twoBytes = 0b11000000;
  uint8_t threeBytes = 0b11100000;
  uint8_t fourBytes = 0b11110000;
  uint8_t invalidBytes0 = 0b11111000;
  if(((d & invalidBytes0) == invalidBytes0)){
    return -1;
  }else if ((d & fourBytes) == fourBytes){
    return 4;
  }else if ((d & threeBytes)== threeBytes){
    return 3;
  }else if ((d & twoBytes) == twoBytes){
    return 2;
  }else if((d & 0b10000000) == oneByte){
    return 1;
  }else{
    return -2;
  }
}

bool trailingByteValid(uint8_t d){
  uint8_t valid = 0b10000000;
  if ((d & 0b11000000) == valid){
    return true;
  }else{
    return false;
  }
}

void arrayFibLessOrEqual(long long int n, vector<long long int>& fibArr){
  if (n==1 || n==2){
    return;
  }
  int i=fibArr.size();
  long long int num = fibArr.back();
  while(num<=n){
    num = fibArr.at(i-1) + fibArr.at(i-2);
    i++;
    if (num<=n) fibArr.push_back(num);
  }
}

long long int findLargestLessOrEqual(long long int key, vector<long long int>& fibArr){
  int l = 0;
  int r = fibArr.size()-1;
  if (fibArr.at(r)<key) return fibArr.at(r);
  while(l < r){
    int m = (l+r)/2;
    if ( (l+r)%2!=0 ) m++;
    if (fibArr.at(m)>key){
      r = m-1;
    }
    else{
      l = m;
    }
  }
  return fibArr.at(l);
}

int findIndexOfGreatesFibLessOrEqual(long long int key, vector<long long int>& fibArr){
  int l = 0;
  int r = fibArr.size()-1;
  if (fibArr.at(r)<key) return r;
  while(l < r){
    int m = (l+r)/2;
    if ( (l+r)%2!=0 ) m++;
    if (fibArr.at(m)>key){
      r = m-1;
    }
    else{
      l = m;
    }
  }
  return l;
}

void convertToFibonacci(uint32_t byteRes, vector<char>& fib, vector<long long int>& fibArr){
  long long int number = (long long int)byteRes+1;
  arrayFibLessOrEqual(number, fibArr);
  int l = findIndexOfGreatesFibLessOrEqual(number, fibArr);
  int i=l;
  fib.resize(l+2);
  while(number!=0){
    fib.at(i) = '1';
    long long int greatestFibSmallerThanNumber = findLargestLessOrEqual(number, fibArr);
    number = number-greatestFibSmallerThanNumber;
    i--;
    while(i>=0 && fibArr.at(i)>number)
    {
      fib.at(i) = '0';
      i--;
    }
  }
  fib.at((l)+1)='1';
  // cout << endl;
}

void LSBToTheRight(vector<char>& outCharStream){
  char tmp;
  int size = outCharStream.size();
  for(int i=0, j=size-1; i<(size/2); i++){
    tmp=outCharStream.at(j);
    outCharStream.at(j)=outCharStream.at(i);
    outCharStream.at(i)=tmp;
    j--;
  }
}

uint8_t convertToBits(vector<char>& outCharStream){
  uint8_t outByte=0b00000000;
  uint8_t x = 0b10000000;
  char one ='1';
  for(auto i = outCharStream.begin(); i!=outCharStream.end(); i++){
    if( (*i) == one){
      outByte = outByte | x;
    }
    x = x >> 1;
  }
  return outByte;
}

bool output(Queue& stream, const char* outFile, ofstream& out_bin){
  int originalSize = stream.size();
  vector<char> outCharStream;
  if (originalSize>=8){
    outCharStream.resize(8);
    for(int i=0; i<8; i++){
      outCharStream.at(i)=(stream.front());
      stream.dequeue();
      // outCharStream.at(i) = stream.at(i);
    }
    // for(int i=8, j=0; i<originalSize; i++){
    //   stream.at(j) = stream.at(i);
    //   j++;
    // }
    // stream.resize(originalSize-8);
  } else {
    outCharStream.resize(8, '0');
    for(int i=0; i<originalSize; i++){
      outCharStream.at(i) = stream.front();
      stream.dequeue();
    }
    // stream.resize(0);
  }

  LSBToTheRight(outCharStream);
  uint8_t outByte;
  outByte = convertToBits(outCharStream);

  out_bin << outByte;
    // bitset<8>x(outByte);
    // cout << x << " " << flush;
  if (out_bin.good()){
    return true;
  } else {
    return false;
  }
}

void removeprefix(uint8_t& d, int bytesToRead){
  if (bytesToRead == 2){
    d = d & 0b00011111;
  } else if (bytesToRead == 3){
    d = d & 0b00001111;
  } else if (bytesToRead == 4){
    d = d & 0b00000111;
  } else return;
}

void removeprefix(uint8_t& d){
  d = d & 0b00111111;
}

bool               utf8ToFibonacci                         ( const char      * inFile, 
                                                             const char      * outFile )
{
  ifstream in_bin (inFile, ios::binary | ios::in );
  if (in_bin.good()==false) return false;
  // remove(outFile);
  ofstream out_bin (outFile, ios::binary | ios::out | ios_base::app);
  if (out_bin.good()==false) return false;
  if(!out_bin.is_open()) return false;
  if (in_bin.is_open()){
    // uint32_t byte1=0;
    // uint32_t byte2=0;
    // uint32_t byte3=0;
    // uint32_t byte4=0;
    uint32_t byteRes=0;
    uint8_t d;
    Queue stream;
    vector<long long int> fibArr;
    fibArr.push_back(1);
    fibArr.push_back(2);
    while(true){
      in_bin.read(reinterpret_cast<char*>(&d), sizeof d);
      if(in_bin.good()==false) return false;
      int bytesToRead = numberBytesToRead(d);
      if (bytesToRead < 1){
        return false;
      } else {
        if (bytesToRead >=2){
          removeprefix(d, bytesToRead);
        }
        byteRes = d;
        for (int i=1; i<bytesToRead; i++){
          in_bin.read(reinterpret_cast<char*>(&d), sizeof d);
          if (in_bin.good() &&  trailingByteValid(d)){
            removeprefix(d);
            byteRes = byteRes << 6;
            byteRes = byteRes | d;
          }
          else{
            return false;
          }
        }
        if(byteRes > 0b100001111111111111111) return false;
        vector<char> fib;
        convertToFibonacci(byteRes, fib, fibArr);
        for(auto i=fib.begin(); i!=fib.end(); i++){
          stream.push_back(*i);
        }
        while (stream.size()>=8){
          bool isWriteSuccess = output(stream, outFile, out_bin);
          if (!isWriteSuccess){
            return false;
          }
        }
        if (in_bin.peek() && in_bin.eof()){
          while (! stream.empty()){
            bool isWriteSuccess = output(stream, outFile, out_bin);
            if (!isWriteSuccess){
              return false;
            }
          }
          // cout << endl;
          return true;
        }
        
        // cout << endl;
        // todo change stream to (LSB to the right)
      } 
      // bitset<8>x(d);
      // bitset<32>y(byteRes);
      // cout << x << ", " << y << endl;
    }
    // cout << endl;
  } else {
    // cout << "failed to open " << inFile << endl;
    return false;
  }
  return true;
}

void placeIntoInStream(u_int8_t d, vector<char>& inStream){
  u_int8_t cntrl = 0b00000001;
  for(int i=7; i>=0; i--){
    if (d & cntrl){
      inStream.push_back('1');
    } else {
      inStream.push_back('0');
    }
    cntrl = cntrl << 1;
  }
  // cout << flush;
}

long long unsigned int fibToInt(vector<char>& fibNumber, vector<int>& fibArr){
  while(fibArr.size()<fibNumber.size()){
    fibArr.push_back(fibArr.at(fibArr.size()-2) + fibArr.at(fibArr.size()-1));
  }
  long long unsigned int intOut = 0;
  for(long unsigned int i=0; i<fibNumber.size(); i++){
    if(fibNumber.at(i)=='1'){
      intOut = intOut+fibArr.at(i);
    }
  }
  intOut--;
  // cout << flush;
  return intOut;
}

bool convertToUTF(long long unsigned int intOut, u_int8_t& byte1, u_int8_t& byte2, u_int8_t& byte3, u_int8_t& byte4){
  u_int8_t trailingByte = 0b10000000;
  u_int32_t cntrl = 1;
  if (intOut > 0x10ffff){
    return false;
  } else if (intOut >= 0x10000){
    byte1 = 0b11110000;
    byte2 = trailingByte;
    byte3 = trailingByte;
    byte4 = trailingByte;
    for (int i=0; i<6; i++){
      uint8_t tmp = intOut & cntrl;
      byte4 = byte4 | tmp;
      cntrl = cntrl <<1;
    }
    for (int i=0; i<6; i++){
      uint16_t tmp = intOut & cntrl;
      tmp = tmp >>6;
      byte3 = byte3 | tmp;
      cntrl = cntrl <<1;
    }
    for (int i=0; i<6; i++){
      uint32_t tmp = intOut & cntrl;
      tmp = tmp >>12;
      byte2 = byte2 | tmp;
      cntrl = cntrl <<1;
    }
    for (int i=0; i<3; i++){
      uint32_t tmp = intOut & cntrl;
      tmp = tmp >>18;
      byte1 = byte1 | tmp;
      cntrl = cntrl <<1;
    }
    // bitset<8>b1(byte1);
    // bitset<8>b2(byte2);
    // bitset<8>b3(byte3);
    // bitset<8>b4(byte4);
    // cout << b1 << " " << b2 << " "<< b3 << " " << b4 << " "<<  flush;

  } else if (intOut >= 0x800){
    byte1 = 0b11100000;
    byte2 = trailingByte;
    byte3 = trailingByte;
    for (int i=0; i<6; i++){
      uint8_t tmp = intOut & cntrl;
      byte3 = byte3 | tmp;
      cntrl = cntrl <<1;
    }
    for (int i=0; i<6; i++){
      uint16_t tmp = intOut & cntrl;
      tmp = tmp >>6;
      byte2 = byte2 | tmp;
      cntrl = cntrl <<1;
    }
    for (int i=0; i<4; i++){
      uint16_t tmp = intOut & cntrl;
      tmp = tmp >>12;
      byte1 = byte1 | tmp;
      cntrl = cntrl <<1;
    }
    // bitset<8>b1(byte1);
    // bitset<8>b2(byte2);
    // bitset<8>b3(byte3);
    // cout << b1 << " " << b2 << " "<< b3 << " " << flush;
  }else if (intOut >= 0x80){
    byte1 = 0b11000000;
    byte2 = trailingByte;
    for (int i=0; i<6; i++){
      uint8_t tmp = intOut & cntrl;
      byte2 = byte2 | tmp;
      cntrl = cntrl <<1;
    }
    for (int i=0; i<5; i++){
      uint16_t tmp = intOut & cntrl;
      tmp = tmp >>6;
      byte1 = byte1 | tmp;
      // bitset<8>tmpb1(byte1);
      // cout << "tmpb1: " << tmpb1 << endl;
      cntrl = cntrl <<1;
    }
    // bitset<8>b1(byte1);
    // bitset<8>b2(byte2);
    // cout << b1 << " " << b2 << " "<< flush;
  }else {
    byte1 = 0;
    for (int i=0; i<7; i++){
      uint8_t tmp = intOut & cntrl;
      byte1 = byte1 | tmp;
      cntrl = cntrl <<1;
    }
    // bitset<8>b1(byte1);
    // cout << b1 << " " << flush;
  }

  return true;
}

bool outputUTF(vector<char>& fibNumber, const char* outFile, vector<int>& fibArr, ofstream& out_bin){
  fibNumber.pop_back();
  long long unsigned int intOut = fibToInt(fibNumber, fibArr);
  u_int8_t byte1 =0;
  u_int8_t byte2 =0;
  u_int8_t byte3 =0;
  u_int8_t byte4 =0;
  bool validUTF = convertToUTF(intOut, byte1, byte2, byte3, byte4);
  if (validUTF==false){
    return false;
  }

    out_bin << byte1;    
    if (byte2){
      out_bin << byte2;
    }
    if (byte3){
      out_bin << byte3;
    }
    if (byte4){
      out_bin << byte4;
    }
  if(out_bin.good()){
    return true;
  } else {
    return false;
  }
}

bool checkEnd(vector<char>& inStream){
  for(auto i=inStream.begin(); i!=inStream.end(); i++){
    if(*i == '1'){
      return false;
    }
  }
  return true;
}
bool checkFileValidity(vector<char>& inStream){
  for(int i=inStream.size()-1; i>=0 ;i--){
    if(i!=0 && inStream.at(i)=='1' && inStream.at(i-1)=='1'){
      return true;
    }else if(i==0 && inStream.at(i)=='1'){
      return false;
    }
  }
  return false;
}
bool               fibonacciToUtf8                         ( const char      * inFile, 
                                                             const char      * outFile )
{
  ifstream in_bin (inFile, ios::binary | ios::in );
  if (in_bin.good()==false) return false;
  // remove(outFile);
  ofstream out_bin (outFile, ios::binary | ios_base::app | ios::out);
  if (out_bin.good()==false) return false;
  if(!out_bin.is_open()) return false;
  // remove(outFile);
  if (in_bin.is_open()){
    vector<char> inStream;
    u_int8_t d;
    vector<int> fibArr;
    fibArr.push_back(1);
    fibArr.push_back(2);
    while(true){
      if (in_bin.read(reinterpret_cast<char*>(&d), sizeof d)){
        if(in_bin.good()==false) return false;
        placeIntoInStream(d, inStream);
        if (in_bin.peek() && in_bin.eof()){
          if(checkFileValidity(inStream)==false){
            return false;
          }
        }
        vector<char> fibNumber;
        for(long unsigned int i=0; i<inStream.size(); i++){
          fibNumber.push_back(inStream.at(i));
          if(fibNumber.size()>30){
            return false;
          }
          if (i>0 && inStream.at(i-1)=='1' && inStream.at(i) =='1'){
            for (long unsigned int j=i+1, k=0; j<inStream.size(); j++){
              inStream.at(k) = inStream.at(j);
              k++;
            }
            inStream.resize(inStream.size()-fibNumber.size());
            bool outputSuccess = outputUTF(fibNumber, outFile, fibArr, out_bin);
            if (outputSuccess == false){
              return false;
            }
            fibNumber.clear();
            if (in_bin.eof()){
              if (in_bin && checkEnd(inStream)){
                // cout << endl;
                return true;
              }
            } else {
              i=-1;
            }
          }
        }
      } else {
        return false;
      }
    }
  } else {
    return false;
  }
}
  
#ifndef __PROGTEST__

bool               identicalFiles                          ( const char      * file1,
                                                             const char      * file2 )
{
  ifstream infile1 (file1, ios::binary | ios::in );
  ifstream infile2 (file2, ios::binary | ios::in );
  if (infile1.is_open() && infile2.is_open()){
    while(true){
      uint8_t d1;
      infile1.read(reinterpret_cast<char*>(&d1), sizeof d1);
      uint8_t d2;
      infile2.read(reinterpret_cast<char*>(&d2), sizeof d2);
      if ((infile1.eof() && (!infile2.eof())) || ((!infile1.eof()) && infile2.eof())){
        return false;
      }
      if (infile1.eof() && infile2.eof()){
        return true;
      }
      if(d1!=d2){
        return false;
      } 
    }
  }else{
    return false;
  }
  return true;
}

int main ( int argc, char * argv [] )
{
  // remove("output.fib");remove("output.utf8");
  utf8ToFibonacci ( "test0.utf8", "output.fib" );
  assert ( ! fibonacciToUtf8 ( "output.fib", "output.utf8" ) );
  assert ( fibonacciToUtf8 ( "output.fib", "output.utf8" )
              && identicalFiles ( "output.utf8", "test0.utf8" ) );remove("output.fib");remove("output.utf8");
  
  utf8ToFibonacci ( "test1.utf8", "output.fib" );
  assert ( fibonacciToUtf8 ( "output.fib", "output.utf8" )
              && identicalFiles ( "output.utf8", "test1.utf8" ) );remove("output.fib");remove("output.utf8");
  
  utf8ToFibonacci ( "test2.utf8", "output.fib" );
  assert ( fibonacciToUtf8 ( "output.fib", "output.utf8" )
              && identicalFiles ( "output.utf8", "test2.utf8" ) );remove("output.fib");remove("output.utf8");
  
  utf8ToFibonacci ( "test3.utf8", "output.fib" );
  assert ( fibonacciToUtf8 ( "output.fib", "output.utf8" )
              && identicalFiles ( "output.utf8", "test3.utf8" ) );remove("output.fib");remove("output.utf8");
  

  assert ( utf8ToFibonacci ( "example/src_0.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_0.fib" ) );remove("output.fib");remove("output.utf8");
  assert ( utf8ToFibonacci ( "example/src_1.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_1.fib" ) );remove("output.fib");remove("output.utf8");
  assert ( utf8ToFibonacci ( "example/src_2.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_2.fib" ) );remove("output.fib");remove("output.utf8");
  assert ( utf8ToFibonacci ( "example/src_3.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_3.fib" ) );remove("output.fib");remove("output.utf8");
  assert ( utf8ToFibonacci ( "example/src_4.utf8", "output.fib" )
           && identicalFiles ( "output.fib", "example/dst_4.fib" ) );remove("output.fib");remove("output.utf8");
  assert ( ! utf8ToFibonacci ( "example/src_5.utf8", "output.fib" ) );remove("output.fib");remove("output.utf8");
  assert ( fibonacciToUtf8 ( "example/src_6.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_6.utf8" ) );remove("output.fib");remove("output.utf8");
  assert ( fibonacciToUtf8 ( "example/src_7.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_7.utf8" ) );remove("output.fib");remove("output.utf8");
  assert ( fibonacciToUtf8 ( "example/src_8.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_8.utf8" ) );remove("output.fib");remove("output.utf8");
  assert ( fibonacciToUtf8 ( "example/src_9.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_9.utf8" ) );remove("output.fib");remove("output.utf8");
  assert ( fibonacciToUtf8 ( "example/src_10.fib", "output.utf8" )
           && identicalFiles ( "output.utf8", "example/dst_10.utf8" ) );remove("output.fib");remove("output.utf8");
  assert ( ! fibonacciToUtf8 ( "example/src_11.fib", "output.utf8" ) );remove("output.fib");remove("output.utf8");
 
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
