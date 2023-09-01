#include "AutoRunner.h"
#include "GraphicFactoryCairo.h"
#include <iostream>

int main(int argc, char *argv[]){
    CAutoRunner(argc, argv, CGraphicFactoryCairo::Instance());
    return 0;
}