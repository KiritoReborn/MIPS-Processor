#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;

const int BASE = 0x00400000;  // Simulated code base

vector<string> memory(1000, "00000000");
vector<int> registers(32, 0);
int pc = BASE;              // Initialize PC to code base
int rs_num, rt_num, rd_num, shamt_num, funct_num, imm_num, address_num, opcode_num;
string instruction, rs, rt, rd, shamt, funct, imm, address, type, opcode, aluop, aluin, var1;
int regdst, memread, memtoreg, memwrt, alusrc, regwr, j;
long long alures, HI, LO;
int max_pc;  // max PC = BASE + (number of instructions * 4)

// Updated opcodes map (as produced by MARS)
map<int, string> opcodes = {
    {0b001001, "I"},  // ADDI (for li)
    {0b001000, "I"},  // alternative ADDI
    {0b000000, "R"},  // R-type
    {0b000100, "I"},  // BEQ
    {0b000101, "I"},  // BNE
    {0b011100, "R"},  // MUL instruction (MIPS32 mul)
    {0b000010, "J"},  // J
    {0b101011, "I"},  // SW
    {0b100011, "I"},  // LW
    {0b001010, "I"}   // SLTI
};

int convert_num_to_binary(int num) {
    if (num == 0) return 0;
    int binary_num = 0;
    int i = 1;
    while (num > 0) {
        binary_num += (num % 2) * i;
        num /= 2;
        i *= 10;
    }
    return binary_num;
}

int convert_binary_to_num(int binary_num) {
    int num = 0;
    int i = 0;
    while (binary_num > 0) {
        num += (binary_num % 10) * pow(2, i);
        binary_num /= 10;
        i++;
    }
    return num;
}

void Fetch() {
    // Convert the simulated PC to a memory index by subtracting BASE.
    int index = pc - BASE;  
    instruction = memory[index] + memory[index+1] + memory[index+2] + memory[index+3];
    pc = pc + 4;
    cout << "Fetch----->" << endl;
    cout << "PC: " << pc << endl;
    cout << "Instruction: " << instruction << endl;
}

void Decode() {
    if (instruction.length() != 32) {
        cout << "Invalid instruction length. Must be 32 bits." << endl;
        return;
    }
    opcode = instruction.substr(0,6);
    opcode_num = bitset<6>(opcode).to_ulong();
    type = opcodes[opcode_num];
    if (type == "R") {
        rs = instruction.substr(6,5);
        rt = instruction.substr(11,5);
        rd = instruction.substr(16,5);
        shamt = instruction.substr(21,5);
        funct = instruction.substr(26,6);
        rs_num = bitset<5>(rs).to_ulong();
        rt_num = bitset<5>(rt).to_ulong();
        rd_num = bitset<5>(rd).to_ulong();
        shamt_num = bitset<5>(shamt).to_ulong();
        funct_num = bitset<6>(funct).to_ulong();
        cout << "Type: " << type << endl;
        cout << "rs: " << rs_num << endl;
        cout << "rt: " << rt_num << endl;
        cout << "rd: " << rd_num << endl;
        cout << "shamt: " << shamt_num << endl;
        cout << "funct: " << funct << endl;
    } else if (type == "I") {
        rs = instruction.substr(6,5);
        rt = instruction.substr(11,5);
        imm = instruction.substr(16,16);
        rs_num = bitset<5>(rs).to_ulong();
        rt_num = bitset<5>(rt).to_ulong();
        imm_num = bitset<16>(imm).to_ulong();
        cout << "Type: " << type << endl;
        cout << "rs: " << rs_num << endl;
        cout << "rt: " << rt_num << endl;
        cout << "imm: " << imm_num << endl;
    } else if (type == "J") {
        address = instruction.substr(6,26);
        address_num = bitset<26>(address).to_ulong();
        cout << "Type: " << type << endl;
        cout << "address: " << address_num << endl;
    } else {
        cout << "Invalid instruction" << endl;
    }
}

