#include "Engine/STaskManager.h"
#include "Engine/STransform.h"

#include <iostream>

#define MULTITHREAD
//#define MONOTHREAD

namespace SE
{
  STaskManager* STaskManager::instance = NULL;

  ///////////////////////////////////////////////////////////////////

  STaskManager *STaskManager::GetInstance()
  {
    if (!instance)
    {
      instance = new STaskManager;
    }
    return instance;
  }

  ///////////////////////////////////////////////////////////////////

  STaskManager::STaskManager()
  {

  }

  ///////////////////////////////////////////////////////////////////

  STaskManager::~STaskManager()
  {

  }

  ///////////////////////////////////////////////////////////////////

  void STaskManager::KillThreads()
  {
    thread_end = true;
    ThreadAlert();

    for (int i = 0; i < cores; i++)
    {
      workers[i].join();
    }
    workers.clear();
  }

  ///////////////////////////////////////////////////////////////////

  void STaskHandler::Wait()
  {
    bool task_working;
    do
    {
      task_working = STaskManager::GetInstance()->TaskWorking(task_id);
    } while (task_working);
  }

  ///////////////////////////////////////////////////////////////////

  STask_UpdateDL::STask_UpdateDL(SDisplayList &dl, std::vector<SGameObject*> &scene)
  {
    DL = &dl;
    scene_copy = scene;
  }

  ///////////////////////////////////////////////////////////////////

  void STask_UpdateDL::Execute()
  {
    for (unsigned int i = 0; i < scene_copy.size(); i++)
    {
      //Only fill the display list if the go have both
      if (scene_copy[i]->mesh && scene_copy[i]->material)
      {
        DL->AddCmd(std::unique_ptr<SUseMaterial>(new SUseMaterial(scene_copy[i]->material, *scene_copy[i]->transform->GetModel())));
        DL->AddCmd(std::unique_ptr<SDrawGeometry>(new SDrawGeometry(scene_copy[i]->mesh)));
      }
    }
  }
     
  ///////////////////////////////////////////////////////////////////

  STaskHandler STaskManager::AddTask(std::unique_ptr<STask> dl_task)
  {
#ifdef MULTITHREAD
    STaskHandler th;

    tasksTodoMutex.lock();

    th.task_id = task_counter;
    dl_task->id = task_counter;

    //tasksTodo.push_back(dl_task);
    tasksTodo.push_back(std::move(dl_task));
    task_counter++;

    tasksTodoMutex.unlock();

    ThreadAlert();
    return th;
#endif

#ifdef MONOTHREAD
    STaskHandler th;
    th.task_id = task_counter;
    dl_task->id = task_counter;

    //tasksTodo.push_back(dl_task);
    tasksTodo.push_back(std::move(dl_task));
    task_counter++;
    ThreadGetTask();
    return th;
#endif // MONOTHREAD
  }

  ///////////////////////////////////////////////////////////////////

  bool STaskManager::TaskWorking(int task_id)
  {
    //Check if the task is in the pending list
    std::lock_guard<std::mutex> lockTaskTodo(tasksTodoMutex);
    if (!tasksTodo.empty())
    {
      //std::cout << "Tasks TODO: " << tasksTodo.size() << std::endl;
      for (unsigned int i = 0; i < tasksTodo.size(); i++)
      {
        if (tasksTodo[i]->id == task_id) 
        { 
          return true; 
        }
      }
    }

    std::lock_guard<std::mutex> lockTaskInProgress(tasksInProgressMutex);
    //Check if the task is in the working list
    if (!tasksInProgress.empty())
    {
      for (unsigned int j = 0; j < tasksInProgress.size(); j++)
      {
        if (tasksInProgress[j]->id == task_id) 
        { 
          return true; 
        }
      }
    }

    //If the task is not in the list
    return false;
  }

  ///////////////////////////////////////////////////////////////////

  void STaskManager::ThreadsWaitforAll()
  {
    for (int i = 0; i < cores; i++)
    {
      workers[i].join();
    }
  }

  ///////////////////////////////////////////////////////////////////

  void STaskManager::ThreadMainFunction()
  {
    while (!thread_end)
    {
      if (ThreadGetTask()) 
      {
        ThreadExecuteTask();
      }
      else
      {
        ThreadSleep();
      }
    }
  }

  ///////////////////////////////////////////////////////////////////

  void STaskManager::ThreadSleep()
  {
    std::unique_lock<std::mutex> lk(cv_m);
    cv.wait(lk);
    //std::cout << "Threads awaken..." << std::endl;
  }

  ///////////////////////////////////////////////////////////////////

  void STaskManager::ThreadAlert()
  {
    std::lock_guard<std::mutex> lk(cv_m);
    cv.notify_all();
    //cv.notify_one();
    //std::cout << "Alert Threads..." << std::endl;
  }

  ///////////////////////////////////////////////////////////////////

  bool STaskManager::ThreadGetTask()
  {
#ifdef MULTITHREAD
    tasksTodoMutex.lock();
    //std::cout << "Thread Get Task" << std::endl;
    if (tasksTodo.empty())
    {
      tasksTodoMutex.unlock();
      return false;
    }
    else
    {
      tasksInProgress.push_back(std::move(tasksTodo[0]));
      tasksTodo.erase(tasksTodo.begin());
      tasksTodoMutex.unlock();
      return true;
    }
#endif
#ifdef MONOTHREAD
    //tasksInProgress.push_back(tasksTodo[0]);
    tasksInProgress.push_back(std::move(tasksTodo[0]));
    tasksTodo.erase(tasksTodo.begin());
    ThreadExecuteTask();
    return true;
#endif
  }

  ///////////////////////////////////////////////////////////////////

  void STaskManager::ThreadExecuteTask()
  {
#ifdef MULTITHREAD
    //std::cout << "Executing task" << std::endl;
    tasksInProgressMutex.lock();
    tasksInProgress[0]->Execute();
    tasksInProgress.erase(tasksInProgress.begin());
    tasksInProgressMutex.unlock();
#endif
#ifdef MONOTHREAD
    //std::cout << "Executing task" << std::endl;
    tasksInProgress[0]->Execute();
    tasksInProgress.erase(tasksInProgress.begin());
#endif
  }

  ///////////////////////////////////////////////////////////////////

  void STaskManager::Init()
  {
    if (!initialized)
    {
#ifdef MULTITHREAD
      task_counter = 0;
      cores = std::thread::hardware_concurrency();
      //cores = 1;

      for (int i = 0; i < cores; i++)
      {
        workers.push_back(std::thread(&STaskManager::ThreadMainFunction, this));
      }

      //ThreadsWaitforAll();

#endif // MULTITHREAD

#ifdef MONOTHREAD
      task_counter = 0;

#endif // MONOTHREAD
      initialized = true;
    }
  }

  ///////////////////////////////////////////////////////////////////
}