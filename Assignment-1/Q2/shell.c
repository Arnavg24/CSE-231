#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <limits.h>
#include <ctype.h>

typedef struct {
    bool first_option;
    bool second_option;
    bool error;
    char dir_name[1000];
} cmdargs;

typedef struct {
    bool first_option;
    bool second_option;
    bool error;
    char file_names[2][1000];
} wordargs;

cmdargs parsecmddir(const char *input) {
    cmdargs cmdInfo = {false, false, false, ""}; 
    int input_len = strlen(input);
    char temp_input[input_len + 1];
    strcpy(temp_input, input);
    
    char *token = strtok(temp_input, " ");
    
    while (token != NULL) {
        if (strcmp(token, "-r") == 0) {
            cmdInfo.first_option = true;
        } else if (strcmp(token, "-v") == 0) {
            cmdInfo.second_option = true;
        } else {
            if (cmdInfo.dir_name[0] == '\0') {
                strcpy(cmdInfo.dir_name, token);
            } else {
                cmdInfo.error = true;
            }
        }
        token = strtok(NULL, " ");
    }
    
    if (cmdInfo.dir_name[0] == '\0') {
        cmdInfo.error = true;
    }
    
    return cmdInfo;
}

cmdargs parsecmddate(const char *input) {
    cmdargs cmdInfo = {false, false, false, ""}; 
    int input_len = strlen(input);
    char temp_input[input_len + 1];
    strcpy(temp_input, input);
    
    char *token = strtok(temp_input, " ");
    
    while (token != NULL) {
        if (strcmp(token, "-d") == 0) {
            cmdInfo.first_option = true;
        } else if (strcmp(token, "-R") == 0) {
            cmdInfo.second_option = true;
        } else {
            if (cmdInfo.dir_name[0] == '\0') {
                strcpy(cmdInfo.dir_name, token);
            } else {
                cmdInfo.error = true;
            }
        }
        token = strtok(NULL, " ");
    }
    
    if (cmdInfo.dir_name[0] == '\0') {
        cmdInfo.error = true;
    }
    
    return cmdInfo;
}

wordargs parsecmdword(const char *input) {
    wordargs cmdInfo = {false, false, false, {{'\0'}, {'\0'}}};
    
    int input_len = strlen(input);
    char temp_input[input_len + 1];
    strcpy(temp_input, input);
    
    char *token = strtok(temp_input, " ");
    
    while (token != NULL) {
        if (strcmp(token, "-n") == 0) {
            cmdInfo.first_option = true;
        } else if (strcmp(token, "-d") == 0) {
            cmdInfo.second_option = true;
        } else {
            if (cmdInfo.file_names[0][0] == '\0') {
                strcpy(cmdInfo.file_names[0], token);
            } else if (cmdInfo.file_names[1][0] == '\0') {
                strcpy(cmdInfo.file_names[1], token);
            } else {
                cmdInfo.error = true;
            }
        }
        token = strtok(NULL, " ");
    }
    
    if (cmdInfo.second_option && (cmdInfo.file_names[0][0] == '\0' || cmdInfo.file_names[1][0] == '\0')) { //takimkc
        cmdInfo.error = true; // Need two file names for -d option
    }
    
    return cmdInfo;
}


int countWords(const char *filename, bool removeNewlines) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    int wordCount = 0;
    int currentChar;
    bool inWord = false;
    
    while ((currentChar = fgetc(file)) != EOF) {
        if (removeNewlines && currentChar == '\n') {
            continue;
        }

        if (isspace(currentChar)) {
            inWord = false;
        } else {
            if (!inWord) {
                inWord = true;
                wordCount++;
            }
        }
    }
    
    fclose(file);
    return wordCount;
}

