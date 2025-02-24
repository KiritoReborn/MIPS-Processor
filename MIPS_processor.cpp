#include<iostream>
#define ll long long int
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
using namespace std;

int rs_num,rt_num,rd_num,shamt_num,funct_num,imm_num,address_num,opcode_num;
string rs,rt,rd,shamt,funct,imm,address,type,opcode;

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

map<string,int> datapath={
    {"memread",0},
    {"memwrite",0},
    {"memtoreg",0},
    {"regwrite",0},
    {"memwrite",0},
    {"aluop",0},
    {"alucontrol",0},
    {"regdst",0},
    {"jump",0},
    {"branch",0},
    {"memtoreg",0},
    {"regwrite",0},
    {"memwrite",0}
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
            datapath["alucontrol"]=010;
            datapath["memread"]=1;

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

void ctrl_ckt(){
    if(strcmp("101011",opcode)==0){
        datapath["memread"]=0;
        datapath["memtoreg"]=1;
        datapath["regwrite"]=1;
        datapath["aluop"]=01;
    }
    else if(strcmp("000100",opcode)==0){
        datapath["branch"]=1;
    }
    else if(strcmp("000010",opcode)==0){
        datapath["jump"]=1;
    }
    else if(strcmp("000000",opcode)==0){
        if(strcmp("100000",funct)==0){
            datapath["aluop"]=10;
            datapath["regwrite"]=1;
        }
        else if(strcmp("100010",funct)==0){
            datapath["aluop"]=11;
            datapath["regwrite"]=1;
        }
    }
    else{
        datapath["aluop"]=00;
        datapath["regwrite"]=1;
    }
}

void ALU(){
    
}

void Memory(){
    
}

int main(){
    string instruction;
    cout<<"Enter the instruction: ";
    cin>>instruction;
}
    