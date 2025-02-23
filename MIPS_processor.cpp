#include<iostream>
#define ll long long int
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;

int rs_num,rt_num,rd_num,shamt_num,funct_num,imm_num,address_num;
string rs,rt,rd,shamt,funct,imm,address;

map<string, int> registers={
    {"$zero",0},
    {"$at",1},
    {"$v0",2},
    {"$v1",3},
    {"$a0",4},
    {"$a1",5},
    {"$a2",6},
    {"$a3",7},
    {"$t0",8},
    {"$t1",9},
    {"$t2",10},
    {"$t3",11},
    {"$t4",12},
    {"$t5",13},
    {"$t6",14},
    {"$t7",15},
    {"$s0",16},
    {"$s1",17},
    {"$s2",18},
    {"$s3",19},
    {"$s4",20},
    {"$s5",21},
    {"$s6",22},
    {"$s7",23},
    {"$t8",24},
    {"$t9",25},
    {"$k0",26},
    {"$k1",27},
    {"$gp",28},
    {"$sp",29},
    {"$fp",30},
    {"$ra",31}
};

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
    string opcode=instruction.substr(0,6);
    int opcode_num=conver_binary_to_num(stoi(opcode));
    string type=opcodes[opcode_num];
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
            cout<<"ADD"<<endl;


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

void ALU(){

}

void Memory(){
    hello;
}

int main(){
    string instruction;
    cout<<"Enter the instruction: ";
    cin>>instruction;
}
    