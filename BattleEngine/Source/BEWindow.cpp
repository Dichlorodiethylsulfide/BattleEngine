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

void BEWindow::EnterMainLoop()
{
    if(!m_windowPointer)
    {
        ERROR("Could not start engine loop as there was no valid window.");
        return;
    }
    LOG("Engine loop started.");
    while(m_windowPointer)
    {
        if(m_engineLoop.IsTicking(BETickState::Running))
        {
            m_engineLoop.EngineTick();
        }
        if(m_renderLoop.IsTicking(BETickState::Running))
        {
            m_renderLoop.EngineTick();
        }
    }
    LOG("Engine loop ended.");
}

BEWindow::~BEWindow()
{
    if(m_windowPointer)
    {
        SDL_DestroyWindow(m_windowPointer.Cast<SDL_Window>());
    }
}
