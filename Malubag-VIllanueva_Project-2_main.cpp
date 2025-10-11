// Malubag, Villanueva
// 4 BS Computer Engineering

// ENGG 123.01
// Project 2: Interactive Basic RISC-V Simulator

#include <iostream>
#include <algorithm>
#include <bitset>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdint>

using namespace std;

bool isValidHex(const string &s, int length)
{
  if((int)s.length() != length) return false;
  for(int i=0; i<s.length(); i++)
  {
    char c = s[i];
    if(!isxdigit(c)) return false;
  }
  return true;
}

void loadData(const string &address, const string &filename, uint8_t * &data_memory)
{
  unsigned long long addr = stoull(address, nullptr, 16);
  
  ifstream file(filename);
  if(!file.is_open())
  {
    cout << "\nERROR: " << filename << " not found." << endl;
    return;
  }

  string line;  
  while(getline(file, line))
  {
    stringstream ss(line);
    string hexStr;

    // TO ADD: console feedback if condition is true
    if(!(ss >> hexStr)) break;
    if(hexStr.empty()) break;
    if(!isValidHex(hexStr, 16)) break;

    unsigned long long value = stoull(hexStr, nullptr, 16);
    for(int i=0; i<8; i++)
    {
      data_memory[addr + i] = (value >> (i * 8)) & 0xFF;
      cout << hex << int(data_memory[addr + i]) << " ";
    }
    addr += 8;
  }

  file.close();
}

void loadCode(const string &address, const string &filename, uint8_t * &instruction_memory)
{
  unsigned long long addr = stoull(address, nullptr, 16);
  
  ifstream file(filename);
  if(!file.is_open())
  {
    cout << "\nERROR: " << filename << " not found." << endl;
    return;
  }

  string line;  
  while(getline(file, line))
  {
    stringstream ss(line);
    string hexStr;

    // TO ADD: console feedback if condition is true
    if(!(ss >> hexStr)) break;
    if(hexStr.empty()) break;
    if(!isValidHex(hexStr, 8)) break;

    unsigned long long value = stoull(hexStr, nullptr, 16);
    for(int i=0; i<4; i++)
    {
      instruction_memory[addr + i] = (value >> (i * 8)) & 0xFF;
      cout << hex << int(instruction_memory[addr + i]) << " ";
    }
    addr += 4;
  }

  file.close();
}

void showData(string &address, int N)
{
  
}

void showCode(string &address, int N)
{

}

// need to update red and mem
void parseInstruction(unsigned int instruction, long long * &reg, uint8_t * &mem)
{
    cout << "\nInstruction: " << bitset<32>(instruction) << "\n\n";

    unsigned int opcode =  instruction & 0x7F;
    unsigned int rd     = (instruction >> 7)  & 0x1F;

    // R Format
    unsigned int funct3 = (instruction >> 12) & 0x07;
    unsigned int rs1    = (instruction >> 15) & 0x1F;
    unsigned int rs2    = (instruction >> 20) & 0x1F;
    unsigned int funct7 = (instruction >> 25) & 0x7F;

    // I Format
    int immediate = (instruction >> 20) & 0xFFF;
    if (immediate & 0x800) immediate |= 0xFFFFF000;

    // S Format
    int imm_s = ((instruction >> 7) & 0x1F) | 
                (((instruction >> 25) & 0x7F) << 5);
    if (imm_s & 0x800) imm_s |= 0xFFFFF000;

    switch (opcode) {
      case 0b0110011: // R-type ADD/SUB
        if (funct3 == 0 && funct7 == 0x00) 
        { // ADD
          if (rd == 0) 
          {
            cout << "ERROR: Cannot write to x0 (rd = 0)." 
                  << endl;
          } 
          else 
          {
            cout << "add x" << rd << ", x" << rs1 << ", x" 
                  << rs2 << endl;
          }
        }
        if (funct3 == 0 && funct7 == 0x20) 
        { // SUB
          if (rd == 0) 
          {
            cout << "ERROR: Cannot write to x0 (rd = 0)." << endl;
          } 
          else 
          {
            cout << "sub x" << rd << ", x" << rs1 << ", x" << rs2 
                  << endl;
          }
        }
        break;

      case 0b0010011: // I-type ADDI
        if (funct3 == 0) 
        {
          if (rd == 0) 
          {
            cout << "ERROR: Cannot write to x0 (rd = 0)." << endl;
          } 
          else 
          {
            cout << "addi x" << rd << ", x" << rs1 << ", " 
                  << immediate << endl;
          }
        }
        break;

      case 0b0000011: // I-type LD
        if (funct3 == 3) 
        {
          if (rd == 0) 
          {
            cout << "ERROR: Cannot write to x0 (rd = 0)." << endl;
          } 
          else 
          {
            cout << "ld x" << rd << ", " << immediate 
                  << "(x" << rs1 << ")" << endl;
          }
        }
        break;

      case 0b0100011: // S-type SD
        if (funct3 == 3) 
        {
          if (rs2 == 0) 
          {
            cout << "ERROR: Cannot store from x0 (rs2 = 0)." 
                  << endl; 
          } 
          else 
          {
            cout << "sd x" << rs2 << ", " << imm_s 
                  << "(x" << rs1 << ")" << endl;
          }
        }
        break;

      default:
        cout << "Unsupported instruction.\n"
              << "opcode = "   << opcode
              << "\tfunct3 = " << funct3
              << "\tfunct7 = " << funct7 << endl;
    }
}

