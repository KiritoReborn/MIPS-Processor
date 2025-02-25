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
int rs_num,rt_num,rd_num,shamt_num,funct_num,imm_num,address_num,opcode_num,LO,HI;
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
        aluin="010";
    }
    else if(aluop == "10" && funct == "100000") {
        aluin="010";
    }
    else if(aluop == "10" && funct == "100010") {
        aluin="011";
    }
    else if(aluop == "10" && funct == "000010") {
        aluin="111";
    }
    else if(aluop == "01") {
        aluin="100";
    }
    else if(aluop=="11")
    {
        aluin="101";
    }
    else if(aluop=="100")
    {
        aluin="110";
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
        memwrt=0;
        alusrc=0;
        regwr=1;
        j=0;
        if(funct=="011000")
        {
            aluop="11";
            cout << "Control: R-type, ALUop = 11" << endl;
        }
        else if(funct=="010010")
        {
            aluop="100";
            cout << "Control: MFLO, ALUop = 100" << endl;
        }
        else{
            aluop="10";
            cout << "Control: R-type, ALUop = 10" << endl;
        }
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
        if(aluin=="110")
        {
            cout<<"Write Data: "<<LO<<endl;
            registers[write_reg] = LO;
        }
        else if (memtoreg == 1 && memread == 1) {  // Only read from memory when memread is active
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

void ALU(){
    cout<<"ALU---";
    if(aluin == "010") {
        if(alusrc==1) {
            alures=registers[rs_num]+imm_num;
            cout<<"After Add:"<<alures;
        }
        else if(alusrc==0) {
            alures=registers[rs_num]+registers[rt_num];
            cout<<"After Add:"<<alures;
        }
    }
    else if(aluin == "011") {
        if(alusrc==0) {
            alures=registers[rs_num]-registers[rt_num];
            cout<<"After Sub:"<<alures;
        }
    }
    else if(aluin == "101") {
        if(alusrc==0) {
            alures=(long long)registers[rs_num]*(long long)registers[rt_num];
            LO = (int)(alures & 0xFFFFFFFF);
            HI=(int)((alures >> 32) & 0xFFFFFFFF);
            cout<<"After Mul:"<<alures;
        }
    }
    else if(aluin=="110")
    {
        registers[rd_num]=LO;
        cout << "After MFLO: Register " << rd_num << " = " << registers[rd_num] << endl;
    }
    else if(aluin == "100") {
        cout<<"Immediate: "<<imm_num;
    }
    cout<<endl;
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
    // beq $3, $4 -> opcode: 0x8C1003FC
    memory[0] = "00010000"; memory[1] = "01100100"; memory[2] = "00000000"; memory[3] = "00001100";

    
    // add $3, $5 -> opcode: 0x8C1803EC
    memory[4] = "00000000"; memory[5] = "01100101"; memory[6] = "00011000"; memory[7] = "00100000";
    
    // add $1, $5 -> opcode: 0x02184020
    memory[8] = "00000000"; memory[9] = "00100101"; memory[10] = "00001000"; memory[11] = "00100000";
    // mult $2, $1 -> opcode: 0x8C0B03EC
    memory[12] = "00000000"; memory[13] = "01000001"; memory[14] = "00010000"; memory[15] = "00011000";
    // mflo to store result in $2 -> opcode: 0x8C0C03EC
    memory[16] = "00000000"; memory[17] = "00000000"; memory[18] = "00010000"; memory[19] = "00010010";

    // jump to 1st instruction
    memory[20] = "00001000"; memory[21] = "00000000"; memory[22] = "00000000"; memory[23] = "00000000";
    registers[1] = 1;  // a
    registers[2] = 1; //factorial
    registers[3]=1; //for loop counter
    registers[4]=6; // number for which factorial is to be calculated 
    registers[5] = 1;  //for (+1) in loop(dont change)

    cout << "\nStarting program execution..." << endl;

    // Main execution loop
    while (pc < 100) {
        cout << "\n=== Instruction at PC=" << pc << " ===" << endl;
        Fetch();
        Decode();
        Execute();
    }

    // Print the final result
    cout << "\nProgram execution completed." << endl;
    cout << "Factorial result stored: " << registers[2] << endl;

    return 0;
}