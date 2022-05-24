#include "Studio.h"
#include <iostream>

using namespace std;

Studio* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    studio.start();
    if(backup!=nullptr){
        delete backup;
        backup = nullptr;
    }
    return 0;
}

//  open 2 nir,swt roei,chp ido,mcl
//  status 2
//  order 2
//  open 3
//  move 2 3 0
//  status 3
//  closeall