int main()
{
  cout << "\nInteractive Basic RISC-V Simulator"
       << "\n(Type \"HELP\" to display all commands.)"
       << "\n(Type \"EXIT\" to terminate the program.)" << endl;

  stringstream ss;
  string userInput = "",
         command = "", 
         address = "", 
         filename = "";
  int N = 0;
  ifstream file;

  // 32 64-bit registers
  long long *registers = new long long [32];
  for(int i=0; i<32; i++) registers[i] = 0;

  // memory
  const int memory_size = 1024 * 64; // 64 KB
  uint8_t *data_memory = new uint8_t [memory_size]; 
  uint8_t *instruction_memory = new uint8_t [memory_size];
  
  while(true)
  {
    ss.clear();
    cout << "\nInput instructions here:\n> ";
    getline(cin, userInput);
    transform(userInput.begin(), userInput.end(), userInput.begin(), 
              [](unsigned char c) {return toupper(c); });

    ss.str(userInput);
    ss >> command;

    if(command == "EXIT")
    {
      ss.clear();
      file.close();
      delete [] registers;
      delete [] data_memory;
      delete [] instruction_memory;

      cout << "\nProgram has been terminated.\n" << endl;
      return 0;
    }

    else if(command == "HELP")
    {
      cout << "\n1. loaddata <address> <filename> - obtains 64-bit"
           << " data from <filename> and stores to <address>"
           << "\n2. showdata <address> <N>        - displays <N>"
           << " data starting from <address>"
           << "\n3. loadcode <address> <filename> - obtains 32-bit"
           << " instructions from <filename> and stores to <address>"
           << "\n4. showcode <address> <N>        - displays <N>"
           << " instructions starting from <address>"
           << "\n5. exec <address>                - simulates execution of"
           << " codes starting from <address>"
           << "\n6. help                          - displays this message"
           << "\n7. exit                          - terminates the program"
           << endl;
    }

    else if(command == "LOADDATA" || command == "LOADCODE")
    {
      ss >> address >> filename;
      if(address.empty() && filename.empty())
      {
        cout << "\nERROR: Missing arguments. Type \"HELP\" to display all commands.\n";
        continue;
      }
      else
      {
        if(!isValidHex(address, 8))
        {
          cout << "\nERROR: Please input an 8-bit hex value for <address>." << endl;
          continue;
        }
        else
        {
          cout << "\nLoading " 
               << (command == "LOADDATA" ? "data" : "instructions") 
               << " from " << filename << " to address 0x" << address << endl;

          if(command == "LOADDATA") loadData(address, filename, data_memory);
          else if(command == "LOADCODE") loadCode(address, filename, instruction_memory);
        }
      }
    }

    else if(command == "SHOWDATA" || command == "SHOWCODE")
    {
      ss >> address >> N;
      if(address.empty() && N <= 0)
      {
        cout << "\nERROR: Missing arguments. Type \"HELP\" to display all commands.\n";
        continue;
      }
      else
      {
        if(!isValidHex(address,8))
        {
          cout << "\nERROR: Please input an 8-bit hex value for <address>." << endl;
          continue;
        }
        else
        {
          cout << "\nShowing " 
               << (command == "SHOWDATA" ? "data" : "instructions") 
               << " " << N << " from address 0x" << address << endl;

          if(command == "SHOWDATA") showData(address, N);
          else if(command == "SHOWCODE") showCode(address, N);
        }
      }      
    }

    else if(command == "EXEC")
    {
      cout << "exec";
    }

    else
    {
      cout << "\nERROR: Invalid command."
           << " Type HELP to display all commands.\n";
    }
  }
}
