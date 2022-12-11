// This program's purpose is that
// is convert to assembly to binary code.

#include <stdio.h>
#include <string.h>
#pragma warning(disable:4996)

char assembly[256]; // Input assembly code
char binary[32]; // output binary code
char del[] = " (),$"; // delimiter
char *token; // assembly token
char op[6]; // opcode
char rs[5]; // rs
char rt[5]; // rt
char rd[5]; // rd
char temp[4]; // temp register value for dec to binary
char imme[26]; // Immediate field for I-type
char format; // format type in MIPS

void checkOp(char*); // check opcode or funct
char checkType(); // check format type (R, I, J)
void checkRType(char*); // R type format scenario
void checkIType(char*); // I type format scenario
void checkJType(char*); // J type format scenario
int convertHexToDec(char*); // convert to hexadecimal to decimal
void convertDecToBin(int, int); // convert to hexadecimal to binary
void clear(); // clear op, registers, imme, format

int main() {
    printf("\n\n\n=====================================================================\n");
    printf("\t\t Welcome! MIPS Instruction Converter\n");
    printf("\t\t if you want exit, input exit\n\n\n");

    while(1) {
        printf("Assembly: "); // Input Assembly code
        scanf(" %[^\n]", assembly);

        if(!strcmp(assembly, "exit")) break; // if input exit, break

        token = strtok(assembly, del);
        checkOp(token);

        printf("binary code: %s\n\n", binary);
        clear();
    }

    return 0;
}

void checkOp(char *token) {
    if(!strcmp(token, "j")) {   // J-type format
        strcpy(binary, "000010");
    }
    else if(!strcmp(token, "addi")) {   // I-type format
        strcpy(binary, "001000");
    } else if(!strcmp(token, "andi")) {
        strcpy(binary, "001100");
    } else if(!strcmp(token, "lw")) {
        strcpy(binary, "100011");
    } else if(!strcmp(token, "sw")) {
        strcpy(binary, "101011");
    } else if(!strcmp(token, "beq")) {
        strcpy(binary, "000100");
    } else if(!strcmp(token, "bne")) {
        strcpy(binary, "000101");
    } else { // R-type format
        if(!strcmp(token, "add")) {
            strcpy(op, "100000");
        } else if(!strcmp(token, "and")) {
            strcpy(op, "100100");
        } else if(!strcmp(token, "nor")) {
            strcpy(op, "100111");
        } else if(!strcmp(token, "or")) {
            strcpy(op, "100101");
        } else if(!strcmp(token, "sub")) {
            strcpy(op, "100010");
        } else if(!strcmp(token, "xor")) {
            strcpy(op, "100110");
        }
        
        strcpy(binary, "000000");
    }

    // check format type to opcode
    format = checkType();
    token = strtok(NULL, del);

    // accoding to format type, method called
    switch(format) {
        case 'R': checkRType(token); break;
        case 'I': checkIType(token); break;
        default: checkJType(token); break;
    }
}

char checkType() {
    if(!strcmp(binary, "000000")) // R-type format
        return 'R';
    else if(!strcmp(binary, "000010")) // J-type format
        return 'J';
    else // I-type format
        return 'I';
}

void checkRType(char *token) {
    // rd register
    convertDecToBin(token[0] - '0', 1);
    strcpy(rd, temp);

    // rs register
    token = strtok(NULL, del);
    convertDecToBin(token[0] - '0', 1);
    strcpy(rs, temp);

    // rt register
    token = strtok(NULL, del);
    convertDecToBin(token[0] - '0', 1);
    strcpy(rt, temp);

    strcat(binary, rs); // save rs code to binary code
    strcat(binary, rt); // save rt code to binary code
    strcat(binary, rd); // save rd code to binary code
    strcat(binary, "00000"); // save shamt to binary code
    strcat(binary, op); // save op code to binary code
}

void checkIType(char *token) {
    if(!strcmp(binary, "001000") || !strcmp(binary, "001100")) { // addi, andi
        // rt
        convertDecToBin(token[0] - '0', 1);
        strcpy(rt, temp);

        // rs
        token = strtok(NULL, del);
        convertDecToBin(token[0] - '0', 1);
        strcpy(rs, temp);

        // imme
        token = strtok(NULL, del);
        int dec = convertHexToDec(token);
        convertDecToBin(dec, 0);

        strcat(binary, rs); // save rt field to binary code
        strcat(binary, rt); // save rs field to binary code
    }
    else if(!strcmp(binary, "100011") || !strcmp(binary, "101011")) { // lw, sw
        // rt
        convertDecToBin(token[0] - '0', 1);
        strcpy(rt, temp);

        // imme
        token = strtok(NULL, del);
        int dec = convertHexToDec(token);
        convertDecToBin(dec, 0);

        // rs
        token = strtok(NULL, del);
        convertDecToBin(token[0] - '0', 1);
        strcpy(rs, temp);

        strcat(binary, rs); // save rt field to binary code
        strcat(binary, rt); // save rs field to binary code
    } else { // bne, beq 
        // rs
        convertDecToBin(token[0] - '0', 1);
        strcpy(rs, temp);

        // rt
        token = strtok(NULL, del);
        convertDecToBin(token[0] - '0', 1);
        strcpy(rt, temp);

        // label
        token = strtok(NULL, del);
        int dec = convertHexToDec(token);
        convertDecToBin(dec, 0);

        strcat(binary, rs); // save rt field to binary code
        strcat(binary, rt); // save rs field to binary code
    }

    strcat(binary, imme); // save immediate field to binary code
}

void checkJType(char *token) {
    int dec = convertHexToDec(token);
    convertDecToBin(dec, 0);

    strcat(binary, imme);
}

int convertHexToDec(char *token) {
    int dec = 0, base = 1;

    for(int i = strlen(token)-1; token[i] != 'x'; i--) {
        if(token[i] >= '0' && token[i] <= '9')
            dec += (token[i] - '0') * base;
        else if(token[i] >= 'A' && token[i] <= 'F')
            dec += (token[i] - 55) * base;
        else if(token[i] >= 'a' && token[i] <= 'f')
            dec += (token[i] - 87) * base;
        base *= 16;
    }

    return dec;
}

void convertDecToBin(int dec, int reg) {
    int field = 4;

    if(reg) {
        for(int i = field; i >= 0; i--)
            temp[field - i] = ((dec >> i) & 1) + '0';
    }
    else {
        if(format == 'J') field = 25;
        else field = 15;

        for(int i = field; i >= 0; i--)
            imme[field - i] = ((dec >> i) & 1) + '0';
    }
}

void clear() {
    op[0] = '\0';
    rs[0] = '\0';
    rt[0] = '\0';
    rd[0] = '\0';
    temp[0] = '\0';
    imme[16] = '\0';
}