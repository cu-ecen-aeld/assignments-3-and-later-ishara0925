#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>

int main(int argc, char *argv[]){

    openlog(NULL,0,LOG_USER);
    if(argc != 3)
        syslog(LOG_ERR,"Invalid Num of Arguments: %d Expected 2 arguments",argc);
    else{
        syslog(LOG_DEBUG,"Writing %s to %s",argv[2],argv[1]);
    }
    char* filename = argv[1];
    char* text = argv[2];

    FILE* fptr =  fopen(filename,"w");

    if(NULL == fptr){
        syslog(LOG_ERR,"Error Opening File");
        return 1;
    }
    else{
        fprintf(fptr,"%s",text);
    }
    fclose(fptr);
    return 0;
}