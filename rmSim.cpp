// #include <bits/stdc++.h>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

#define REG_STR "registers"
#define REG_STR_LENGTH 9
#define NL '\n'
#define INC "inc"
#define DECJZ "decjz"
#define HALT "HALT"
#define NONE -1

#define NO_ERROR 0 
#define LABEL_ALREADY_DEFINED 1
#define UNDEFINED_LABEL 2

vector <long long> registers;
unordered_map <string, int> labels;

struct Instruction {
    string instType;
    int registerNr;
    string labelToJumpTo;
};
vector <Instruction> programLines;

char currentCh;
int nrRegisters;
int lineIndex;
int nrLines;

char errorMessage;

bool isAlpha(char ch);
bool isDigit(char ch);
void skipSP();
int getNr();
string getStr();

void regSpec() {
    currentCh = fgetc(stdin);
    getStr(); // read 'registers'
    
    while (currentCh != NL) {
        skipSP();
        registers.push_back(getNr());
    }    
}

int getRegister();
Instruction getLabInst();

void program() {
    while (currentCh != EOF) {
        currentCh = fgetc(stdin);
        programLines.push_back(getLabInst());
        ++lineIndex;
    }
    nrLines = lineIndex;
}

void execute() {
    int i = 0;
    bool halt = false;
    while (i < nrLines && !halt) {
        int regNr = programLines[i].registerNr;
        if(programLines[i].instType == INC) {
            ++registers[regNr];
            ++i;
        }
        else {
            if (!registers[regNr]) {  // if the value in the register is 0, then jump to the label
                string jumpTo = programLines[i].labelToJumpTo;
                if(jumpTo == HALT)
                    halt = true;
                else
                    i = labels[jumpTo];
            }
            else {
                --registers[regNr];
                ++i;
            }
        }
    }
}

void output() {
    for (int i = 0; i < registers.size(); ++i)
        printf("%lld ", registers[i]);
}

Instruction getLabInst() {
    string labelOrInstr = getStr();
    skipSP();

    string instType;
    if (currentCh == ':') { // then the string read above is a label
        string label = labelOrInstr;

        if(labels.count(label))
            errorMessage = LABEL_ALREADY_DEFINED;
        else
            labels[label] = lineIndex;

        currentCh = fgetc(stdin);
        skipSP();
        instType = getStr();
    }
    else {
        instType = labelOrInstr;
    }

    skipSP();
    int registerNr = getRegister();

    string labelToJumpTo;
    if (instType == DECJZ) {
        skipSP();
        labelToJumpTo = getStr();
    }
    else {
        labelToJumpTo = NONE;
    }

    Instruction labInst;
    labInst.instType = instType;
    labInst.registerNr = registerNr;
    labInst.labelToJumpTo = labelToJumpTo;

    return labInst;
}

void searchUndefinedLabels() {
    bool anyUndefLabels = false;
    for (Instruction instr : programLines) {
        string label = instr.labelToJumpTo;
        if(label != HALT && !labels.count(label)) // if we need to jump to an unexistent label
            anyUndefLabels = true;
    }
    if(anyUndefLabels) 
        errorMessage = UNDEFINED_LABEL;
}

void errorCheck() {
    searchUndefinedLabels();
    switch(errorMessage) {
        case LABEL_ALREADY_DEFINED: throw invalid_argument("Error: A label cannot be defined twice"); break;
        case UNDEFINED_LABEL: throw invalid_argument("Error: The program cannot jump to an undefined label"); break;
    }
}

int getRegister() {
    currentCh = fgetc(stdin); // move from 'r' to the first digit of the nr
    int regIndex = getNr();
    
    if (regIndex > registers.size() - 1) 
        for (int i = registers.size(); i <= regIndex; ++i)
            registers.push_back(0);
    
    return regIndex;
}

int getNr() {
    int nr = 0;
    while (isDigit(currentCh)) {
        nr = nr * 10 + (currentCh - '0');            
        currentCh = fgetc(stdin);
    }
        
    return nr;
}

string getStr() {
    vector <char> str;

    while (isAlpha(currentCh) || isDigit(currentCh)) {
        str.push_back(currentCh);
        currentCh = fgetc(stdin);
    }

    string newStr (str.begin(), str.end());
    return newStr;
}

bool isAlpha(char ch) {
    return (ch >= 'A' && ch <= 'z');
}

bool isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

void skipSP() {
    while (currentCh == ' ' || currentCh == '\t')
        currentCh = fgetc(stdin);
}

int main() {
    
    regSpec();
    program();
    errorCheck();
    execute();
    printf("\n Output:");
    output();

    // Testing

    printf("\n");

    for (int i = 0; i < registers.size(); ++i)
        printf("%lld ", registers[i]);

    printf("\n");

    for (int i = 0; i < programLines.size(); i++) {
        printf("line = %d, instr = %s, reg = %d, label = %s\n",
        i, programLines[i].instType.c_str(), programLines[i].registerNr, programLines[i].labelToJumpTo.c_str()); 
    }

    printf("\n");
    unordered_map<string, int>::iterator it;
    for (it  = labels.begin(); it != labels.end(); ++it) {
        string label = it->first;
        int line = it->second;
        printf("%s %d\n", label.c_str(), line);
    }
    printf("nr of instruction lines is %d", nrLines);
    return 0;
}