// Malubag, Villanueva
// 4 BS Computer Engineering

// ENGG 123.01
// Project 2: Interactive Basic RISC-V Simulator

#include <iostream>
#include <algorithm>
#include <bitset>
#include <sstream>
#include <fstream>

using namespace std;

bool isValidHex(const string &s)
{
  if(s.length() != 8) return false;
  for(int i=0; i<s.length(); i++)
  {
    char c = s[i];
    if(!isxdigit(c)) return false;
  }
  return true;
}

int main()
{
  cout << "\nInteractive Basic RISC-V Simulator"
       << "\n(Type HELP to display all commands.)"
       << "\n(Type EXIT to terminate the program.)" << endl;
  
  string userInput = "";
  
  while(true)
  {
    cout << "\nInput instructions here:\n> ";
    getline(cin, userInput);
    transform(userInput.begin(), userInput.end(), userInput.begin(), 
              [](unsigned char c) {return toupper(c); });
    stringstream ss(userInput);
    string command;
    ss >> command;

    if(command == "EXIT")
    {
      cout << "\nProgram has been terminated.\n" << endl;
      return 0;
    }

    if(command == "HELP")
    {
      cout << "\n[commands]\n" << endl;
    }

    if(command == "LOADDATA")
    {
      cout << "loaddata";
    }

    if(command == "SHOWDATA")
    {
      cout << "showdata";
    }

    if(command == "LOADCODE")
    {
      cout << "loadcode";
    }

    if(command == "SHOWCODE")
    {
      cout << "showcode";
    }

    if(command == "EXEC")
    {
      cout << "exec";
    }



    // if(!isValidHex(userInput))
    // {
    //   cout << "ERROR: Please input an 8-bit hex value." << endl;
    // }
    // else
    // {
    //   unsigned long instruction = stoul(userInput, nullptr, 16);
    //   cout << "\nInstruction: " << bitset<32>(instruction) << "\n\n";

    //   unsigned int opcode =  instruction & 0x7F;
    //   unsigned int rd     = (instruction >> 7)  & 0x1F;

    //   // R Format
    //   unsigned int funct3 = (instruction >> 12) & 0x07;
    //   unsigned int rs1    = (instruction >> 15) & 0x1F;
    //   unsigned int rs2    = (instruction >> 20) & 0x1F;
    //   unsigned int funct7 = (instruction >> 25) & 0x7F;

    //   // I Format
    //   int immediate = (instruction >> 20) & 0xFFF;
    //   if (immediate & 0x800) immediate |= 0xFFFFF000;

    //   // S Format
    //   int imm_s = ((instruction >> 7) & 0x1F) | 
    //               (((instruction >> 25) & 0x7F) << 5);
    //   if (imm_s & 0x800) imm_s |= 0xFFFFF000;

    //   switch (opcode) {
    //     case 0b0110011: // R-type ADD/SUB
    //       if (funct3 == 0 && funct7 == 0x00) 
    //       { // ADD
    //         if (rd == 0) 
    //         {
    //           cout << "ERROR: Cannot write to x0 (rd = 0)." 
    //                << endl;
    //         } 
    //         else 
    //         {
    //           cout << "add x" << rd << ", x" << rs1 << ", x" 
    //                << rs2 << endl;
    //         }
    //       }
    //       if (funct3 == 0 && funct7 == 0x20) 
    //       { // SUB
    //         if (rd == 0) 
    //         {
    //           cout << "ERROR: Cannot write to x0 (rd = 0)." << endl;
    //         } 
    //         else 
    //         {
    //           cout << "sub x" << rd << ", x" << rs1 << ", x" << rs2 
    //                << endl;
    //         }
    //       }
    //       break;

    //     case 0b0010011: // I-type ADDI
    //       if (funct3 == 0) 
    //       {
    //         if (rd == 0) 
    //         {
    //           cout << "ERROR: Cannot write to x0 (rd = 0)." << endl;
    //         } 
    //         else 
    //         {
    //           cout << "addi x" << rd << ", x" << rs1 << ", " 
    //                << immediate << endl;
    //         }
    //       }
    //       break;

    //     case 0b0000011: // I-type LD
    //       if (funct3 == 3) 
    //       {
    //         if (rd == 0) 
    //         {
    //           cout << "ERROR: Cannot write to x0 (rd = 0)." << endl;
    //         } 
    //         else 
    //         {
    //           cout << "ld x" << rd << ", " << immediate 
    //                << "(x" << rs1 << ")" << endl;
    //         }
    //       }
    //       break;

    //     case 0b0100011: // S-type SD
    //       if (funct3 == 3) 
    //       {
    //         if (rs2 == 0) 
    //         {
    //           cout << "ERROR: Cannot store from x0 (rs2 = 0)." 
    //                << endl; 
    //         } 
    //         else 
    //         {
    //           cout << "sd x" << rs2 << ", " << imm_s 
    //                << "(x" << rs1 << ")" << endl;
    //         }
    //       }
    //       break;

    //     default:
    //       cout << "Unsupported instruction.\n"
    //            << "opcode = "   << opcode
    //            << "\tfunct3 = " << funct3
    //            << "\tfunct7 = " << funct7 << endl;
    //   }
    // }



  }
}
