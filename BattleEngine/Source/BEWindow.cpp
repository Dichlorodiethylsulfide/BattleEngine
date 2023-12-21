#include "BEWindow.h"
#include "BEConsoleIO.h"

#include <SDL2/SDL_video.h>

#include "SDL2/SDL_events.h"

void BECPULoop::Tick()
{
    SDL_Event Event{};
    while(SDL_PollEvent(&Event))
    {
        if (Event.type == SDL_KEYDOWN)
        {
            if (Event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_shouldStopTicking = true;
                break;
            }
        }
    }
}

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

void BEWindow::EnterLoop()
{
    if(!m_windowPointer)
    {
        ERROR("Could not start engine loop as there was no valid window.");
        return;
    }
    LOG("Engine loop started.");
    while(m_windowPointer && m_engineLoop)
    {
        m_engineLoop.Tick();
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
