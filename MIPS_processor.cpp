#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <fstream>
using namespace std;

const int BASE = 0x00400000;  // Simulated code base
vector<string> memory(1000, "00000000");
vector<int> registers(32, 0);
int pc = BASE;              // Initialize PC to code base
int max_pc;                 // max_pc = BASE + (number of instructions * 4)

// Global instruction fields
string instruction, opcode, rs, rt, rd, shamt, funct, imm, address;
int opcode_num, rs_num, rt_num, rd_num, shamt_num, funct_num, imm_num, address_num;

// Control signals and ALU op selector
int regdst, memread, memtoreg, memwrt, alusrc, regwr, jump;
string aluop; // "ADD", "SUB", "SLT", "MUL"

// Updated opcodes map (as produced by MARS)
map<int, string> opcodes = {
    {0b001001, "I"},  // ADDI
    {0b001000, "I"},  // alternative ADDI
    {0b000000, "R"},  // R-type
    {0b000100, "I"},  // BEQ
    {0b000101, "I"},  // BNE
    {0b011100, "R"},  // MUL (MIPS32 mul)
    {0b000010, "J"},  // J
    {0b101011, "I"},  // SW
    {0b100011, "I"},  // LW
    {0b001010, "I"}   // SLTI
};

// FETCH: Concatenate 4 consecutive 8-bit strings into a 32-bit instruction.
void Fetch() {
    int index = pc - BASE;
    instruction = memory[index] + memory[index+1] + memory[index+2] + memory[index+3];
    cout << "\nFetch: PC = " << pc << "\nInstruction = " << instruction << endl;
    pc += 4;
}

// DECODE: Extract opcode and fields using substring operations.
void Decode() {
    if(instruction.length() != 32) {
        cout << "Invalid instruction length." << endl;
        return;
    }
    opcode = instruction.substr(0,6);
    opcode_num = bitset<6>(opcode).to_ulong();
    string type = opcodes[opcode_num];
    
    if(type == "R") {
        rs    = instruction.substr(6,5);
        rt    = instruction.substr(11,5);
        rd    = instruction.substr(16,5);
        shamt = instruction.substr(21,5);
        funct = instruction.substr(26,6);
        rs_num    = bitset<5>(rs).to_ulong();
        rt_num    = bitset<5>(rt).to_ulong();
        rd_num    = bitset<5>(rd).to_ulong();
        shamt_num = bitset<5>(shamt).to_ulong();
        funct_num = bitset<6>(funct).to_ulong();
        cout << "Decode R-type: rs=" << rs_num << ", rt=" << rt_num 
             << ", rd=" << rd_num << ", funct=" << funct << endl;
    }
    else if(type == "I") {
        rs  = instruction.substr(6,5);
        rt  = instruction.substr(11,5);
        imm = instruction.substr(16,16);
        rs_num = bitset<5>(rs).to_ulong();
        rt_num = bitset<5>(rt).to_ulong();
        imm_num = bitset<16>(imm).to_ulong();
        // Sign-extend if needed
        if(imm[0]=='1'){
            imm_num -= (1 << 16);
        }
        cout << "Decode I-type: rs=" << rs_num << ", rt=" << rt_num 
             << ", imm=" << imm_num << endl;
    }
    else if(type == "J") {
        address = instruction.substr(6,26);
        address_num = bitset<26>(address).to_ulong();
        cout << "Decode J-type: address=" << address_num << endl;
    }
    else {
        cout << "Unknown instruction type" << endl;
    }
}