void alu_ctrl() {
    if (aluop == "00") {
        aluin = "010";  // addition
    } else if (aluop == "10" && funct == "100000") {
        aluin = "010";  // R-type add
    } else if (aluop == "10" && funct == "100010") {
        aluin = "011";  // R-type subtract
    } else if (aluop == "10" && funct == "000010") {
        aluin = "111";
    } else if (aluop == "01") {
        aluin = "100";  // branch comparison (not used directly)
    } else if (aluop == "11") {
        aluin = "101";
    } else if (aluop == "100") {
        aluin = "110";  // MFLO
    } else if (aluop == "001") {
        aluin = "SLT";  // For I-type SLT (used in Fibonacci)
    } else if (aluop == "SLT_R") {
        aluin = "SLT_R";  // For R-type SLT (used in Factorial)
    } else if (aluop == "MUL") {
        aluin = "MUL";  // Multiplication
    }
}

void ctrl_ckt() {
    if (opcode == "101011") {  // SW
        regdst = 2;
        memread = 0;
        memtoreg = 2;
        aluop = "00";
        memwrt = 1;
        alusrc = 1;
        regwr = 0;
        j = 0;
        cout << "Control: SW, ALUop = 00" << endl;
    } else if (opcode == "000000") {  // R-type
        regdst = 1;
        memread = 0;
        memtoreg = 1;
        memwrt = 0;
        alusrc = 0;
        regwr = 1;
        j = 0;
        // For Factorial, check for R-type SLT (funct "101010")
        if (funct == "101010") {
            aluop = "SLT_R";
            cout << "Control: SLT, ALUop = SLT_R" << endl;
        } else if (funct == "011000") {
            aluop = "11";
            cout << "Control: R-type, ALUop = 11" << endl;
        } else if (funct == "010010") {
            aluop = "100";
            cout << "Control: MFLO, ALUop = 100" << endl;
        } else {
            aluop = "10";
            cout << "Control: R-type, ALUop = 10" << endl;
        }
    } else if (opcode == "011100") {  // MUL instruction (MIPS32 mul) used in Factorial
        regdst = 1;
        memread = 0;
        memtoreg = 0;
        memwrt = 0;
        alusrc = 0;
        regwr = 1;
        j = 0;
        aluop = "MUL";
        cout << "Control: MUL" << endl;
    } else if (opcode == "000100") {  // BEQ (used in Fibonacci)
        regdst = 2;
        memread = 0;
        memtoreg = 2;
        aluop = "01";
        memwrt = 0;
        alusrc = 0;
        regwr = 0;
        j = 0;
        cout << "Control: BEQ, ALUop = 01" << endl;
    } else if (opcode == "000101") {  // BNE (used in Fibonacci)
        regdst = 2;
        memread = 0;
        memtoreg = 2;
        aluop = "01";
        memwrt = 0;
        alusrc = 0;
        regwr = 0;
        j = 0;
        cout << "Control: BNE" << endl;
    } else if (opcode == "100011") {  // LW
        regdst = 0;
        memread = 1;
        memtoreg = 1;
        aluop = "00";
        memwrt = 0;
        alusrc = 1;
        regwr = 1;
        j = 0;
        cout << "Control: LW, ALUop = 00" << endl;
    } else if (opcode == "000010") {  // J
        regdst = 2;
        memread = 0;
        memtoreg = 2;
        aluop = "22";
        memwrt = 0;
        alusrc = 2;
        regwr = 0;
        j = 1;
        cout << "Control: J, ALUop = 22" << endl;
    } else if (opcode == "001001" || opcode == "001000") {  // ADDI (for li and addi)
        regdst = 0;
        memread = 0;
        memtoreg = 0;
        aluop = "00";
        memwrt = 0;
        alusrc = 1;
        regwr = 1;
        j = 0;
        cout << "Control: ADDI, ALUop = 00" << endl;
    } else if (opcode == "001010") {  // SLTI (used in Fibonacci)
        regdst = 0;
        memread = 0;
        memtoreg = 0;
        aluop = "001";
        memwrt = 0;
        alusrc = 1;
        regwr = 1;
        j = 0;
        cout << "Control: SLTI, ALUop = 001" << endl;
    }
}

