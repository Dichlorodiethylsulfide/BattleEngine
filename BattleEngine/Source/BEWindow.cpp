#include "BEWindow.h"
#include "BEConsoleIO.h"
#include "BETime.h"

#include "SDL2/SDL.h"


BEWindow::BEWindow(BEString WindowTitle, Dimensions Dimensions, unsigned int FrameRate)
{
    if(FrameRate < 1)
    {
        ERROR("The frame rate was either zero or negative.");
        return;
    }
    if(!Dimensions.IsValid())
    {
        ERROR("The dimensions for the window were invalid, the window will not be created.");
        return;
    }
    m_windowTitle = WindowTitle;
    m_windowDimensions = Dimensions;
    m_windowPointer = SDL_CreateWindow(m_windowTitle, Dimensions.TopLeft.X, Dimensions.TopLeft.Y, Dimensions.Dimensions.X, Dimensions.Dimensions.Y, SDL_WINDOW_SHOWN);
    m_millisecondsPerFrame = 1000 / FrameRate;
    m_renderLoop = {m_windowPointer};
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
        m_frameTimeKeeper = BETime::Now();
        if(m_engineLoop.IsTicking(BETickState::Running))
        {
            m_engineLoop.EngineTick();
        }
        if(m_renderLoop.IsTicking(BETickState::Running))
        {
            m_renderLoop.EngineTick();
        }
        if(m_engineLoop.IsTicking(BETickState::Stopped))
        {
            break;
        }
        EnsureFrameTimeIsKept();
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

void BEWindow::Init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        WARN(SDL_GetError());
    }
    /*if(!IMG_Init(IMG_INIT_PNG))
    {
        WARN(SDL_GetError());
    }*/
}

void BEWindow::Quit()
{
    SDL_Quit();
    //IMG_Quit();
}

void BEWindow::EnsureFrameTimeIsKept()
{
    const auto current_millis = m_frameTimeKeeper.SinceLastCall().As<BETime::Milliseconds>();
    if(current_millis < m_millisecondsPerFrame)
    {
        BETime::WaitForMilliseconds(m_millisecondsPerFrame - current_millis);
    }
    else
    {
        WARN("Frame time was not kept.");
    }
}