// CONTROL: Set control signals and choose the ALU operation.
void Control() {
    // Reset control signals
    regdst = memread = memtoreg = memwrt = alusrc = regwr = jump = 0;
    aluop = "ADD";  // default
    
    if(opcode == "101011") {            // SW
        regdst = 2; memread = 0; memtoreg = 2;
        alusrc = 1; regwr = 0; jump = 0;
        aluop = "ADD"; // for address calculation
        cout << "Control: SW" << endl;
    }
    else if(opcode == "000000") {       // R-type
        regdst = 1; memread = 0; memtoreg = 1;
        alusrc = 0; regwr = 1; jump = 0;
        if(funct == "101010") { // SLT
            aluop = "SLT";
        } else if(funct == "100010") { // SUB
            aluop = "SUB";
        } else {
            aluop = "ADD";
        }
        cout << "Control: R-type, ALU op = " << aluop << endl;
    }
    else if(opcode == "011100") {       // MUL
        regdst = 1; memread = 0; memtoreg = 1;
        alusrc = 0; regwr = 1; jump = 0;
        aluop = "MUL";
        cout << "Control: MUL" << endl;
    }
    else if(opcode == "000100") {       // BEQ
        regdst = 2; memread = 0; memtoreg = 2;
        alusrc = 0; regwr = 0; jump = 0;
        aluop = "SUB"; // for comparison
        cout << "Control: BEQ" << endl;
    }
    else if(opcode == "000101") {       // BNE
        regdst = 2; memread = 0; memtoreg = 2;
        alusrc = 0; regwr = 0; jump = 0;
        aluop = "SUB"; // for comparison
        cout << "Control: BNE" << endl;
    }
    else if(opcode == "100011") {       // LW
        regdst = 0; memread = 1; memtoreg = 1;
        alusrc = 1; regwr = 1; jump = 0;
        aluop = "ADD"; // for effective address
        cout << "Control: LW" << endl;
    }
    else if(opcode == "000010") {       // J
        regdst = 2; memread = 0; memtoreg = 2;
        alusrc = 2; regwr = 0; jump = 1;
        cout << "Control: J" << endl;
    }
    else if(opcode == "001001" || opcode == "001000") { // ADDI
        regdst = 0; memread = 0; memtoreg = 0;
        alusrc = 1; regwr = 1; jump = 0;
        aluop = "ADD";
        cout << "Control: ADDI" << endl;
    }
    else if(opcode == "001010") {       // SLTI
        regdst = 0; memread = 0; memtoreg = 0;
        alusrc = 1; regwr = 1; jump = 0;
        aluop = "SLT";
        cout << "Control: SLTI" << endl;
    }
    else {
        cout << "Control: Unknown opcode" << endl;
    }
}

// ALU: Compute the result based on the selected aluop.
void ALU_Operation() {
    long long result = 0;
    if(aluop == "ADD") {
        if(alusrc == 1)
            result = registers[rs_num] + imm_num;
        else
            result = registers[rs_num] + registers[rt_num];
        cout << "ALU: ADD result = " << result << endl;
    }
    else if(aluop == "SUB") {
        result = registers[rs_num] - registers[rt_num];
        cout << "ALU: SUB result = " << result << endl;
    }
    else if(aluop == "SLT") {
        result = (registers[rs_num] < registers[rt_num]) ? 1 : 0;
        cout << "ALU: SLT result = " << result << endl;
    }
    else if(aluop == "MUL") {
        result = registers[rs_num] * registers[rt_num];
        cout << "ALU: MUL result = " << result << endl;
    }
    
    if(regwr && aluop != "JUMP") {
        int dest = (regdst == 1) ? rd_num : rt_num;
        registers[dest] = result;
        cout << "Writeback: Register[" << dest << "] = " << result << endl;
    }
}

// Execute: First set control signals, then handle branch/jump before doing ALU computation.
void Execute() {
    Control();
    // Jump handling (J-type)
    if(jump == 1) {
        int target = (pc & 0xF0000000) | (address_num << 2);
        pc = target;
        cout << "Jump taken: New PC = " << pc << endl;
        return;
    }
    // Branch handling: use register comparisons directly.
    if(opcode == "000100") { // BEQ
        if(registers[rs_num] == registers[rt_num]) {
            pc = pc + (imm_num * 4);
            cout << "BEQ taken: New PC = " << pc << endl;
            return;
        }
    }
    if(opcode == "000101") { // BNE
        if(registers[rs_num] != registers[rt_num]) {
            pc = pc + (imm_num * 4);
            cout << "BNE taken: New PC = " << pc << endl;
            return;
        }
    }
    ALU_Operation();
}

// MEMORY ACCESS: (Simulated load/store)
void MemoryAccess() {
    if(memread == 1) {
        cout << "Memory Access: Simulated load." << endl;
    }
    if(memwrt == 1) {
        cout << "Memory Access: Simulated store." << endl;
    }
}

// load_memory: Split each 32-bit instruction string into four 8-bit chunks.
void load_memory(const vector<string>& instructions) {
    int index = 0;
    for (const auto &instr : instructions) {
        memory[index++] = instr.substr(0, 8);
        memory[index++] = instr.substr(8, 8);
        memory[index++] = instr.substr(16, 8);
        memory[index++] = instr.substr(24, 8);
    }
}

int main() {
    ifstream infile("instructions.txt");
    string line;
    vector<string> instructions;
    while(getline(infile, line)) {
        instructions.push_back(line);
    }
    load_memory(instructions);
    max_pc = BASE + instructions.size() * 4;
    
    while(pc < max_pc) {
        Fetch();
        Decode();
        Execute();
        MemoryAccess();
    }
    
    cout << "\nFinal value in register 4: " << registers[11] << endl;
    return 0;
}
