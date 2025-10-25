#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct OPTAB {
    char opcode[10];
    char code[10];
} optab[50];

struct SYMTAB {
    char label[10];
    int address;
} symtab[50];

int main() {
    FILE *intermediate, *optabFile, *symtabFile, *objprog, *listing;
    char label[10], opcode[10], operand[20];
    char objcode[20], symbol[10], mnemonic[10], addressStr[10];
    int startAddr = 0, locctr = 0, progLen = 0;
    int symCount = 0, optCount = 0;
    char name[10];

    // Open all files
    intermediate = fopen("intermediate.txt", "r");
    optabFile = fopen("optab.txt", "r");
    symtabFile = fopen("symtab.txt", "r");
    objprog = fopen("objectprogram.txt", "w");
    listing = fopen("listing.txt", "w");

    if (!intermediate || !optabFile || !symtabFile || !objprog || !listing) {
        printf("Error opening files.\n");
        exit(1);
    }

    // Load OPTAB into memory
    while (fscanf(optabFile, "%s %s", optab[optCount].opcode, optab[optCount].code) != EOF)
        optCount++;
    fclose(optabFile);

    // Load SYMTAB into memory (skip header line if present)
    char dummy1[20], dummy2[20];
    fscanf(symtabFile, "%s %s", dummy1, dummy2); // skip header
    fscanf(symtabFile, "%s %s", dummy1, dummy2); // skip line of ----
    while (fscanf(symtabFile, "%s %d", symtab[symCount].label, &symtab[symCount].address) != EOF)
        symCount++;
    fclose(symtabFile);

    // Read first line of intermediate file
    fscanf(intermediate, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        strcpy(name, label);
        startAddr = atoi(operand);
        fprintf(listing, "%s\t%s\t%s\n", label, opcode, operand);
        fscanf(intermediate, "%d %s %s %s", &locctr, label, opcode, operand);
    }

    // Write header record to object program
    fprintf(objprog, "H^%-6s^%06d", name, startAddr);

    // We will find program length from last LOCCTR
    int lastAddr = 0;
    FILE *temp = fopen("intermediate.txt", "r");
    char l[10], o[10], op[10];
    int loc;
    while (fscanf(temp, "%d %s %s %s", &loc, l, o, op) != EOF)
        lastAddr = loc;
    fclose(temp);
    progLen = lastAddr - startAddr;
    fprintf(objprog, "^%06d\n", progLen);

    // Initialize text record
    fprintf(objprog, "T^%06d^", locctr);
    int textLenPos = ftell(objprog);
    fprintf(objprog, "00"); // placeholder
    int textStart = locctr;
    int textLen = 0;

    // Process lines until END
    while (strcmp(opcode, "END") != 0) {
        if (strcmp(opcode, "RESW") != 0 && strcmp(opcode, "RESB") != 0 && strcmp(opcode, "END") != 0) {
            int found = 0;
            for (int i = 0; i < optCount; i++) {
                if (strcmp(optab[i].opcode, opcode) == 0) {
                    found = 1;
                    int operandAddr = 0;
                    int symFound = 0;
                    if (strcmp(operand, "-") != 0) {
                        for (int j = 0; j < symCount; j++) {
                            if (strcmp(symtab[j].label, operand) == 0) {
                                operandAddr = symtab[j].address;
                                symFound = 1;
                                break;
                            }
                        }
                    }
                    if (!symFound && strcmp(operand, "-") != 0)
                        printf("Error: Undefined symbol %s\n", operand);

                    sprintf(objcode, "%s%04d", optab[i].code, operandAddr);
                    fprintf(listing, "%04d\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, objcode);

                    if (textLen + 3 > 30) {
                        fseek(objprog, textLenPos, SEEK_SET);
                        fprintf(objprog, "%02X", textLen);
                        fseek(objprog, 0, SEEK_END);
                        fprintf(objprog, "\nT^%06d^", locctr);
                        textLenPos = ftell(objprog);
                        fprintf(objprog, "00");
                        textLen = 0;
                    }

                    fprintf(objprog, "^%s", objcode);
                    textLen += 3;
                    break;
                }
            }

            if (!found) {
                // Handle BYTE and WORD
                if (strcmp(opcode, "BYTE") == 0) {
                    char value[20] = "";
                    if (operand[0] == 'C') {
                        for (int k = 2; k < strlen(operand) - 1; k++) {
                            char hex[3];
                            sprintf(hex, "%02X", operand[k]);
                            strcat(value, hex);
                        }
                    } else if (operand[0] == 'X') {
                        strncpy(value, operand + 2, strlen(operand) - 3);
                        value[strlen(operand) - 3] = '\0';
                    }
                    fprintf(listing, "%04d\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, value);
                    fprintf(objprog, "^%s", value);
                    textLen += strlen(value) / 2;
                } else if (strcmp(opcode, "WORD") == 0) {
                    sprintf(objcode, "%06d", atoi(operand));
                    fprintf(listing, "%04d\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, objcode);
                    fprintf(objprog, "^%s", objcode);
                    textLen += 3;
                }
            }
        } else {
            fprintf(listing, "%04d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        }

        fscanf(intermediate, "%d %s %s %s", &locctr, label, opcode, operand);
    }

    // Close final text record
    fseek(objprog, textLenPos, SEEK_SET);
    fprintf(objprog, "%02X", textLen);
    fseek(objprog, 0, SEEK_END);
    fprintf(objprog, "\n");

    // Write End record
    fprintf(objprog, "E^%06d\n", startAddr);
    fprintf(listing, "%s\t%s\t%s\n", label, opcode, operand);

    fclose(intermediate);
    fclose(objprog);
    fclose(listing);

    printf("\nPASS 2 COMPLETED SUCCESSFULLY ✅\n");
    printf("Object program  : objectprogram.txt\n");
    printf("Listing file    : listing.txt\n");

    return 0;
}