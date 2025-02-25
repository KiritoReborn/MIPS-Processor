#include<iostream>
#define ll long long int
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstring>
using namespace std;

int rs_num,rt_num,rd_num,shamt_num,funct_num,imm_num,address_num,opcode_num;
std::string rs,rt,rd,shamt,funct,imm,address,type,opcode,aluop.c_str(),aluin;
int regdst,branch,memread,memtoreg,memwrt,alusrc,regwr,j,zero;

map<int, string> opcodes={
    {001001,"I"},
    {000000,"R"},
    {001000,"I"},
    {000100,"I"},
    {011100,"R"},
    {000010,"J"},
    {000101,"I"},
    {101011,"I"},
    {100011,"I"}
};

int convert_num_to_binary(int num){
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

void Decode(string instruction){
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

        if(funct_num==32){
            cout<<"Add"<<endl;

        }

        if(funct_num==34){
            cout<<"Sub"<<endl;

        }
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
        
        if(opcode_num==35){
            cout<<"Load"<<endl;

        }

        if(opcode_num==43){
            cout<<"Store"<<endl;

        }
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

void alu_ctrl()
{
    if(strcmp("00",aluop.c_str())==0)
    {
        aluin="010";
    }
    else if(strcmp("10",aluop.c_str())==0 &&strcmp("100000",funct.c_str())==0)
    {
        aluin="010";
    }
    else if(strcmp("10",aluop.c_str())==0 &&strcmp("100010",funct.c_str())==0)
    {
        aluin="011";
    }
    else if(strcmp("10",aluop.c_str())==0 &&strcmp("000010",funct.c_str())==0)
    {
        aluin="111";
    }
    else if(strcmp("01",aluop.c_str())==0)
    {
        aluin="100";
    }
}

void ctrl_ckt(){
    if(strcmp("101011",opcode)==0){
        regdst=2;
        branch=0;
        memread=0;
        memtoreg=1;
        aluop.c_str()="00";
        memwrt=1;
        alusrc=1;
        regwr=0;
        j=0;
    }
    else if(strcmp("000000",opcode)==0){
        regdst=1;
        branch=0;
        memread=0;
        memtoreg=1;
        aluop.c_str()="10";
        memwrt=0;
        alusrc=0;
        regwr=1;
        j=0;
    }
    else if(strcmp("000100",opcode)==0){
        regdst=2;
        branch=1;
        memread=0;
        memtoreg=1;
        aluop.c_str()="01";
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
        else if(strcmp("000101",opcode)==0){
        regdst=2;
        branch=1;
        memread=0;
        memtoreg=1;
        aluop.c_str()="01";
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
    else if(strcmp("100011",opcode)==0){
        regdst=0;
        branch=0;
        memread=1;
        memtoreg=1;
        aluop.c_str()="00";
        memwrt=0;
        alusrc=1;
        regwr=1;
        j=0;
    }
    else if(strcmp("011100",opcode)==0){
        regdst=1;
        branch=0;
        memread=0;
        memtoreg=0;
        aluop.c_str()="10";
        memwrt=0;
        alusrc=0;
        regwr=1;
        j=0;
    }
    else if(strcmp("000010",opcode)==0){
        regdst=2;
        branch=1;
        memread=0;
        memtoreg=1;
        aluop.c_str()="00";
        memwrt=0;
        alusrc=1;
        regwr=0;
        j=1;
    }
    //alu
    
    alu_ctrl();
}
void memory()
{

}

void ALU(){
    cout<<"ALU---";
    if(strcmp("010",aluin)==0)
    {
        if(alusrc==1)
        {
            alures=rs_num+imm_num;
            cout<<"After Add:"<<alures;
        }
        else if(alusrc==0)
        {
            alures=rs_num+rt_num;
            cout<<"After Add:"<<alures;
        }
    }
    else if(strcmp("011",aluin)==0)
    {
        if(alusrc==0)
        {
            alures=rs_num-rt_num;
            cout<<"After Sub:"<<alures;
        }
    }
    else if(strcmp("111",aluin)==0)
    {
        if(alusrc==0)
        {
            alures=rs_num*rt_num;
            cout<<"After Mul:"<<alures;
        }
    }
    else if(strcmp("100",aluin)==0)
    {
        imm_num=imm_num*4;
        cout<<"Immediate: "<<imm_num;
    }
    cout<<" ";
    memory();
    return;
}


int main(){
    string instruction;
    cout<<"Enter the instruction: ";
    cin>>instruction;
}
    