#include "DLLInject.hpp"

int main(int argc, char** argv)
{
    DLLInject injector{"demo.exe", "hook.dll"};
    injector.run();
    return 0;
}