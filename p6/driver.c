/**
@file driver
@author Ethan Browne
Top level of program. Contains main method
*/
#include "input.h"
#include "value.h"
#include "map.h"


/**
The main method
@return whether the program was run successfully
*/
int main()
{
    Map* map = makeMap();
    char *line = readLine(NULL);
    printf("cmd> ");

    while (line != NULL){
        char command[strlen(line) + 1];
        memset( command, '\0', strlen(line) + 1);

        int offset = 0;
        int n = 0;
        printf("%s\n", line);
        if (sscanf(line, "%s%n", command, &n) == 1){
            offset += n;
            if (strcmp(command, "set") == 0) {
                char key[strlen(line + offset) + 1];
                memset( key, '\0', strlen(line + offset) + 1);
                if (sscanf(line + offset, "%s%n", key, &n) == 1) { // Get the Key
                    bool validKey = true;
                    for (int i = 0; key[i]; i ++) {
                        if (key[i] > '~' || '!' > key[i]) {
                            validKey = false;
                        }
                    }
                    if (validKey) {
                        offset += n;
                        Value *val = parseInteger(line + offset);
                        if (val == NULL){
                            val = parseDouble(line + offset);
                            if (val == NULL){
                                val = parseString(line + offset);
                            }
                        }
                        mapSet(map, key, val);
                    } else {
                        printf("invalid\n");
                    }
                } else {
                    printf("invalid\n");
                }
            } else if (strcmp(command, "get") == 0) {
                char key[strlen(line + offset) + 1];
                memset( key, '\0', strlen(line + offset) + 1);
                if (sscanf(line + offset, "%s%n", key, &n) == 1) { // Get the Key
                    offset += n;
                    if (sscanf(line + offset, "%s%n", key, &n) != 1){
                        Value *val = mapGet(map, key);
                        if (val == NULL) {
                            printf("invalid\n");
                        } else {
                            char *s = val->toString(val);
                            printf("%s\n", s);
                            free(s);
                        }
                    } else {
                        printf("invalid\n");
                    }
                } else {
                    printf("invalid\n");
                }
            } else if (strcmp(command, "remove") == 0) {
                char key[strlen(line + offset) + 1];
                memset( key, '\0', strlen(line + offset) + 1);
                if (sscanf(line + offset, "%s%n", key, &n) == 1) { // Get the Key
                    if (!mapRemove(map, key)) {
                        printf("invalid\n");
                    }
                } else {
                    printf("invalid\n");
                }
            } else if (strcmp(command, "plus") == 0) {
                char key[strlen(line + offset) + 1];
                memset( key, '\0', strlen(line + offset) + 1);
                if (sscanf(line + offset, "%s%n", key, &n) == 1) { // Get the Key
                    Value *val = mapGet(map, key);
                    if (val == NULL) {
                        printf("invalid\n");
                    } else {
                        offset += n;
                        Value *newVal = parseInteger(line + offset);
                        bool goodVal = true;
                        if (newVal == NULL){
                            newVal = parseDouble(line + offset);
                            if (newVal == NULL){
                                newVal = parseString(line + offset);
                                 if (newVal == NULL){
                                    printf("invalid\n");
                                    goodVal = false;
                                }
                            }
                        }
                        if (goodVal) {
                            if (!val->plus(val, newVal)){
                                printf("invalid\n");
                            }
                            newVal->destroy(newVal);
                        }
                    }
                }
            } else if (strcmp(command, "size") == 0) {
                printf("%d\n", mapSize(map));
            } else if (strcmp(command, "quit") == 0) {
                free(line);
                freeMap(map);
                return EXIT_SUCCESS;
            } else {
                printf("invalid\n");
            }
        }
        free(line);
        line = readLine(NULL);
        printf("\ncmd> ");
    }
    free(line);
    freeMap(map);
    return EXIT_SUCCESS;
}