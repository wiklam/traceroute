#include "header.h"

void check_enter(int arg_num, char *table[]){
    
    if(arg_num != 2){
        fprintf(stderr, "Number of arguments incorrect\n");
        exit(EXIT_FAILURE);
    }
    
    char *ip = table[1];
    int number_count = 0, byte = 0, byte_count = 0;
    
    while(*ip != '\0'){
        
        if(*ip >= '0' && *ip <= '9'){
            if(number_count == 0)
                byte_count++;

            number_count++;
        }
        
        else{
            if(number_count == 0 || *ip != '.'){
                fprintf(stderr, "Invalid address\n");
                exit(EXIT_FAILURE);
            }
            
            byte = 0;
            number_count = 0;
        }

        byte *= 10;
        byte += (int) (*ip - '0');
        
        if(byte > 255){
            fprintf(stderr, "Invalid address\n");
            exit(EXIT_FAILURE);
        }

        ip++;
    }

    if(byte_count != 4){
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }
}