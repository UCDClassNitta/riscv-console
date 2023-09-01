#include "RISCVConsoleApplication.h"
#include "GUIFactoryGTK3.h"

int main(int argc, char *argv[]){
    auto MainApp = CRISCVConsoleApplication::Instance("edu.ucdavis.cs.ecs251.riscv-console",CGUIFactoryGTK3::Instance());
    return MainApp->Run(argc, argv);
}