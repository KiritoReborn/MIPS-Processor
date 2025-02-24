#include<iostream>
#define ll long long int
#include <vector>
using namespace std;

// Control signals
int regdst;
int alusrc;
int memreg;
int regwr;
int memrd;
int memwr;
int branch;
int aluop1;
int aluop0;
int jmp;
int aluop;

enum reg {
    zero,  at,  v0,  v1,  a0,  a1,  a2,  a3,  // 0-7
    t0,    t1,  t2,  t3,  t4,  t5,  t6,  t7,  // 8-15
    s0,    s1,  s2,  s3,  s4,  s5,  s6,  s7,  // 16-23
    t8,    t9,  k0,  k1,  gp,  sp,  fp,  ra   // 24-31
};
ll r[32] = {0}; // Initialize all registers to 0

int PC = 0; // Program Counter
int inst, op, rs, rt, rd, shamt, func, address, jumpadd;
char type;
int ctrlinput;
unsigned char memory[128] = {0}; // Memory initialized to 0 (8-bit per location)

// Fetch instruction from memory
int fetch() {
    if (PC >= 128) {
        cout << "Program terminated: PC out of memory bounds." << endl;
        exit(0);
    }
    // Combine 4 bytes to form a 32-bit instruction (big-endian)
    inst = (memory[PC] << 24) | (memory[PC + 1] << 16) | (memory[PC + 2] << 8) | memory[PC + 3];
    op = (inst >> 26) & 0x3F;
    cout << "Fetch: PC = " << PC << ", Instruction = " << hex << inst << endl;
    PC += 4; // Increment PC
    return inst;
}

// Decode instruction
void decode() {
    if (op == 0) { // R-type
        type = 'r';
        rs = (inst >> 21) & 0x1F;
        rt = (inst >> 16) & 0x1F;
        rd = (inst >> 11) & 0x1F;
        shamt = (inst >> 6) & 0x1F;
        func = inst & 0x3F;
        cout << "Decode: R-type, rs = " << rs << ", rt = " << rt << ", rd = " << rd << ", func = " << func << endl;
    } else if (op == 0b000010) { // J-type
        type = 'j';
        jumpadd = (inst & 0x3FFFFFF) << 2;
        cout << "Decode: J-type, jump address = " << jumpadd << endl;
    } else { // I-type
        type = 'i';
        rs = (inst >> 21) & 0x1F;
        rt = (inst >> 16) & 0x1F;
        address = inst & 0xFFFF;
        cout << "Decode: I-type, rs = " << rs << ", rt = " << rt << ", address = " << address << endl;
    }
}

// Control circuit to set control signals
void ctrlcircuit() {
    if (op == 0b000000) { // R-type
        regdst = 1;
        alusrc = 0;
        memreg = 0;
        regwr = 1;
        memrd = 0;
        memwr = 0;
        branch = 0;
        aluop1 = 1;
        aluop0 = 0;
        jmp = 0;
        aluop = 0b10;
        cout << "Control: R-type, ALUop = 10" << endl;
    } else if (op == 0b100011) { // LW
        regdst = 0;
        alusrc = 1;
        memreg = 1;
        regwr = 1;
        memrd = 1;
        memwr = 0;
        branch = 0;
        aluop1 = 0;
        aluop0 = 0;
        aluop = 0b00;
        jmp = 0;
        cout << "Control: LW, ALUop = 00" << endl;
    } else if (op == 0b101011) { // SW
        regdst = 2;
        alusrc = 1;
        memreg = 2;
        regwr = 0;
        memrd = 0;
        memwr = 1;
        branch = 0;
        aluop1 = 0;
        aluop0 = 0;
        jmp = 0;
        aluop = 0b00;
        cout << "Control: SW, ALUop = 00" << endl;
    } else if (op == 0b000100) { // BEQ
        regdst = 2;
        alusrc = 0;
        memreg = 2;
        regwr = 0;
        memrd = 0;
        memwr = 0;
        branch = 1;
        aluop1 = 0;
        aluop0 = 1;
        jmp = 0;
        aluop = 0b01;
        cout << "Control: BEQ, ALUop = 01" << endl;
    } else if (op == 0b000010) { // J
        regdst = 2;
        alusrc = 2;
        memreg = 2;
        regwr = 0;
        memrd = 0;
        memwr = 0;
        branch = 2;
        aluop1 = 2;
        aluop0 = 2;
        jmp = 1;
        aluop = 0b11;
        cout << "Control: J, ALUop = 11" << endl;
    }
}

