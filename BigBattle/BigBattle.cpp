
// Based on Touhou Big Big Battle

#include <BEConsoleIO.h>
#include <BEWindow.h>

int main(int argc, char* argv[])
{
    DEBUG("Hello World!");
    BEWindow window = {"Window", {{100, 100}, {800, 600}}};
    window.EnterMainLoop();
    DEBUG("Goodbye World!");
    return 0;
}
