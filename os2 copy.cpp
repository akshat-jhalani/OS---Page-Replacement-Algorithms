#include <iostream>
#include <fstream>
#include <map>
#include <iterator>
#include <queue>
#include <vector>

using namespace std;

// class Pairs
// {
//     // private:
//         // unsigned key;
//         // char value;

//     public:
//       unsigned key;
//       char value;

//       unsigned getKey(){
//           return this->key;
//       }
//       char getValue(){
//           return this->value;
//       }

//       void setKey(unsigned keyPassed){
//         this->key = keyPassed;
//       }

//       void setValue(char valuePassed){
//         this->value = valuePassed;
//       }

//       void setKV(unsigned addr, char rw){
//         this->setKey(addr);
//         this->setValue(rw);
//       }

      

// };

// class NewMap
// {
//   private:
//     vector<Pairs> map;
//   public:
//     void insert(Pairs pair){
//       map.push_back(pair);
//     }
//     void remove(){
//       map.pop_back();
//     }

//     bool isWrite(){
//       char rw = this->map.back().getValue();
//       if(rw == 'W'){
//         return true;
//       }
//       else{
//         return false;
//       }
//     }
//     void removeLast(){
//       this->map.pop_back();
//     }
//     void printList(){
//         for(int i =0; i < this->map.size(); ++i){
//           cout << hex << this->map[i].getKey()<< ": "<< this->map[i].getValue()<< "\n";
//         }
//     }
//     vector<Pairs> getList(){
//       return this->map;
//     }
//     vector<Pairs>::iterator search(unsigned int addr){
//       vector<Pairs>::iterator it;
//       it = find(this->getList().begin(), this->getList().end(), addr)
//     }
// };



int main(int argc, char const *argv[]) {

  map<unsigned, char> memory;
  vector<unsigned> pageTable; 
  ifstream file;
  file.open("test.trace");
  unsigned address;
  char rw;
  // Pairs KVpair;
  // NewMap refList;
  int trace = 0;
  int totalFrames = 64;
  int totalRead = 0;
  int totalWrite = 0;
  while(file >> hex >> address >> rw){
    // Adjusting the address according to the offset
    address = address/4096;
    trace ++;
    // KVpair.setKV(address,value);
    // refList.insert(KVpair);

    // The page is found in the memory
    if(memory.find(address) != memory.end()){
      if(memory[address] == 'R'){
        memory[address] = rw;
      }
    }

    //When the page cannot be found, two cases arise: 1. The memory is full, or the memory has space
    else{
      
      //case 1: Memory has space
      if(memory.size() >= totalFrames){
        pageTable.insert(pageTable.begin(), address);
        memory[address] = rw;
      }
      //case 2: Memory is full and we need to replace a page
      else{
        //checking if the most recent memory access is a write/dirty
        unsigned mostRecent = pageTable.front();
        unsigned firstOut = pageTable.back();
        if(memory[mostRecent]=='W'){
          //incrementing write count
          totalWrite ++;
        }
        else{
          //removing from the page table
          memory.erase(firstOut);
          pageTable.pop_back();
          pageTable.insert(pageTable.begin(), address);
        }
      }
      // No matter, what case it is, we need to access the hard disk and therefore the read count goes up
      totalRead++;
    }

  cout << "Frames: " << totalFrames << "\n";
  cout << "Trace Count: " << trace << "\n";
  cout << "Total Reads: " << totalRead << "\n";
  cout << "Total Writes: "<< totalWrite << "\n";
  }
  
  // refList.printList();

  // vector<unsigned int> pageTable;
  // NewMap memory;

  // for (int i = 0; i < refList.getList().size(); i++){
  //   int temp = refList.getList()[i].getKey();
  //   int temp2 = (int)temp/4096;
  //   cout <<"\n" << temp << ": "<< temp2;
  //   // pageTable.push_back(temp/4096);
  // }

  //case 1: Address found

  // for(int i =0; i < pageTable.size();i++){
  //   cout << pageTable[i] << "\n";
  // }















  // itr = referenceList.begin();
  // while(itr!= referenceList.end()){
  //   cout << itr->first + ": " + itr->second;
  //   cout << "\n";
  //   ++itr;
  // }
  
//   queue <unsigned, page_number> page_table;

//   for (itr = symbolTable.begin(); itr != symbolTable.end(); it++)
//   {
//     if itr->first
//     it->first    // string (key)
//     it->second   // string's value 
// }

// so the references is just like a list of commands
// we still need to have memory and a page table

// vector <unsigned> page_table;
  // fclose(file);
}

// create a memory map that maps the page table exactly, then do the same operations from there as well


// void push(vector pageTable, std::map mem_lib, const int pageNumber, int &reads, int &writes, int &fault){
//     map<unsigned, char>::iterator itr; // iterator for reference commands
//     map<unsigned, char> memory; // the memory map
//     map<unsigned, char>::iterator iter = mem_lib.begin(); //iterator for the memory map
//     //the page table is a vector storing only the addess of the commands


//     for(itr->first = mem_lib.begin(); itr != mem_lib.end(); itr++){ //going through all the commands
//         // if we found the element in the page table then it is a hit
//         if (std::binary_search(pageTable.begin(), pageTable.end(), itr->first){
//             cout << "Hit \n";
//             return; 
//         }
//         // if the element is not in the table, we need to check if there is room

//         //if there is space -> just add it to the front, this should result in a memory load and we need to ++ the reads
//         if(pageTable.size()< pageNumber){ //incorrect logic here...!
//             pageTable.insert(pageTable.begin(), itr->first);
//             memory.insert(std::pair<unsigned, int>(itr->first, itr->second));
//             read++;
//             fault++;
//             return; 
//         }
//         else{// if it is not in memory and the space is full                           //need to determine if the last access was a write then the page is dirty, it must be saved, i.e. write ++
//             auto it = memory.end();
//             it--;
//             pageTable.pop_back();
//             memory.erase(it); // deleting the last element of the memory map
//             pageTable.insert(pageTable.begin(), itr->first);
            
//             //some way to check if there were any previous write operations ??
//             read++; 
//             fault++;
//         }
//     }
// }
