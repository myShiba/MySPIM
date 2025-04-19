#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl){
        case 0x0:
            *ALUresult = A + B; // Add
            break;
        case 0x1:
            *ALUresult = A - B; // Sub
            break;
        case 0x2:
            *ALUresult = (A < B) ? 1 : 0; // SLT signed
            break;
        case 0x3:
            *ALUresult = (A < (unsigned int)B) ? 1 : 0; // SLT unsigned
            break;
        case 0x4:
            *ALUresult = A & B; // AND
            break;
        case 0x5:
            *ALUresult = A | B; // OR
            break;
        case 0x6:
            *ALUresult = B << 16; // Shift
            break;
        case 0x7:
            *ALUresult = ~A; // NOT
            break;
        }

    *Zero = (*ALUresult == 0) ? 1 : 0; // if zero
}


/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    unsigned adress = PC >> 2;
    //This will check if the PC is divisible by 4
    if(PC % 4 != 0){
        return 1;
    }
    //This  will fetch the instruction
    *instruction = Mem[adress];
    return 0;
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // op (6 bits)
    *op = (instruction >> 26) & 0x3F;

    // r1 (5 bits)
    *r1 = (instruction >> 21) & 0x1F;

    // r2 (5 bits)
    *r2 = (instruction >> 16) & 0x1F;

    // r3 (5 bits)
    *r3 = (instruction >> 11) & 0x1F;

    // funct (6 bits)
    *funct = instruction & 0x3F;

    // offset (16 bits)
    *offset = instruction & 0xFFFF;

    // jsec (26 bits)
    *jsec = instruction & 0x03FFFFFF;

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls) {
    // Decode the instruction using the opcode (op)
    switch (op) {
        case 0x00: // R-type instruction
            controls->RegWrite = 1;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemtoReg = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 7; // Add
            break;
        case 0x08: //Add immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x02: // J-type instruction
            controls->RegWrite = 0;
            controls->MemRead = 0;
            controls->MemWrite = 0;
            controls->RegDst = 0;
            controls->Jump = 1;
            controls->Branch = 0;
            controls->MemtoReg = 0;
            controls->ALUSrc = 0;
            controls->ALUOp = 0; // Dont care
            break;
        case 0x23: // I-type instruction (lw)
            controls->RegWrite = 1;
            controls->MemRead = 1;
            controls->MemWrite = 0;
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemtoReg = 1;
            controls->ALUSrc = 1;
            controls->ALUOp = 0; // Add
            break;
        case 0x2B: // I-type instruction (sw)
            controls->RegWrite = 0;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemtoReg = 2;
            controls->ALUSrc = 1;
            controls->ALUOp = 0; // Add
            break;
        case 0x04: // I-type instruction (beq)
            controls->RegWrite = 0;
            controls->MemRead = 0;
            controls->MemWrite = 1;
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemtoReg = 2;
            controls->ALUSrc = 0;
            controls->ALUOp = 1;
            break;
        case 0x0A: //Set less than immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x0B: //Set less than immediate unsigned
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        case 0x0F: //Load upper immediate
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;
        default:
            // Halt condition: unknown opcode
            return 1;
    }

    // Return 0 if no halt condition occurs
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // Read data and assign to registers
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // 15th bit 1 | 0
    if(offset & (1 << 15))
        *extended_value = offset | 0xFFFF0000; // extends upper bits to 1
    else
        *extended_value = offset; // extends upper bits to 0
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    char ALUControl;
    unsigned B;

    B = ALUSrc ? extended_value : data2; // data2 | extended_value


    if (ALUOp < 0 || ALUOp > 7){
        return 1;
    }
    else if(ALUOp == 7){
        switch(funct) {
            case 0x20 : ALUControl = 0; break; // add
            case 0x22 : ALUControl = 1; break; // sub
            case 0x2A : ALUControl = 2; break; // slt signed
            case 0x2B : ALUControl = 3; break; // slt unsigned
            case 0x24 : ALUControl = 4; break; // and
            case 0x25 : ALUControl = 5; break; // or

            default: return 1; // halt
        }
    }else{
        ALUControl=ALUOp;
    }

    ALU(data1, B, ALUControl, ALUresult, Zero);

    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // Halt conditions
    // if (ALUresult % 4 != 0) return 1; // unaligned address
    // if ((ALUresult >> 2) >= 65536 >> 2) return 1; // out of bounds memory access

    // Load word
    if(MemRead){
        if((ALUresult % 4) ==0){
            *memdata = Mem[ALUresult >> 2];
        }else{
            return 1;
        }
    }
    // Store word
    if(MemWrite){
        if((ALUresult % 4) ==0){
            Mem[ALUresult >> 2] = data2;
        }else{
            return 1;
        }
    }
    return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    // If writing to register
    if(RegWrite==1){
        // If memory to register
        if(MemtoReg==1){
            // Register 2 or 3
            if(RegDst==1){
                Reg[r3]=memdata;
            }else{
                Reg[r2]=memdata;
            }
        // If ALU result to register
        }else{
            // Register 2 or 3
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

    // Increment by 4
    *PC += 4;

    // Update PC with concatenation of
    // • Top 4 bits of old PC
    // • 26-bit jump address
    // • 00
    if(Jump==1){
        *PC= (*PC & 0xF0000000) | (jsec<<2);
    }

    // If branch and there's a zero, add extended val
    if(Branch==1 && Zero==1){
        *PC += extended_value << 2;
    }
}
