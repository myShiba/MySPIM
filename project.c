#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch (ALUControl)
    {
        case 0: 
            *ALUresult = A + B; 
            break;
        case 1: 
            *ALUresult = A - B; 
            break;
        case 2: 
            *ALUresult = A < B ? 1 : 0; 
            break; 
        case 3: 
            *ALUresult = ((int)A < (int)B) ? 1 : 0; 
            break;
        case 4: 
            *ALUresult = A & B; 
            break;
        case 5: 
            *ALUresult = A | B; 
            break;
        case 6: 
            *ALUresult = B << 16; 
            break;
        case 7: 
            *ALUresult = !A; 
            break;
        default: 
            *ALUresult = 0; 
            break;
    }
    *Zero = (*ALUresult == 0) ? 1 : 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite==1){
        if(MemtoReg==1){
            if(RegDst==1){
                Reg[r3]=memdata;
            }else{
                Reg[r2]=memdata;
            }
        }else{
            if(RegDst==1){
                Reg[r3]=ALUresult;
            }else{
                Reg[r2]=ALUresult;
            }
        }
    }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    *PC=PC+4;

    if(Jump==1){
        *PC= (jsec<<2) | (*PC | 00);
    }
    else if(Branch==1 && Zero==1){
        *PC += (extended_value<<2);
    }
}

