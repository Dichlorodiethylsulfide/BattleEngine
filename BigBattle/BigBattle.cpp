
// Based on Touhou Big Big Battle

#include <BEConsoleIO.h>
#include <BEWindow.h>

#include <BEVector.h>
#include <BEList.h>
#include <iostream>

#define NOT !

static void ValidateAllocations()
{
    bool validated = true;
    for(size_t i = 0; i < ALLOC_COUNT; i++)
    {
        if(NOT allocations[i].is_free)
        {
            validated = false;
            std::cout << "Allocation was not freed at: " << i << "\n";
        }
    }
    if(validated)
    {
        std::cout << "Successfully validated allocations" << "\n";
    }
    std::cout << "Total Allocations: " << totalAllocations << "\n";
    std::cout << "Total Deallocations: " << totalDeallocations << "\n";
}

void function()
{
    BEVector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
}

int main(int argc, char* argv[])
{
    /*DEBUG("Hello World!");
    BEWindow::Init();
    BEWindow window = {"Window", BEWindow::Dimensions::MakeDimensions(100, 100, 800, 600)};
    window.EnterMainLoop();
    BEWindow::Quit();
    DEBUG("Goodbye World!");*/

    for(size_t i = 0; i < 10; i++)
    {
        function();
    }
    
    ValidateAllocations();
    return 0;
}