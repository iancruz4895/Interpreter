//
//  main.cpp
//  Program 4
//
//  Created by Ian Gabrielle Gojo Cruz on 10/29/18.
//  Copyright © 2018 Ian Gabrielle Gojo Cruz. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <string>
#include <set>

//including tokens.h
#include "tokens.h"
#include "parse.h"
#include "parsetree.h"
#include "value.h"
using namespace std;
void RuntimeError(int lineNumber, string msg) {
    cerr << lineNumber << ": RUNTIME ERROR " << msg << endl;
    exit(1);
}

int main(int argc, const char * argv[])
{
    void printStats(ParseTree*);
    vector <string> files;
    for (int i = 1; i < argc; i++)
    {
        files.push_back(argv[i]);
    }
    
    if (files.size() > 1)
    {
        cerr << "TOO MANY FILENAMES" << endl;
        return -1;
    }
    else
    {
        bool readFile = files.size() == 1;
        ifstream file;
        istream *in = &cin;
        int lineNumber = 0;
        if (readFile)
        {
            file.open(files.front());
            in = &file;
        }
        ParseTree *prog = Prog(in, &lineNumber);
        
        if(prog == 0)
            return 0; // quit on error
        
        prog ->Eval();
        
    }
    return 0;
}