int main(int argc, char *argv[]) {
    char cwdorig[1024];
    getcwd(cwdorig, sizeof(cwdorig));
    char cmd[1000];
    do {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s:>",cwd);
        scanf("%s", cmd);
        if (strcmp(cmd, "dir") == 0) {
            scanf(" %[^\n]", cmd); // scanf which reads the rest of the line but removes whitespaces
            cmdargs argsdir = parsecmddir(cmd);
            if (access(argsdir.dir_name, F_OK) == 0) {
                printf("Directory '%s' already exists.\n", argsdir.dir_name);
                if(!argsdir.first_option){
                    continue;}}

            pid_t piddir = fork();
            if (piddir == 0) {
                if (argsdir.error) {
                    printf("invalid arguments\n");
                } else if (argsdir.first_option) {
                    if (argsdir.second_option) {
                        printf("removing duplicate directories if any present.\n");
                    }
                    pid_t pid_rm = fork();
                    if (pid_rm == 0) {
                        char *argsrm[] = {"rm", "-rf", argsdir.dir_name, NULL};
                        execvp("rm", argsrm);
                    }
                    wait(NULL);
                }
                if (argsdir.second_option) {
                    printf("making new directory.\n");
                }
                char *args[] = {"mkdir", argsdir.dir_name, NULL};
                execvp("mkdir", args);
            } else if (piddir < 0) {
                perror("Fork error");
                return 1;
            } else {
                wait(NULL);
                chdir(argsdir.dir_name);
            }
        }
        if (strcmp(cmd, "date") == 0) {
            scanf(" %[^\n]", cmd); // scanf which reads the rest of the line but removes whitespaces
            cmdargs argsdate = parsecmddate(cmd);
            pid_t piddate = fork();
            if (piddate == 0) {
                if (argsdate.error) {
                    printf("invalid arguments\n");
                } else if (argsdate.first_option) {
                    // Print after piping through date without formatting argument UPDATE pass string given through date
                    char cwdtemp[1024];
                    strcpy(cwdtemp,cwdorig);
                    strcat(cwdtemp,"/lswithdate.sh");           
                    char *argsbsh[] = {cwdtemp,argsdate.dir_name,NULL};
                    execvp(cwdtemp,argsbsh);
                } else if (argsdate.second_option) {
                    // Print after piping through date -R for rfc5322 format
                    char cwdtemp[1024];
                    strcpy(cwdtemp,cwdorig);
                    strcat(cwdtemp,"/lswithdateR.sh");                 
                    char *argsbsh[] = {cwdtemp,argsdate.dir_name,NULL};
                    execvp(cwdtemp, argsbsh);
                } else {
                    // Implement ls -l extracted date without any other formatting as default output
                    char cwdtemp[1024];
                    strcpy(cwdtemp,cwdorig);   
                    strcat(cwdtemp,"/lswithoutdate.sh");                 
                    char *argsbsh[] = {cwdtemp,argsdate.dir_name,NULL};
                    execvp(cwdtemp,argsbsh);
                }
            } else if (piddate < 0) {
                perror("Fork error");
                return 1;
            } else {
                wait(NULL);
            }
        }
        if (strcmp(cmd, "word") == 0) {
            scanf(" %[^\n]", cmd); // scanf which reads the rest of the line but removes whitespaces
            wordargs argsword = parsecmdword(cmd);
            // pid_t pidword = fork();
            // if (pidword == 0) {
            if (argsword.error) {
                printf("invalid arguments\n");
            } 
            // else if (argsword.first_option&&argsword.second_option)//
            // {
                //both args
                // char cwdtemp[1024];
                // strcpy(cwdtemp,cwd);
                // strcat(cwdtemp,"/wcdiffnless.sh");
                // char *argsbsh[] = {cwdtemp,argsword.file_names[0],argsword.file_names[1],NULL};
                // execvp(cwdtemp,argsbsh);
            // }
            // else if (argsword.first_option)
            // {
                // char cwdtemp[1024];
                // strcpy(cwdtemp,cwd);                    
                // strcat(cwdtemp,"/wcnless.sh");
                // char *argsbsh[] = {cwdtemp,argsword.file_names[0],NULL};
                // execvp(cwdtemp,argsbsh);                    
            // }
            else if (argsword.second_option)
            {
                // char cwdtemp[1024];
                // strcpy(cwdtemp,cwd);
                // strcat(cwdtemp,"/wcdiff.sh");                   
                // char *argsbsh[] = {cwdtemp,argsword.file_names[0],argsword.file_names[1],NULL};
                // execvp(cwdtemp,argsbsh);
                int wordsCountdiff=(countWords(argsword.file_names[0],argsword.first_option)-countWords(argsword.file_names[1],argsword.first_option));
                printf("Difference in number of words: %d\n", wordsCountdiff);
            }
            else
            {
                // char *argsbsh[] = {"wc","-w",argsword.file_names[0],NULL};
                // execvp("wc",argsbsh);
                int huh = countWords(argsword.file_names[0],argsword.first_option);
                printf("Number of words: %d\n",huh);
            }
        }
    } while (strcmp(cmd, "exit") != 0);
    return 0;
}