// Determine ALU control input
void ctrlinputfn() {
    if (aluop == 0b10) { // R-type
        if (func == 0b100000) { // ADD
            ctrlinput = 0b010;
        } else if (func == 0b100010) { // SUB
            ctrlinput = 0b011;
        } else if (func == 0b100100) { // AND
            ctrlinput = 0b000;
        } else if (func == 0b100101) { // OR
            ctrlinput = 0b001;
        } else if (func == 0b101010) { // SLT
            ctrlinput = 0b100;
        }
        cout << "ALU Control: R-type, func = " << func << ", ALU input = " << ctrlinput << endl;
    } else if (aluop == 0b00) { // LW/SW
        ctrlinput = 0b010; // ADD
        cout << "ALU Control: LW/SW, ALU input = 010" << endl;
    } else if (aluop == 0b01) { // BEQ
        ctrlinput = 0b011; // SUB
        cout << "ALU Control: BEQ, ALU input = 011" << endl;
    }
}

// ALU function
int alu() {
    int result = 0;
    if (ctrlinput == 0b010) { // ADD
        result = r[rs] + r[rt];
    } else if (ctrlinput == 0b011) { // SUB
        result = r[rs] - r[rt];
    } else if (ctrlinput == 0b000) { // AND
        result = r[rs] & r[rt];
    } else if (ctrlinput == 0b001) { // OR
        result = r[rs] | r[rt];
    } else if (ctrlinput == 0b100) { // SLT
        result = (r[rs] < r[rt]) ? 1 : 0;
    }
    cout << "ALU: rs = " << r[rs] << ", rt = " << r[rt] << ", result = " << result << endl;
    return result;
}

// Write data to memory (8-bit per location)
void writememory(int address, int data) {
    if (address >= 0 && address < 128) {
        memory[address] = data & 0xFF; // Store only the lower 8 bits
        cout << "Memory Write: Address = " << address << ", Data = " << (data & 0xFF) << endl;
    } else {
        cout << "Memory Write Error: Address " << address << " out of bounds." << endl;
    }
}

// Read data from memory (8-bit per location)
int readmemory(int address) {
    if (address >= 0 && address < 128) {
        int data = memory[address];
        cout << "Memory Read: Address = " << address << ", Data = " << data << endl;
        return data;
    } else {
        cout << "Memory Read Error: Address " << address << " out of bounds." << endl;
        return 0;
    }
}

// Execute instruction
void execute() {
    ctrlcircuit(); // Set control signals
    ctrlinputfn(); // Determine ALU control input

    int alu_result = alu();
    if (branch && (alu_result == 0)) { // BEQ
        PC = PC+(address)*4;
        cout << "Branch Taken: New PC = " << PC << endl;
    } else if (jmp) { // J
        PC = jumpadd;
        cout << "Jump Taken: New PC = " << PC << endl;
    }

    if (memwr) { // SW
        writememory(alu_result, r[rt]);
    } else if (memrd) { // LW
        r[rt] = readmemory(alu_result);
    } else if (regwr) { // R-type or LW
        if (regdst) {
            r[rd] = alu_result;
            cout << "Register Write: rd = " << rd << ", Data = " << r[rd] << endl;
        } else {
            r[rt] = alu_result;
            cout << "Register Write: rt = " << rt << ", Data = " << r[rt] << endl;
        }
    }
}

int main() {

memory[0]=0b00010000;

memory[1]=0b00100101;

memory[2]=0b00000000;

memory[3]=0b00000101;

memory[4]=0b00000000;

memory[5]=0b01100010;

memory[6]=0b00100000;

memory[7]=0b00100000;

memory[8]=0b00000000;

memory[9]=0b01100000;

memory[10]=0b00010000;

memory[11]=0b00100000;

memory[12]=0b00000000;

memory[13]=0b10000000;

memory[14]=0b00011000;

memory[15]=0b00100000;

memory[16]=0b00000000;

memory[17]=0b10100110;

memory[18]=0b00101000;

memory[19]=0b00100000;

memory[20]=0b00001000;

memory[21]=0b00000000;

memory[22]=0b00000000;

memory[23]=0b00000000;


r[1]=7;
r[2]=0;
r[3]=1;
r[4]=88;
r[5]=2;
r[6]=1;

    while (PC < 30) {
        fetch();
        decode();
        execute();
    }

    // Print the final result
    cout << "Final value in $3: " << r[4] << endl;
    return 0;}