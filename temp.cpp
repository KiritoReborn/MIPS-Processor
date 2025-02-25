#include<iostream>
#define ll long long int
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <bitset>
using namespace std;

vector<string> memory(1000, "00000000");
vector<int> registers(32, 0);
int pc=0;
int rs_num,rt_num,rd_num,shamt_num,funct_num,imm_num,address_num,opcode_num;
string instruction,rs,rt,rd,shamt,funct,imm,address,type,opcode,aluop,aluin,var1;
int regdst,branch,memread,memtoreg,memwrt,alusrc,regwr,j,zero,alures;

map<int, string> opcodes={
    {0b001001,"I"},  // Changed to binary notation for clarity
    {0b000000,"R"},
    {0b001000,"I"},
    {0b000100,"I"},
    {0b011100,"R"},
    {0b000010,"J"},
    {0b000101,"I"},
    {0b101011,"I"},
    {0b100011,"I"}
};

int convert_num_to_binary(int num){
    if(num == 0) return 0;  // Handle zero case
    int binary_num=0;
    int i=1;
    while(num>0){
        binary_num+=(num%2)*i;
        num/=2;
        i*=10;
    }
    return binary_num;
}

int convert_binary_to_num(int binary_num){
    int num=0;
    int i=0;
    while(binary_num>0){
        num+=(binary_num%10)*pow(2,i);
        binary_num/=10;
        i++;
    }
    return num;
}

void Fetch(){
    instruction = memory[pc] + memory[pc+1] + memory[pc+2] + memory[pc+3];
    pc = pc + 4;
    cout << "Fetch----->" << endl;
    cout << "PC: " << pc << endl;
    cout << "Instruction: " << instruction << endl;
    
}

void Decode(){
    if(instruction.length() != 32) {
        cout << "Invalid instruction length. Must be 32 bits." << endl;
        return;
    }
    
    opcode=instruction.substr(0,6);
    opcode_num=bitset<6>(opcode).to_ulong();
    type=opcodes[opcode_num];
    if(type=="R"){
        rs=instruction.substr(6,5);
        rt=instruction.substr(11,5);
        rd=instruction.substr(16,5);
        shamt=instruction.substr(21,5);
        funct=instruction.substr(26,6);
        rs_num=bitset<5>(rs).to_ulong();
        rt_num=bitset<5>(rt).to_ulong();
        rd_num=bitset<5>(rd).to_ulong();
        shamt_num=bitset<5>(shamt).to_ulong();
        funct_num=bitset<6>(funct).to_ulong();
        cout<<"Type: "<<type<<endl;
        cout<<"rs: "<<rs_num<<endl;
        cout<<"rt: "<<rt_num<<endl;
        cout<<"rd: "<<rd_num<<endl;
        cout<<"shamt: "<<shamt_num<<endl;
        cout<<"funct: "<<funct_num<<endl;
    }
    else if(type=="I"){
        rs=instruction.substr(6,5);
        rt=instruction.substr(11,5);
        imm=instruction.substr(16,16);
        rs_num=bitset<5>(rs).to_ulong();
        rt_num=bitset<5>(rt).to_ulong();
        imm_num=bitset<16>(imm).to_ulong();
        cout<<"Type: "<<type<<endl;
        cout<<"rs: "<<rs_num<<endl;
        cout<<"rt: "<<rt_num<<endl;
        cout<<"imm: "<<imm_num<<endl;
    }
    else if(type=="J"){
        address=instruction.substr(6,26);
        address_num=bitset<26>(address).to_ulong();
        cout<<"Type: "<<type<<endl;
        cout<<"address: "<<address_num<<endl;
    }
    else{
        cout<<"Invalid instruction"<<endl;
    }
}

void alu_ctrl() {
    if(aluop == "00") {
        aluin = "010";  // ADD
    }
    else if(aluop == "10" && funct == "100000") {
        aluin = "010";  // ADD
    }
    else if(aluop == "10" && funct == "100010") {
        aluin = "011";  // SUB
    }
    else if(aluop == "10" && funct == "000010") {
        aluin = "111";  // Shift Right (SRL)
    }
    else if(aluop == "10" && funct == "011000") {
        aluin = "110";  // MUL
    }
    else if(aluop == "10" && funct == "011010") {
        aluin = "101";  // DIV
    }
    else if(aluop == "01") {
        aluin = "100";  // Branch (BEQ)
    }
}

