#include <iostream>
#include "ShellIj.h"

int main(int argc, char* argv[]) {
    system("chcp 65001");

    if (argc == 2) {

        int PID = atoi(argv[1]);
        ShellIj::Init(PID);
        printf("[+] 已注入进程 %d \n", PID);
    }
    return 0;
}
