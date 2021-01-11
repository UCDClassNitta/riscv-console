#include "RISCVConsoleApplication.h"

int main(int argc, char *argv[]){
    auto MainApp = CRISCVConsoleApplication::Instance("edu.ucdavis.cs.ecs251.riscv-console");
    return MainApp->Run(argc, argv);
}