void ctrl_ckt(){
    if(opcode == "101011"){ //SW
        regdst=2;
        branch=0;
        memread=0;
        memtoreg=2;
        aluop="00";
        memwrt=1;
        alusrc=1;
        regwr=0;
        j=0;
        cout << "Control: SW, ALUop = 00" << endl;
    }
    else if(opcode == "000000"){ // R-type
        regdst=1;
        branch=0;
        memread=0;
        memtoreg=1;
        aluop="10";
        memwrt=0;
        alusrc=0;
        regwr=1;
        j=0;
        cout << "Control: R-type, ALUop = 10" << endl;
    }
    else if(opcode == "000100"){ //BEQ
        regdst=2;
        branch=1;
        memread=0;
        memtoreg=2;
        aluop="01";
        memwrt=0;
        alusrc=0;
        regwr=0;
        j=0;
        if(registers[rs_num]==registers[rt_num]){
            zero=1;
        }
        else{
            zero=0;
        }
        cout << "Control: BEQ, ALUop = 01" << endl;
    }
    else if(opcode == "100011"){ //LW
        regdst=0;
        branch=0;
        memread=1;
        memtoreg=1;
        aluop="00";
        memwrt=0;
        alusrc=1;
        regwr=1;
        j=0;
        cout << "Control: LW, ALUop = 00" << endl;
    }
    else if(opcode == "000010"){ //J
        regdst=2;
        branch=2;
        memread=0;
        memtoreg=2;
        aluop="22";
        memwrt=0;
        alusrc=2;
        regwr=0;
        j=1;
        cout << "Control: J, ALUop = 00" << endl;
        
    }

}

void writeback(){
    if(regwr==1){
        cout<<"Writeback ----->"<<endl;
        cout<<"rd: "<<rd_num<<endl;
        cout<<"rt: "<<rt_num<<endl;
        cout<<"memtoReg: "<<memtoreg<<endl;
        int write_reg = (regdst == 1) ? rd_num : rt_num; // Determine the correct register to write to
        if (memtoreg == 1 && memread == 1) {  // Only read from memory when memread is active
            cout<<"Write Data: "<<var1<<endl;
            registers[write_reg] = bitset<32>(var1).to_ulong();
        } else {  // Otherwise, write ALU result
            cout<<"Write Data: "<<alures<<endl;
            registers[write_reg] = alures;
        } 
        cout << "After write back, value at destination: " << registers[write_reg] << endl;
    }    
}

void Memory(){
    if(memwrt==0 && memread==1){
        cout<<"Memory Access----->"<<endl;
        var1 = ""; // Clear var1 before appending new data
        for(int i=0;i<4;i++){
            var1 += memory[alures+i];
        }
        cout << "Read Data: " << var1 << endl;
    }
    else if(memwrt==1 && memread==0){
        cout<<"Memory ----->"<<endl;
        var1 = to_string(convert_num_to_binary(alures));
        cout<<"Write Data: "<<var1<<endl;
        for(int i=0;i<4;i++){
            memory[alures+i] = var1.substr(i*8, 8); // Write 8 bits at a time
        }
    }

}

void ALU() {
    cout << "ALU---";
    if (aluin == "010") {
        if (alusrc == 1) {
            alures = registers[rs_num] + imm_num;
            cout << "After Add: " << alures;
        }
        else if (alusrc == 0) {
            alures = registers[rs_num] + registers[rt_num];
            cout << "After Add: " << alures;
        }
    }
    else if (aluin == "011") {
        if (alusrc == 0) {
            alures = registers[rs_num] - registers[rt_num];
            cout << "After Sub: " << alures;
        }
    }
    else if (aluin == "110") {
        if (alusrc == 0) {
            alures = registers[rs_num] * registers[rt_num];
            cout << "After Mul: " << alures;
        }
    }
    else if (aluin == "101") {
        if (alusrc == 0) {
            if (registers[rt_num] != 0) {
                alures = registers[rs_num] / registers[rt_num]; // Division
                cout << "After Div: " << alures;
            } else {
                cout << "Division by zero error!";
            }
        }
    }
    else if (aluin == "111") {
        if (alusrc == 0) {
            alures = registers[rs_num] >> shamt_num; // Shift Right
            cout << "After Shift Right: " << alures;
        }
    }
    else if (aluin == "100") {
        cout << "Immediate: " << imm_num;
    }
    cout << endl; // Ensure a newline after each ALU operation
}

void Execute(){
    ctrl_ckt();
    alu_ctrl();
    ALU(); // Ensure ALU is called to perform the operation
    if(branch && zero){ // Check zero flag for branch condition
        pc=pc+(imm_num)*4;
        cout << "Branch Taken: New PC = " << pc << endl;
    } else if (j) { // J
        pc = address_num<<2;
        cout << "Jump Taken: New PC = " << pc << endl;
    }
    Memory();
    writeback();
}

