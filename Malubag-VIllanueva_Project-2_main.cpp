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
#include <iomanip>

using namespace std;

bool isValidHex(const string &s, int length)
{
  if((int)s.length() > length) return false;
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

    if(!(ss >> hexStr)) 
    {
      cout << "ERROR: File contains invalid data." << endl;
      break;
    }
    if(hexStr.empty()) 
    {
      cout << "ERROR: File is empty." << endl;
      break;
    }
    if(!isValidHex(hexStr, 16)) 
    {
      cout << "ERROR: File contains an invalid hex string length." << endl;
      break;
    }

    unsigned long long value = stoull(hexStr, nullptr, 16);
    for(int i=0; i<8; i++)
    {
      data_memory[addr + i] = (value >> (i * 8)) & 0xFF;
    }
    addr += 8;
  }

  file.close();

  cout << "\nData loaded successfully from " << filename << " to address " << hex << uppercase << "0x" << setw(8) << setfill('0') << address << endl;
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

    if(!(ss >> hexStr)) 
    {
      cout << "ERROR: File contains invalid data." << endl;
      break;
    }
    if(hexStr.empty()) 
    {
      cout << "ERROR: File is empty." << endl;
      break;
    }
    if(!isValidHex(hexStr, 16)) 
    {
      cout << "ERROR: File contains an invalid hex string length." << endl;
      break;
    }

    unsigned long long value = stoull(hexStr, nullptr, 16);
    for(int i=0; i<4; i++)
    {
      instruction_memory[addr + i] = (value >> (i * 8)) & 0xFF;
    }
    addr += 4;
  }

  file.close();

  cout << "\nInstructions loaded successfully from " << filename << " to address " << hex << uppercase << "0x" << setw(8) << setfill('0') << address << endl;
}

void showData(string &address, int N, uint8_t * &data_memory)
{
  unsigned long long addr = stoull(address, nullptr, 16);
  for(int i=0; i<N; i++)
  {
    uint64_t val = 0;
    for(int j=0; j<8; j++)
    {
      val |= (uint64_t)data_memory[addr + j] << (j * 8);
    }
    
    cout << hex << uppercase << "0x" << setw(8) << setfill('0') << addr << "\t";
    cout << hex << uppercase << setw(16) << setfill('0') << val << dec << setfill(' ') << endl;

    addr += 8;
  }
}

void showCode(string &address, int N, uint8_t * &instruction_memory)
{
  unsigned long long addr = stoull(address, nullptr, 16);
  for(int i=0; i<N; i++)
  {
    uint64_t val = 0;
    for(int j=0; j<4; j++)
    {
      val |= (uint32_t)instruction_memory[addr + j] << (j * 8);
    }
    
    cout << hex << uppercase << "0x" << setw(8) << setfill('0') << addr << "\t";
    cout << hex << uppercase << setw(8) << setfill('0') << val << dec << setfill(' ') << endl;

    addr += 4;
  }
}

void execInstruction(unsigned int instruction, long long * &reg, uint8_t * &mem)
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

  vector<unsigned int> usedRegs;

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
          reg[rd] = reg[rs1] + reg[rs2];
          cout << "add x" << rd << ", x" << rs1 << ", x" 
                << rs2 << endl;
          usedRegs = {rd, rs1, rs2};
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
          reg[rd] = reg[rs1] - reg[rs2];
          cout << "sub x" << rd << ", x" << rs1 << ", x" << rs2 
                << endl;
          usedRegs = {rd, rs1, rs2};
        }
      }
      break;

    case 0b0010011: // I-type ADDI
      if (funct3 == 0) 
      { //ADDI
        if (rd == 0) 
        {
          cout << "ERROR: Cannot write to x0 (rd = 0)." << endl;
        } 
        else 
        {
          reg[rd] = reg[rs1] + immediate;
          cout << "addi x" << rd << ", x" << rs1 << ", " 
                << immediate << endl;
          usedRegs = {rd, rs1};
        }
      }
      // need to add SLLI instruction
      if (funct3 == 1 && funct7 == 0)
      { // SLLI
        
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
          uint64_t mem_addr = reg[rs1] + immediate;
          uint64_t val = 0;
          for (int i = 0; i < 8; i++) 
          {
            val |= ((uint64_t)mem[mem_addr + i]) << (i * 8);
          }
          reg[rd] = val;
          cout << "ld x" << rd << ", " << immediate 
                << "(x" << rs1 << ")" << endl;
          usedRegs = {rd, rs1};
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
          uint64_t mem_addr = reg[rs1] + imm_s;
          uint64_t val = reg[rs2];
          for (int i = 0; i < 8; i++) 
          {
            mem[mem_addr + i] = (val >> (i * 8)) & 0xFF;
          }
          cout << "sd x" << rs2 << ", " << imm_s 
                << "(x" << rs1 << ")" << endl;
          usedRegs = {rs1, rs2};
        }
      }
      break;

      //case 0b0100011: // need to add BLT instruction
      //break;

    default:
      cout << "Unsupported instruction.\n"
            << "opcode = "   << opcode
            << "\tfunct3 = " << funct3
            << "\tfunct7 = " << funct7 << endl;
  }

  if (!usedRegs.empty()) 
  {
    cout << "\n[Register Dump]\n";
    for (int r : usedRegs) {
      if (r == 0) continue;
      cout << "x" << setw(2) << left << r
           << " = " << dec << reg[r]
           << "\t(0x" << hex << uppercase << reg[r] << ")\n";
    }
    cout << dec;
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
          cout << "\nShowing " << N << " "
               << (command == "SHOWDATA" ? "data" : "instructions") 
               << " from address " << hex << uppercase << "0x" << setw(8) << setfill('0') << address << endl;

          if(command == "SHOWDATA") showData(address, N, data_memory);
          else if(command == "SHOWCODE") showCode(address, N, instruction_memory);
        }
      }      
    }

    else if(command == "EXEC")
    {
      ss >> address;
      if (address.empty()) 
      {
        cout << "\nERROR: Missing <address> argument. Usage: EXEC <address>\n";
        continue;
      }
      if (!isValidHex(address, 8)) 
      {
        cout << "\nERROR: Invalid address format.\n";
        continue;
      }

      unsigned long long addr = stoull(address, nullptr, 16);
      unsigned long long PC = addr;

      cout << "\n[Starting execution at 0x" << hex << uppercase << setw(8)
          << setfill('0') << PC << "]\n";

      while (true)
      {
        if (PC + 4 >= memory_size)
        {
          cout << "\n[ERROR] Program counter out of bounds. Halting execution.\n";
          break;
        }

        unsigned int instr = 0;
        for (int j = 0; j < 4; j++)
        {
          instr |= ((unsigned int)instruction_memory[PC + j]) << (j * 8);
        }
        if (instr == 0x00000000)
        {
          break;
        }

        cout << "\nExecuting instruction at 0x" << hex << uppercase << setw(8)
             << setfill('0') << PC << "...\n";

        execInstruction(instr, registers, data_memory);

        PC += 4;
      }

      cout << "\nExecution finished\n";
      }

    else
    {
      cout << "\nERROR: Invalid command."
           << " Type HELP to display all commands.\n";
    }
  }
}
