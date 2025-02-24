#include<iostream>
#define ll long long int
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

vector<string> memory(1000, "00000000");
int pc=0;
int rs_num,rt_num,rd_num,shamt_num,funct_num,imm_num,address_num,opcode_num;
string rs,rt,rd,shamt,funct,imm,address,type,opcode,aluop,aluin;
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

int conver_binary_to_num(int binary_num){
    int num=0;
    int i=0;
    while(binary_num>0){
        num+=(binary_num%10)*pow(2,i);
        binary_num/=10;
        i++;
    }
    return num;
}
void Fetch(string& instruction){
    instruction = memory[pc] + memory[pc+1] + memory[pc+2] + memory[pc+3];
    pc = pc + 4;
    cout << "Fetch----->" << endl;
    cout << "PC: " << pc << endl;
    cout << "Instruction: " << instruction << endl;
}

void Decode(string instruction){
    if(instruction.length() != 32) {
        cout << "Invalid instruction length. Must be 32 bits." << endl;
        return;
    }
    
    opcode=instruction.substr(0,6);
    opcode_num=conver_binary_to_num(stoi(opcode));
    type=opcodes[opcode_num];
    if(type=="R"){
        rs=instruction.substr(6,5);
        rt=instruction.substr(11,5);
        rd=instruction.substr(16,5);
        shamt=instruction.substr(21,5);
        funct=instruction.substr(26,6);
        rs_num=conver_binary_to_num(stoi(rs));
        rt_num=conver_binary_to_num(stoi(rt));
        rd_num=conver_binary_to_num(stoi(rd));
        shamt_num=conver_binary_to_num(stoi(shamt));
        funct_num=conver_binary_to_num(stoi(funct));
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
        rs_num=conver_binary_to_num(stoi(rs));
        rt_num=conver_binary_to_num(stoi(rt));
        imm_num=conver_binary_to_num(stoi(imm));
        cout<<"Type: "<<type<<endl;
        cout<<"rs: "<<rs_num<<endl;
        cout<<"rt: "<<rt_num<<endl;
        cout<<"imm: "<<imm_num<<endl;
    }
    else if(type=="J"){
        address=instruction.substr(6,26);
        address_num=conver_binary_to_num(stoi(address));
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
}

void ctrl_ckt(){
    if(opcode == "101011"){
        regdst=2;
        branch=0;
        memread=0;
        memtoreg=1;
        aluop="00";
        memwrt=1;
        alusrc=1;
        regwr=0;
        j=0;
    }
    else if(opcode == "000000"){
        regdst=1;
        branch=0;
        memread=0;
        memtoreg=1;
        aluop="10";
        memwrt=0;
        alusrc=0;
        regwr=1;
        j=0;
    }
    else if(opcode == "000100"){
        regdst=2;
        branch=1;
        memread=0;
        memtoreg=1;
        aluop="01";
        memwrt=0;
        alusrc=0;
        regwr=0;
        j=0;
        if(rs_num==rt_num){
            zero=1;
        }
        else{
            zero=0;
        }
    }
    else if(opcode == "000101"){
        regdst=2;
        branch=1;
        memread=0;
        memtoreg=1;
        aluop="01";
        memwrt=0;
        alusrc=1;
        regwr=1;
        j=0;
        if(rs_num!=rt_num){
            zero=1;
        }
        else{
            zero=0;
        }
    }
    else if(opcode == "100011"){
        regdst=0;
        branch=0;
        memread=1;
        memtoreg=1;
        aluop="00";
        memwrt=0;
        alusrc=1;
        regwr=1;
        j=0;
    }
    else if(opcode == "011100"){
        regdst=1;
        branch=0;
        memread=0;
        memtoreg=0;
        aluop="10";
        memwrt=0;
        alusrc=0;
        regwr=1;
        j=0;
    }
    else if(opcode == "000010"){
        regdst=2;
        branch=1;
        memread=0;
        memtoreg=1;
        aluop="00";
        memwrt=0;
        alusrc=1;
        regwr=0;
        j=1;
    }
    
    alu_ctrl();
}

void writeback(){
    if(regwr==1){
        cout<<"Writeback ----->"<<endl;
        cout<<"rd: "<<rd_num<<endl;
        cout<<"memtoReg: "<<memtoreg<<endl;
        if(memtoreg==1){
            cout<<"Write Data: "<<alures<<endl;

        }
    }    
}

void ALU(){
    cout<<"ALU---";
    if(aluin == "010") {
        if(alusrc==1) {
            alures=rs_num+imm_num;
            cout<<"After Add:"<<alures;
        }
        else if(alusrc==0) {
            alures=rs_num+rt_num;
            cout<<"After Add:"<<alures;
        }
    }
    else if(aluin == "011") {
        if(alusrc==0) {
            alures=rs_num-rt_num;
            cout<<"After Sub:"<<alures;
        }
    }
    else if(aluin == "111") {
        if(alusrc==0) {
            alures=rs_num*rt_num;
            cout<<"After Mul:"<<alures;
        }
    }
    else if(aluin == "100") {
        imm_num=imm_num*4;
        cout<<"Immediate: "<<imm_num;
    }
    cout<<endl;
    Memory();
}

int main(){
    string instruction;
    cout<<"Enter the instruction (32 bits): ";
    cin>>instruction;
    
    Decode(instruction);
    ctrl_ckt();
    ALU();
    
    return 0;
}