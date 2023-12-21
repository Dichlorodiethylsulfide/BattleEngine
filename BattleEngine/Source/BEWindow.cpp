#include "BEWindow.h"
#include "BEConsoleIO.h"

#include <SDL2/SDL_video.h>

BEWindow::BEWindow(BEString WindowTitle, BEBox<int> Dimensions)
{
    if(!Dimensions.IsValid())
    {
        ERROR("The dimensions for the window were invalid, the window will not be created.");
        return;
    }
    m_windowTitle = WindowTitle;
    m_windowDimensions = Dimensions;
    m_windowPointer = SDL_CreateWindow(m_windowTitle, Dimensions.TopLeft.X, Dimensions.TopLeft.Y, Dimensions.Dimensions.X, Dimensions.Dimensions.Y, SDL_WINDOW_SHOWN);
}

BEWindow::~BEWindow()
{
    if(m_windowPointer)
    {
        SDL_DestroyWindow(m_windowPointer.Cast<SDL_Window>());
    }
}
