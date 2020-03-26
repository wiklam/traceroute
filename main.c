#include "header.h"

int main(int argc, char *argv[]){
    if(!check_enter(argc, argv))
        error_handle("Invalid IP addres: %s\n", argv[1]);
    traceroute(argv[1]);
    return 0;
}