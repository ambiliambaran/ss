#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct OPTAB {
    char opcode[10];
    int length;
} optab[50];

struct SYMTAB {
    char label[10];
    int address;
} symtab[50];

int main() {
    FILE *input, *optabFile, *symtabFile, *intermediate, *output;
    char label[10], opcode[10], operand[20];
    int locctr, startAddr = 0, symCount = 0, optCount = 0, programLength;

    // Open all files
    input = fopen("input.txt", "r");
    optabFile = fopen("optab.txt", "r");
    symtabFile = fopen("symtab.txt", "w");
    intermediate = fopen("intermediate.txt", "w");
    output = fopen("output.txt", "w");

    if (!input || !optabFile || !symtabFile || !intermediate || !output) {
        printf("Error: File open failed.\n");
        exit(1);
    }

    // Read OPTAB into memory
    while (fscanf(optabFile, "%s %d", optab[optCount].opcode, &optab[optCount].length) != EOF)
        optCount++;
    fclose(optabFile);

    // Read first line from input
    fscanf(input, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        startAddr = atoi(operand);
        locctr = startAddr;
        fprintf(intermediate, "%s\t%s\t%s\n", label, opcode, operand);
        fprintf(output, "Program Name: %s\nStarting Address: %d\n\n", label, startAddr);
        fscanf(input, "%s %s %s", label, opcode, operand);
    } else {
        locctr = 0;
    }

    // Main PASS 1 Loop
    while (strcmp(opcode, "END") != 0) {
        if (label[0] != '-') { // not a comment
            int duplicate = 0;
            for (int i = 0; i < symCount; i++) {
                if (strcmp(symtab[i].label, label) == 0) {
                    printf("Error: Duplicate symbol %s\n", label);
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate && strcmp(label, "-") != 0) {
                strcpy(symtab[symCount].label, label);
                symtab[symCount].address = locctr;
                symCount++;
            }
        }

        // Search in OPTAB
        int found = 0;
        for (int i = 0; i < optCount; i++) {
            if (strcmp(optab[i].opcode, opcode) == 0) {
                locctr += optab[i].length;
                found = 1;
                break;
            }
        }

        // Handle assembler directives
        if (!found) {
            if (strcmp(opcode, "WORD") == 0)
                locctr += 3;
            else if (strcmp(opcode, "RESW") == 0)
                locctr += 3 * atoi(operand);
            else if (strcmp(opcode, "RESB") == 0)
                locctr += atoi(operand);
            else if (strcmp(opcode, "BYTE") == 0)
                locctr += strlen(operand) - 3; // e.g. C'EOF' = 3 bytes
            else
                printf("Warning: Invalid opcode %s ignored.\n", opcode);
        }

        fprintf(intermediate, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        fscanf(input, "%s %s %s", label, opcode, operand);
    }

    fprintf(intermediate, "%s\t%s\t%s\n", label, opcode, operand);
    fclose(intermediate);

    // Write SYMTAB
    fprintf(symtabFile, "LABEL\tADDRESS\n-----------------\n");
    for (int i = 0; i < symCount; i++)
        fprintf(symtabFile, "%s\t%d\n", symtab[i].label, symtab[i].address);
    fclose(symtabFile);

    // Calculate and write final output
    programLength = locctr - startAddr;
    fprintf(output, "PROGRAM LENGTH = %d\n\n", programLength);
    fprintf(output, "SYMBOL TABLE:\nLABEL\tADDRESS\n-----------------\n");
    for (int i = 0; i < symCount; i++)
        fprintf(output, "%s\t%d\n", symtab[i].label, symtab[i].address);
    fclose(output);

    fclose(input);

    printf("\nPASS 1 COMPLETED SUCCESSFULLY ✅\n");
    printf("Intermediate file : intermediate.txt\n");
    printf("Symbol Table file : symtab.txt\n");
    printf("Output file       : output.txt\n");
    printf("Program length    : %d\n", programLength);

    return 0;
}