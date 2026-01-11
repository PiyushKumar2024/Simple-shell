#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <dirent.h>
#include<sys/stat.h>

int cd(char* dest){
    if(!chdir(dest)){
        return 0;
    }
    return -1;
}

int ls(void){
    DIR *dir;
    struct dirent *entry;

    dir=opendir(".");

    if(dir==NULL){
        perror("error opening directory");
        return -1;
    }

    while((entry=readdir(dir))!=NULL){
        if(entry->d_type==DT_REG){
            printf("File: %s\n",entry->d_name);
        }
        else if(entry->d_type==DT_DIR){
            printf("Directory:%s\n",entry->d_name);
        }
    }

    if(closedir(dir)==-1){
        perror("Error closing the dir");
        return -1;
    }
    return 0;
}

int make_dir(char *dir){
    int status = mkdir(dir);
    if (status != 0) {
        perror("Error creating directory");
        return -1;
    }
    return 0;
}

int grep(char* pattern,char* fileName){
    FILE* fp=fopen(fileName,"r");
    char buff[1024];

    if(fp==NULL){
        perror("cant open file/failed to fond file");
        return -1;
    }

    while(fgets(buff,sizeof(buff),fp)!=NULL){
        if(strstr(buff,pattern)!=NULL){
            printf("%s",buff);
        }
    }

    fclose(fp);
    return 0;

}

int rm_file(char *file) {
    if (remove(file) == 0) return 0;
    perror("rm failed");
    return -1;
}

int rm_dir(char *dir) {
    if (rmdir(dir) == 0) return 0;
    perror("rmdir failed\n");
    return -1;
}

int main(int argc,char *argv[],char * envp[]){

    char cwd[1024];
    char input[1024];
    char *command;
    char *arg;

    while(1){

        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd failed");
            break;
        }

        printf("%s>",cwd);
        fgets(input,sizeof(input),stdin);

        command = strtok(input, " \n\t");   
        if(command==NULL) continue;

        arg = strtok(NULL, " \n\t");

        if(!strcmp(command,"cd")){
            if(!arg){
                printf("Enter a valid argument to cd");
            }else{
                if(cd(arg)==0) continue;
                else{
                    perror("failed to execute or dir not found");
                }
            }
        }

        else if(!strcmp(command,"ls")){
            if(ls()==0) continue;
            else{
                perror("failed to execute or dir not found");
                break;
            }
        }

        else if(!strcmp(command,"mkdir")){
            if(!arg){
                printf("Enter a argument to mkdir");
            }else{
                if(make_dir(arg)==0) continue;
                else{
                    perror("failed to make directory");
                    break;
                }
            }
        }

        else if(!strcmp(command,"grep")){
            char *file;
            file = strtok(NULL, " \n\t");
            if(!arg || !file){
                printf("Enter valid argument to grep");
            }else{
                if(grep(arg,file)==0) continue;
                else{
                    perror("failed to find pattern");
                    break;
                }
            }
        }

        else if(!strcmp(command,"rm")){
            if(!arg){
                printf("Enter valid argument to rm");
            }else{
                if(rm_file(arg)==0) continue;
                else{
                    perror("failed to delete pattern");
                    break;
                }
            }
        }

        else if(!strcmp(command,"rmdir")){
            if(!arg){
                printf("Enter valid argument to rm");
            }else{
                if(rm_dir(arg)==0) continue;
                else{
                    perror("failed to delete directory");
                    break;
                }
            }
        }

        else{
            perror("No command found");
            break;
        }

    }

    return 0;
    
}
