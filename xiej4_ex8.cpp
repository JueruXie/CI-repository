/*
 * xiej4_ex8.cpp
 * Copyright 2018 xiej4@miamiOH.edu
 */

#include <ext/stdio_filebuf.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>


#define READ 0
#define WRITE 1   

using namespace std;
using Vecv = vector<string>;

void run_f(int pipeid, Vecv arglist) {
    dup2(pipeid, WRITE);
    std::vector<char*> args;
    for (size_t i = 1; (i < arglist.size()); i++) {
        args.push_back(&arglist[i][0]);
    }
    args.push_back(nullptr);
    execvp(args[0], &args[0]);
}

void run_s(int pipeid, Vecv arglist) {
    dup2(pipeid, READ);
    std::vector<char*> args;
    for (size_t i = 0; (i < arglist.size()); i++) {
        args.push_back(&arglist[i][0]);
    }
    args.push_back(nullptr);
    execvp(args[0], &args[0]);
}

void pipeF(int pipeid[], Vecv arg1, Vecv arg2) {
    pipe(pipeid);
    pid_t pid2, pid1 = fork();
    if (pid1 == 0) {
        close(pipeid[WRITE]);
        run_s(pipeid[READ], arg2);
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            close(pipeid[READ]);
            run_f(pipeid[WRITE], arg1);
        } else {
            int exitCode = 0;
            waitpid(pid2, &exitCode, 0);
            close(pipefd[1]);
            waitpid(pid1, &exitCode, 0);
        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    Vecv arg1;
    Vecv arg2;
    int pipefd[2];
    int pnum;
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        if (arg == "|") {
            pnum = i;
        }
    }
    for (int i = 0; i < pnum; i++) {
        string arg = argv[i];
        quoted(arg);
        arg1.push_back(arg);
    }
    for (int j = pnum + 1; j < argc; j++) {
        string arg = argv[j];
        quoted(arg);
        arg2.push_back(arg);
    }
    pipeF(pipefd, arg1, arg2);
    return 0;
}

