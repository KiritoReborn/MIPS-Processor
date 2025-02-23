from IMT2023548_MIPS_Listing import user_list

opcodesM = {"001001":"I", "000000":"R", "001000":"I", "000100":"I", "011100":"R", "000010":"J", "000101":"I",
            "101011":"I", "100011":"I"}


def decimal_to_twos_complement(num, size):
    if num >= 0:
        str = bin(num)[2:].zfill(size)
    else:
        str = bin((1 << size) + num)[2:]

    return str

def twos_complement_to_decimal(str):
    if str[0] == '1':
        return -(int(''.join('1' if bit == '0' else '0' for bit in str), 2) + 1)
    else:
        return int(str, 2)

class MIPS:
    def __init__(self):
        self.PC = 0
        self.PCb = decimal_to_twos_complement(self.PC, 32)
        self.M = []                                 #memory
        self.memW = 0                               #mem write
        self.memR = 0                               #mem read
        for i in range (0,10000000):
            self.M.append('')
        var = decimal_to_twos_complement(0, 32)
        self.M[1000] = var[0:8]
        self.M[1001] = var[8:16]
        self.M[1002] = var[16:24]
        self.M[1003] = var[24:32]
        k=1
        for i in range (1004, 5005, 4):
            var = decimal_to_twos_complement(k, 32)
            self.M[i] = var[0:8]
            self.M[i+1] = var[8:16]
            self.M[i+2] = var[16:24]
            self.M[i+3] = var[24:32]
            k += 1
        
        self.Reg = []                               #reg file  
        self.regwr = 0                              #reg write
        self.regdst = 0                               #read address 1, will get rs
        self.branch = 0                               #read address 2, will get rt
        self.memread = 0                                #write address, gets rd
        self.memtoreg = 0                               #read data 1, output value of $rs
        self.aluop = ""                               #read data 2, output value of $rt
        self.memwrt = 0
        self.alusrc=0 
        self.func=""
        self.aluin="" 
        self.rsval=0
        self.immval=0
        self.alures=0
        self.zero=0
        self.j=0                          #input data for $rd
        self.rdval=0
        self.rtval=0
        for i in range (0,32):
            self.Reg.append('')
        self.Reg[0] += decimal_to_twos_complement(0, 32)      #$0 at index 0
        
        index = 0
        #giving self.instruction as 4 bytes to memory
        for j in range (0,len(user_list)):
            b1 = user_list[j][0][0:8]
            b2 = user_list[j][0][8:16]
            b3 = user_list[j][0][16:24]
            b4 = user_list[j][0][24:32]
            self.M[index] = b1
            index += 1
            self.M[index] = b2
            index += 1
            self.M[index] = b3
            index += 1
            self.M[index] = b4
            index += 1
        while (self.PC != 99999999 and len(self.M[self.PC])!=0):
            print("PC: ",self.PCb)
            MIPS.IF(self)
            if(self.branch==1 and self.zero==1 and self.j==0):
                self.PC=self.PC+4+self.immval
                self.PCb = decimal_to_twos_complement(self.PC, 32)
            elif(self.j==1):
                self.PC=(twos_complement_to_decimal(self.jmpaddr)-(1048576)*4)
                self.PCb = decimal_to_twos_complement(self.PC, 32)
            else:
                self.PC+=4
                self.PCb = decimal_to_twos_complement(self.PC, 32)
            
            
        
    def IF(self):
        print("Instruction Fetch ----->")
        self.instr = ''
        for j in range (self.PC, self.PC+4):
            self.instr += self.M[j]
        print("Instruction: ",self.instr)

        self.form = self.instr[0:6]                      #6 bit opcode
        self.rs = ''                                #5 bit rs
        self.rt = ''                                #5 bit rt
        self.rd = ''                                #5 bit rd
        self.shamt = ''                             #5 bit shift amount
        self.funct = ''                             #6 bit function value
        self.imm = ''                               #16 bit immediate value
        self.jmp = ''                               #26 bit jump address
        self.writedata = ''
        self.data=''
        print(" ")
        self.ID()

    def ID(self):
        print("Instruction Decode ----->")
        #assigning values based on format
        if (opcodesM[self.form] == "R"):
            self.rs += self.instr[6:11]
            self.rt += self.instr[11:16]
            self.rd += self.instr[16:21]
            self.shamt += self.instr[21:26]
            self.funct += self.instr[26:32]
        if (opcodesM[self.form] == "I"):
            self.rs += self.instr[6:11]
            self.rt += self.instr[11:16]
            self.imm += self.instr[16:32]    
        if (opcodesM[self.form] == "J"):
            self.jmp += self.instr[6:32]
        
        if(self.form == "101011"):
                print("STOR:")
                self.rsval=twos_complement_to_decimal(self.Reg[int(self.rs,2)])
                self.rtval=twos_complement_to_decimal(self.Reg[int(self.rt,2)])
                temp = twos_complement_to_decimal(self.imm)
                self.imm = decimal_to_twos_complement(temp, 32)
                self.immval=twos_complement_to_decimal(self.imm)
                print("Value of RS:",self.rsval,"Value of RT:",self.rtval,"Value of Imm:",self.immval)
        elif(self.form == "100011"):
                print("LOAD:")
                self.rsval=twos_complement_to_decimal(self.Reg[int(self.rs,2)])
                temp = twos_complement_to_decimal(self.imm)
                self.imm = decimal_to_twos_complement(temp, 32)
                self.immval=twos_complement_to_decimal(self.imm)
                print("Value of RS=",self.rsval,"Value of RT=",self.rtval,"Value of Imm=",self.immval)
                 
        elif(self.form=='000000'):
            if(self.funct=='100000'):
                print("ADD:")
                self.rsval=twos_complement_to_decimal(self.Reg[int(self.rs,2)])
                self.rtval=twos_complement_to_decimal(self.Reg[int(self.rt,2)])
                print("Value of RS=",self.rsval,"Value of RT=",self.rtval)
            elif(self.funct=='100010'):
                print("SUB:")
                self.rsval=twos_complement_to_decimal(self.Reg[int(self.rs,2)])
                self.rtval=twos_complement_to_decimal(self.Reg[int(self.rt,2)])
                print("Value of RS=",self.rsval,"Value of RT=",self.rtval)
        elif(self.form=='011100'):
            print("MUL:")
            self.funct='000010'
            self.rsval=twos_complement_to_decimal(self.Reg[int(self.rs,2)])
            self.rtval=twos_complement_to_decimal(self.Reg[int(self.rt,2)])
            print("Value of RS=",self.rsval,"Value of RT=",self.rtval)
        elif(self.form=='000100'):
            print("BEQ:")
            self.rsval=twos_complement_to_decimal(self.Reg[int(self.rs,2)])
            self.rtval=twos_complement_to_decimal(self.Reg[int(self.rt,2)])
            temp = twos_complement_to_decimal(self.imm)
            self.imm = decimal_to_twos_complement(temp, 32)
            self.immval=twos_complement_to_decimal(self.imm)
            print("Value of RS=",self.rsval,"Value of RT=",self.rtval,"Value of Imm=",self.immval)
        elif(self.form=='000101'):
            print("BNE:")
            self.rsval=twos_complement_to_decimal(self.Reg[int(self.rs,2)])
            self.rtval=twos_complement_to_decimal(self.Reg[int(self.rt,2)])
            temp = twos_complement_to_decimal(self.imm)
            self.imm = decimal_to_twos_complement(temp, 32)
            self.immval=twos_complement_to_decimal(self.imm)
            print("Value of RS=",self.rsval,"Value of RT=",self.rtval,"Value of Imm=",self.immval)
        elif(self.form=='000010'):
            print("JUMP:")
            self.jmpaddr = "0000" + self.jmp + "00"
            self.immval=twos_complement_to_decimal(self.jmpaddr)
            print("Jump address=",self.jmpaddr)
        print(" ")
        self.ctrl_ckt()
        return

            
    def ctrl_ckt(self):
        if(self.form=="101011"):
            self.regdst=2
            self.branch=0
            self.memread=0
            self.memtoreg=1
            self.aluop="00"
            self.memwrt=1
            self.alusrc=1
            self.regwr=0
            self.j=0
        elif(self.form=="100011"):
            self.regdst=0
            self.branch=0
            self.memread=1
            self.memtoreg=1
            self.aluop="00"
            self.memwrt=0
            self.alusrc=1
            self.regwr=1
            self.j=0
        elif(self.form=="000000" or self.form=='011100'):
            self.regdst=1
            self.branch=0
            self.memread=0
            self.memtoreg=0
            self.aluop="10"
            self.memwrt=0
            self.alusrc=0
            self.regwr=1
            self.j=0
        elif(self.form=="000100"):
            self.regdst=2
            self.branch=1
            self.memread=0
            self.memtoreg=1
            self.aluop="01"
            self.memwrt=0
            self.alusrc=0
            self.regwr=0
            self.j=0
            if(self.rsval==self.rtval):
                self.zero=1
        elif(self.form=="000101"):
            self.regdst=2
            self.branch=1
            self.memread=0
            self.memtoreg=1
            self.aluop="01"
            self.memwrt=0
            self.alusrc=0
            self.regwr=0
            self.j=0
            if(self.rsval!=self.rtval):
                self.zero=1
        elif(self.form=="000010"):
            self.regdst=2
            self.branch=1
            self.memread=0
            self.memtoreg=1
            self.aluop="00"
            self.memwrt=0
            self.alusrc=1
            self.regwr=0
            self.j=1
            print("-"*75)
            return
        if(self.regdst==1):
            self.rdval=int(self.rd,2)
        elif(self.regdst==0):
            self.rdval=int(self.rt,2)
        self.alu_ctrl()
        return
    def alu_ctrl(self):
        if(self.aluop=="00"):
            self.aluin="010"
        elif(self.aluop=="10" and self.funct=="100000"):
            self.aluin="010"
        elif(self.aluop=="10" and self.funct=="100010"):
            self.aluin="011"
        elif(self.aluop=="10" and self.funct=="000010"):
            self.aluin="111"
        elif(self.aluop=='01'):
            self.aluin="100"  
        self.alu()
        return
    def alu(self):
        print("ALU ----->")
        if(self.aluin=="010"):
            if(self.alusrc==1):
                self.alures=self.rsval+self.immval
                print("After Add:",self.alures)
            elif(self.alusrc==0):
                self.alures=self.rsval+self.rtval
                print("After Add:",self.alures)
        elif(self.aluin=="011"):
            if(self.alusrc==0):
                self.alures=self.rsval-self.rtval
                print("After Sub:",self.alures)
        elif(self.aluin=="111"):
            if(self.alusrc==0):
                self.alures=self.rsval*self.rtval
                print("After Mul:",self.alures)
        elif(self.aluin=='100'):
            self.immval=self.immval*4
            print("Immediate: ",self.immval)
        print(" ")
        self.mem()
        return

    def mem(self):
        
        if (self.memwrt==0 and self.memread==1):
            print("Memory Access ----->")
            var1 = ''
            for i in range (0,4):
                var1 += self.M[self.alures+i]
            self.data =var1
            print("Read Data: ",self.data)
            print(" ")
        elif(self.memwrt==1 and self.memread==0):
            print("Memory:")
            var1 = decimal_to_twos_complement(self.rtval,32)
            print("Write Data: ",var1)
            self.M[self.alures] = var1[0:8]
            self.M[self.alures+1] = var1[8:16]
            self.M[self.alures+2] = var1[16:24]
            self.M[self.alures+3] = var1[24:32]
            print(" ")
        self.writeback()
        return

    def writeback(self):

        if (self.regwr==1):
            print("Writeback ----->")
            print("rd :", self.rdval)
            print("MemtoReg: ",self.memtoreg)
            if(self.memtoreg==1):
                print("Write Data: ",self.data)
                self.writedata=self.data
                self.Reg[self.rdval] = self.writedata
                print("After write back, value at destination: ", self.Reg[self.rdval])
            else:
                print("Write Data: ",decimal_to_twos_complement(self.alures,32))
                self.writedata=self.alures
                self.Reg[self.rdval] = decimal_to_twos_complement(self.writedata,32)
                print("After write back, value at destination: ", self.Reg[self.rdval])
        print("-"*75)
    
M = MIPS()
ans = M.M[6000]
ans += M.M[6001]
ans += M.M[6002]
ans += M.M[6003]
print(twos_complement_to_decimal(ans))