void writeback() {
    if (regwr == 1) {
        cout << "Writeback ----->" << endl;
        int write_reg = (regdst == 1) ? rd_num : rt_num;
        if (memtoreg == 1 && memread == 1) {
            cout << "Write Data: " << var1 << endl;
            registers[write_reg] = bitset<32>(var1).to_ulong();
        } else {
            cout << "Write Data: " << alures << endl;
            registers[write_reg] = alures;
        }
        cout << "After write back, value at destination: " << registers[write_reg] << endl;
    }
}

void Memory() {
    if (memwrt == 0 && memread == 1) {
        cout << "Memory Access----->" << endl;
        var1 = "";
        for (int i = 0; i < 4; i++) {
            var1 += memory[alures + i];
        }
        cout << "Read Data: " << var1 << endl;
    } else if (memwrt == 1 && memread == 0) {
        cout << "Memory ----->" << endl;
        var1 = to_string(convert_num_to_binary(alures));
        cout << "Write Data: " << var1 << endl;
        for (int i = 0; i < 4; i++) {
            memory[alures + i] = var1.substr(i * 8, 8);
        }
    }
}

void ALU() {
    cout << "ALU---";
    if (aluin == "010") {
        if (alusrc == 1) {
            alures = registers[rs_num] + imm_num;
            cout << "After Add:" << alures;
        } else {
            alures = registers[rs_num] + registers[rt_num];
            cout << "After Add:" << alures;
        }
    } else if (aluin == "011") {
        alures = registers[rs_num] - registers[rt_num];
        cout << "After Sub:" << alures;
    } else if (aluin == "MUL") {
        alures = registers[rs_num] * registers[rt_num];
        cout << "After Mul:" << alures;
    } else if (aluin == "110") {
        registers[rd_num] = LO;
        cout << "After MFLO: Register " << rd_num << " = " << registers[rd_num];
    } else if (aluin == "100") {
        cout << "Immediate: " << imm_num;
    } else if (aluin == "SLT_R") {
        // R-type SLT: compare registers[rs_num] and registers[rt_num]
        alures = (registers[rs_num] < registers[rt_num]) ? 1 : 0;
        cout << "After SLT:" << alures;
    } else if (aluin == "SLT") {
        // I-type SLT (for SLTI): compare registers[rs_num] with immediate
        alures = (registers[rs_num] < imm_num) ? 1 : 0;
        cout << "After SLT:" << alures;
    }
    cout << endl;
}

void Execute() {
    ctrl_ckt();
    alu_ctrl();
    // Handle branch instructions directly
    if (opcode == "000100") {  // BEQ
        if (registers[rs_num] == registers[rt_num]) {
            pc = pc + (imm_num * 4);
            cout << "Branch Taken (BEQ): New PC = " << pc << endl;
            Memory();
            writeback();
            return;
        }
    } else if (opcode == "000101") {  // BNE
        if (registers[rs_num] != registers[rt_num]) {
            pc = pc + (imm_num * 4);
            cout << "Branch Taken (BNE): New PC = " << pc << endl;
            Memory();
            writeback();
            return;
        }
    }
    ALU();
    if (j) {  // J-type instruction: standard MIPS jump formula
        int target = (pc & 0xF0000000) | (address_num << 2);
        pc = target;
        cout << "Jump Taken: New PC = " << pc << endl;
    }
    Memory();
    writeback();
}

void load_memory(vector<string> instructions) {
    int index = 0;
    for (const auto &instr : instructions) {
        for (int i = 0; i < 4; i++) {
            memory[index++] = instr.substr(i * 8, 8);
        }
    }
}

int main() {
    ifstream infile("instructions.txt");
    string line;
    vector<string> instructions;
    while (getline(infile, line)) {
        instructions.push_back(line);
    }
    load_memory(instructions);
    max_pc = BASE + instructions.size() * 4;  // Loaded instructions are from BASE to BASE + size*4
    
    // Run simulation until PC goes beyond our loaded instructions.
    while (pc < max_pc) {
        Fetch();
        Decode();
        Execute();
    }
    
    cout << "Final value in register 4: " << registers[4] << endl;
    return 0;
}
