#include "Engine/SDebug.h"

#include "stdio.h"
#include <cstring>
#include <iostream>

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

using namespace SE;

SDebug* SDebug::instance = NULL;

///////////////////////////

SDebug* SDebug::GetInstance()
{
  if (!instance)
  {
    instance = new SDebug;
  }
  return instance;
}

///////////////////////////

void SDebug::ClearLog()
{
  for (int i = 0; i < Items.Size; i++)
    free(Items[i]);
  Items.clear();
  ScrollToBottom = true;
}

///////////////////////////

void SDebug::Draw(ImVec2 position, ImVec2 scale, bool show)
{
  ImGui::SetNextWindowPos(position);
  ImGui::SetNextWindowSize(scale); 
  if (!ImGui::Begin("Console", &show, scale, 0.6f, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
  {
    ImGui::End();
    return;
  }

  static ImGuiTextFilter filter;
  filter.Draw("Filter", 180);
  ImGui::SameLine();
  if (ImGui::Button("Clear")) ClearLog();
  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");

  ImGui::Separator();
  ImGui::BeginChild("ScrollingRegion", ImVec2(0, scale.y - scale.y/2.4f), false, ImGuiWindowFlags_HorizontalScrollbar);
  if (ImGui::BeginPopupContextWindow())
  {
    if (ImGui::Selectable("Clear")) ClearLog();
    ImGui::EndPopup();
  }
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); 
  for (int i = 0; i < Items.Size; i++)
  {
    const char* item = Items[i];
    if (!filter.PassFilter(item))
      continue;
    ImVec4 col = ImColor(255, 255, 255);
    if (strstr(item, "[error]")) col = ImColor(255, 0, 0);
    else if (strstr(item, "[warning]")) col = ImColor(255, 255, 0);
    ImGui::PushStyleColor(ImGuiCol_Text, col);
    ImGui::TextUnformatted(item);
    ImGui::PopStyleColor();
  }
  if (ScrollToBottom)
    ImGui::SetScrollHere();
  ScrollToBottom = false;
  ImGui::PopStyleVar();
  ImGui::EndChild();
  ImGui::End();
}

///////////////////////////

void SDebug::Log(const char *log, ...) IM_PRINTFARGS(2)
{
  char buf[1024];
  va_list args;
  va_start(args, log);
  vsnprintf(buf, IM_ARRAYSIZE(buf), log, args);
  buf[IM_ARRAYSIZE(buf) - 1] = 0;
  va_end(args);

  char text[] = "[log] ";
  char *newText = new char[std::strlen(text) + std::strlen(buf) + 1];
  std::strcpy(newText, text);
  std::strcat(newText, buf);

  Items.push_back(strdup(newText));
  ScrollToBottom = true;

  if (Items.Size >= maxLogItems){ Items.erase(Items.begin()); }

  delete[] newText;
}

///////////////////////////

void SDebug::LogError(const char *log, ...) IM_PRINTFARGS(2)
{
  char buf[1024];
  va_list args;
  va_start(args, log);
  vsnprintf(buf, IM_ARRAYSIZE(buf), log, args);
  buf[IM_ARRAYSIZE(buf) - 1] = 0;
  va_end(args);

  char text[] = "[error] ";
  char *newText = new char[std::strlen(text) + std::strlen(buf) + 1];
  std::strcpy(newText, text);
  std::strcat(newText, buf);

  Items.push_back(strdup(newText));
  ScrollToBottom = true;

  if (Items.Size >= maxLogItems){ Items.erase(Items.begin()); }

  delete [] newText;
}

///////////////////////////

void SDebug::LogWarning(const char *log, ...) IM_PRINTFARGS(2)
{
  char buf[1024];
  va_list args;
  va_start(args, log);
  vsnprintf(buf, IM_ARRAYSIZE(buf), log, args);
  buf[IM_ARRAYSIZE(buf) - 1] = 0;
  va_end(args);

  char text[] = "[warning] ";
  char *newText = new char[std::strlen(text) + std::strlen(buf) + 1];
  std::strcpy(newText, text);
  std::strcat(newText, buf);

  Items.push_back(strdup(newText));
  ScrollToBottom = true;

  if (Items.Size >= maxLogItems){ Items.erase(Items.begin()); }

  delete[] newText;
}

///////////////////////////