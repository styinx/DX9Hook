#include "DLLInject.hpp"

int main(int argc, char** argv)
{
    DLLInject injector{"demo.exe", "hook.dll"};
    injector.run();
    return 0;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int shown)
{
    return main(__argc, __argv);
}