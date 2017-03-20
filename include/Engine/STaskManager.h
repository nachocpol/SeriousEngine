/*
  STaskManager.h David Parra                                           
- Threads detect when a new task is added and one of them start to work (the others go to sleep)
- When a thread finishes its work checks if there is more tasks to do in the list
- if yes: the thread works on that task
- if not: the thread goes to sleep

- TaskHandler
- Contains the id of the task and the position of the array of the task (list task doing)
- if the id of the taskhandler and the id of the task on that position of the array are the same
- if yes: the specific task is still working (send a bool)
- if no: the task has ended (update taskhandler(delete?))

- When insert a new task it is stored here
- When a thread gets a task the task is moved to the list of tasks doing
- List of Tasks doing
*/


#ifndef STASKMANAGER_H_
#define STASKMANAGER_H_

#include <thread>
#include <condition_variable>
#include <functional>
#include <vector>

#include "glm.hpp"

#include "SDisplayList.h"
#include "SGameObject.h"
#include "SCamera.h"

namespace SE
{
  typedef std::shared_ptr<SGameObject> GOptr;

  class STask
  {
  public:
    STask(){};
    virtual ~STask(){};

    virtual void Execute(){};

    int id;
    bool working = false;
    bool ended = false;
  };

  class STaskHandler
  {
  public:
    STaskHandler(){};
    ~STaskHandler(){};

    int task_id;
    void Wait();
  };

  class STask_UpdateDL : public STask
  {
  public:
    STask_UpdateDL(SDisplayList &dl, std::vector<SGameObject*> &scene);
    ~STask_UpdateDL(){};
    void Execute();

  private:
    SDisplayList *DL;
    std::vector<SGameObject*> scene_copy;
  };

  class STaskManager
  {
  public:
    static STaskManager *GetInstance();
    STaskManager();
    ~STaskManager();

    void Init();
    bool TaskWorking(int task_id);
    void KillThreads();
    STaskHandler AddTask(std::unique_ptr<STask> dl);

  private:
    void ThreadSleep();
    void ThreadAlert();
    bool ThreadGetTask(); 
    void ThreadExecuteTask(); 
    void ThreadsWaitforAll();
    void ThreadMainFunction();

    std::mutex tasksTodoMutex;
    std::mutex tasksInProgressMutex;

    static STaskManager *instance;
    std::vector<std::thread> workers;

    std::vector<std::unique_ptr<STask>> tasksTodo;
    std::vector<std::unique_ptr<STask>> tasksInProgress;

    int cores;
    bool thread_end = false;
    bool initialized = false;
    int task_counter;

    std::condition_variable cv;
    std::mutex cv_m;
  };
}

#endif