int main() {
    // *Correct MIPS Instructions in Memory (Big-Endian)*
<<<<<<< HEAD
    // LW $16, 1020($0) -> opcode: 0x8C1003FC
    memory[0] = "10001100"; memory[1] = "00010000"; memory[2] = "00000011"; memory[3] = "11111100";

    // LW $24, 1004($0) -> opcode: 0x8C1803EC
    memory[4] = "10001100"; memory[5] = "00011000"; memory[6] = "00000011"; memory[7] = "11101100";

    // ADD $8, $16, $24 -> opcode: 0x02184020
    memory[8] = "00000010"; memory[9] = "00011000"; memory[10] = "00100000"; memory[11] = "00100000";

    // LW $11, 1004($0) -> opcode: 0x8C0B03EC
    memory[12] = "10001100"; memory[13] = "00001011"; memory[14] = "00000011"; memory[15] = "11101100";

    // LW $12, 1004($0) -> opcode: 0x8C0C03EC
    memory[16] = "10001100"; memory[17] = "00001100"; memory[18] = "00000011"; memory[19] = "11101100";
=======
    // BEQ $5, $1, 5 -> opcode: 0x10220005
    memory[0] = "00010000"; memory[1] = "00100101"; memory[2] = "00000000"; memory[3] = "00000101";

    // ADD $4, $3, $2 -> opcode: 0x00602020
    memory[4] = "00000000"; memory[5] = "01100010"; memory[6] = "00100000"; memory[7] = "00100000";

    // ADD $2, $3, 0 -> opcode: 0x00643020
    memory[8] = "00000000"; memory[9] = "01100000"; memory[10] = "00010000"; memory[11] = "00100000";

    // ADD $3, $4, 0 -> opcode: 0x00862020
    memory[12] = "00000000"; memory[13] = "10000000"; memory[14] = "00011000"; memory[15] = "00100000";

    // ADD $5,$5,$6 -> opcode: 0xAC230028
    memory[16] = "00000000"; memory[17] = "10100110"; memory[18] = "00101000"; memory[19] = "00100000";
>>>>>>> 4a428289bf52bb6650bd32d4c90db937e8669e40

    // factorial_loop: BEQ $8, $12, end_loop -> opcode: 0x110C0004
    memory[20] = "00010001"; memory[21] = "00001100"; memory[22] = "00000000"; memory[23] = "00000100";

    // MUL $11, $11, $12 -> opcode: 0x016C0018
    memory[24] = "00000001"; memory[25] = "01101100"; memory[26] = "00000000"; memory[27] = "00011000";

    // ADD $12, $12, $24 -> opcode: 0x01986020
    memory[28] = "00000001"; memory[29] = "10011000"; memory[30] = "00110000"; memory[31] = "00100000";

    // J factorial_loop -> opcode: 0x08000005
    memory[32] = "00001000"; memory[33] = "00000000"; memory[34] = "00000000"; memory[35] = "00000101";

    // end_loop: SW $11, 6000($0) -> opcode: 0xAC0B1770
    memory[36] = "10101100"; memory[37] = "00001011"; memory[38] = "00010111"; memory[39] = "01110000";

    // Initialize registers
<<<<<<< Updated upstream
    registers[1] = 5;  // Number of Fibonacci terms to compute
    registers[2] = 0;  // Loop counter
    registers[3] = 1;  // First Fibonacci number
    registers[4] = 1; // Result storage (dummy)
    registers[5] = 2;  // Memory address offset
    registers[6] = 1;  // Second Fibonacci number

    while (pc < 100) {
=======
<<<<<<< HEAD
    registers[16] = 5;  // Example value for $s0
    registers[24] = 1;   // Example value for $t8
    registers[8] = registers[16] + registers[24];
    registers[11] = 1;   // Factorial result initialized to 1
    registers[12] = registers[24];

    while (pc < 40) {
=======
    registers[1] = 1;  // Number of Fibonacci terms to compute
    registers[2] = 0;  // first fibonacci number
    registers[3] = 1;  // second Fibonacci number
    registers[4] = 0; // Result storage (dummy)
    registers[5] = 2;  // Memory address offset
    registers[6] = 1;  // for loop counter(+1)

    while (pc < 100) {
>>>>>>> 4a428289bf52bb6650bd32d4c90db937e8669e40
>>>>>>> Stashed changes
        Fetch();
        Decode();
        Execute();
    }

    // Print the final result
    cout << "Factorial result stored in memory[6000]: " << registers[11] << endl;
    return 0;

}