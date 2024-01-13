//windows specific
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//#include <options.h>
#include <fileapi.h>
#include <string.h>
#include "../include/configure.h"

void reset_buffer(char *buffer, int size){
    char c = buffer[0];
    int i = 0;
    while (c != '\0' && i < size){
        *buffer = '\0';
        buffer += sizeof(char);
        c = *buffer;
        i++;
    }
}

const char *create_config_folder(){
    //either .exe is in bin folder,
    //or ran straight...
    const char *path = ".\\.config";
    if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) {
        const char *dev_path = "..\\.config";
        if (GetFileAttributesA(dev_path) == INVALID_FILE_ATTRIBUTES) {
            if (CreateDirectoryA(path, NULL)) {
                puts("Created .config folder");
                if (!SetFileAttributesA(path, FILE_ATTRIBUTE_HIDDEN)) puts("Could not make folder hidden...");
                return path;
            }
            else{
                puts("Error in trying to create a .config dir. Aborting.");
                exit(1);
            }
        }
        else{
            puts("Dev path exists");
            return dev_path;
        }
    }
    else{
        puts("Path exists");
        return path;
    } 
}

High_Scores check_score(){
    const char *path = create_config_folder();
    char path_buffer[strlen(path) + 8];
    strcpy(path_buffer, path);
    strcat(path_buffer, "\\.score");
    FILE *score = fopen(path_buffer, "r");
    if (score == NULL){
        puts("Could not open .score file. Creating a .score file");
        fclose(score);
        score = fopen(path_buffer, "a+");
        High_Scores scores = {0, 0, 0};
        const char *data = "high-scores\n\t-easy: %d;\n\t-norm: %d;\n\t-hard: %d;";
        if (fprintf(score, data, scores.easy, scores.norm, scores.hard) < 0){
            puts("Error writing a .score file");
            fclose(score);
            exit(1);
        }
        fclose(score);
        return scores;
    }
    else{
        char line[100]; 
        const char *first = "high-scores\n";
        const char *easy = "\t-easy: ";
        const char *norm = "\t-norm: ";
        const char *hard = "\t-hard: ";
        fgets(line, 100, score);
        if (strcmp(line, first)){
            puts("Syntax error in .score file!");
            fclose(score);
            exit(1);
        }
        High_Scores scores;
        char data[50];
        reset_buffer(data, 50);
        while(!feof(score)){
            fgets(line, 50, score);
            char *tmp;
            if (!strncmp(line, easy, strlen(easy))){
                //go to ith char of line, where i is char after easy
                tmp = line + strlen(easy) * sizeof(char);
                //char before the \n
                if (strlen(tmp) > 2 && tmp[strlen(tmp) - 2] == ';'){
                    strncpy(data, tmp, strlen(tmp) - 2);
                    for (int i = 0; i < strlen(data); i++){
                        if (!isdigit(data[i])){
                            printf("Syntax error: cannot have a non digit as a score!\n%s\n", line);
                            exit(1);
                        }
                    }
                    scores.easy = atoi(data);
                }
                else if (strlen(tmp) > 2 && tmp[strlen(tmp) - 1] != ';'){
                    puts("Syntax error in easy row");
                    exit(1);
                }
                else{
                    puts("No high score found for easy... Aborting");
                    fclose(score);
                    exit(1);
                }
            }
            else if (!strncmp(line, norm, strlen(norm))){
                tmp = line + strlen(norm) * sizeof(char);
                //char before the \n
                if (strlen(tmp) > 2 && tmp[strlen(tmp) - 2] == ';'){
                    strncpy(data, tmp, strlen(tmp) - 2);
                    for (int i = 0; i < strlen(data); i++){
                        if (!isdigit(data[i])){
                            printf("Syntax error: cannot have a non digit as a score!\n%s\n", line);
                            exit(1);
                        }
                    }
                    scores.norm = atoi(data);
                }
                else if (strlen(tmp) > 2 && tmp[strlen(tmp) - 1] != ';'){
                    puts("Syntax error in norm row");
                    exit(1);
                }
                else{
                    puts("No high score found for norm... Aborting");
                    fclose(score);
                    exit(1);
                }
            }
            else if (!strncmp(line, hard, strlen(hard))){
                tmp = line + strlen(hard) * sizeof(char);
                //no \n in hard
                if (strlen(tmp) > 1 && tmp[strlen(tmp) - 1] == ';'){
                    strncpy(data, tmp, strlen(tmp) - 1);
                    for (int i = 0; i < strlen(data); i++){
                        if (!isdigit(data[i])){
                            printf("Syntax error: cannot have a non digit as a score!\n%s\n", line);
                            exit(1);
                        }
                    }
                    scores.hard = atoi(data);
                }
                else if (strlen(tmp) > 1 && tmp[strlen(tmp) - 1] != ';'){
                    puts("Syntax error in hard row");
                    exit(1);
                }
                else{
                    puts("No high score found for hard... Aborting");
                    fclose(score);
                    exit(1);
                }
            }
            else{
                puts("Syntax error inside high-scores block!");
                printf("%d, %d, %d", scores.easy, scores.norm, scores.hard);
                fclose(score);
                exit(1);
            }
        }
        return scores;
    }
}

int save_highscores(High_Scores *old, High_Scores *update, const char *pathname){
    FILE *file = fopen(pathname, "r+");
    if (file == NULL){
        puts("File doesnt exist. Aborting");
        return 1;
    }
    fpos_t prev;
    fpos_t cur;
    char line[100];
    fgets(line, 100, file);
    fgetpos(file, &prev);
    fgets(line, 100, file);
    fgetpos(file, &cur);
    fsetpos(file, &prev);
    if (update->easy > old->easy){
        const char *easy = "\t-easy: ";
        if (!strncmp(easy, line, strlen(easy))){
            //add error handling
            fprintf(file, "\t-easy: %d;", update->easy);
            puts("Printing on easy line");
            prev = cur;
            fsetpos(file, &cur);
            fgets(line, 100, file);
        }
        else{
            printf("Syntax error: %s\n", line);
            return 2;
        }
    }
    else{
        fgets(line, 100, file);
        prev = cur;
        fgets(line, 100, file);
    }
    fgetpos(file, &cur);
    fsetpos(file, &prev);
    if (update->norm > old->norm){
        const char *norm = "\t-norm: ";
        if (!strncmp(norm, line, strlen(norm))){
            const char *w_norm = "\t-norm: %d;";
            //add error handling
            fprintf(file, w_norm, update->norm);
            puts("Printing on norm line");
            prev = cur;
            fsetpos(file, &cur);
            fgets(line, 100, file);
        }
        else{
            printf("Syntax error: %s\n", line);
            return 2;
        }
    }
    else{
        fgets(line, 100, file);
        prev = cur;
        fgets(line, 100, file);
    }
    fgetpos(file, &cur);
    fsetpos(file, &prev);
    if (update->hard > old->hard){
        const char *hard = "\t-hard: ";
        if (!strncmp(hard, line, strlen(hard))){
            const char *w_hard = "\t-hard: %d;";
            //add error handling
            fprintf(file, w_hard, update->hard);
        }
        else{
            printf("Syntax error: %s\n", line);
            return 2;
        }
    }
    fclose(file);
    return 0;
}
