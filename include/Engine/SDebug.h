/*
  SDebug.h David Parra
*/

#ifndef SDEBUG_H
#define SDEBUG_H

#include "imgui.h"

namespace SE
{
  class SDebug
  {
  public:
    /*
      Get the instance of the singleton
      @return the instance of the Singleton
    */
    static SDebug* GetInstance();
    SDebug(){}
    ~SDebug(){}

    /*
      Draws the current console GUI into the screen
      @param position The position of the Console screen
      @param scale The scale of the Console screen
      @param show If the Console will be drawn on the screen
    */
    void Draw(ImVec2 position, ImVec2 scale, bool show);

    /*
      Adds a log to the console state (white)
      @param log The log text that will be added
      @param ... The variables that will be printed (like a printf)
    */
    void Log(const char *log, ...); 

    /*
      Adds a error log to the console state (red)
      @param log The log text that will be added
      @param ... The variables that will be printed (like a printf)
    */
    void LogError(const char *log, ...);

    /*
      Adds a warning log to the console state (yellow)
      @param log The log text that will be added
      @param ... The variables that will be printed (like a printf)
    */
    void LogWarning(const char *log, ...);

  private:
    ImVector<char*>     Items;
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;
    int                 maxLogItems = 50;

    static SDebug *instance;

    /*
      Clear all the logs of the console
    */
    void ClearLog();
  };
}

#endif