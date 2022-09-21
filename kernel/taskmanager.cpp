#include <kernel/taskmanager.h>
#include <kernel/kernel.h>
#include <kernel/printer.h>

extern void* stackBase;

IMPL_MODULE_INSTANCE(TaskManager)

TaskManager::TaskManager()
{
    TaskManager::instance = this;
}

TaskManager::~TaskManager()
{
}

Task* TaskManager::makeTask(mainFunction entryPoint, int argc, char** argv)
{
    ds::Node* prevNode = this->findTaskPosition();
    Task *newTask = new Task(entryPoint, argc, argv);
    if (prevNode)
    {
        Task *prevTask = (Task*)prevNode->value;
        newTask->initialize(prevTask->context.rbp + TASK_STACK_SIZE);
    } else
    {
        newTask->initialize((uint64)stackBase);
    }
    list.addNodeAfter(prevNode,new ds::Node(newTask));
    if (list.getCurrent() == 0)
    {
        list.begin();
    }
    return newTask;
}

void TaskManager::initialize()
{
    this->list.begin();
}

void TaskManager::save(uint64 *address)
{
    ds::Node *node = this->list.getCurrent();
    if (node)
    {
        Task *task = (Task*)node->value;
        task->save(address);
    }
}

void TaskManager::load(uint64 *address)
{
    ds::Node *node = this->list.next();
    if (!node)
    {
        node = this->list.begin();
        // Printer::println("TaskManager :NULL", 18);
    }else{
        // Printer::println("TaskManager :OK", 16);
    }
    Task *task = (Task*)node->value;
    task->load(address);
}

void TaskManager::saveMainKernel(uint64 *address)
{
    ds::Node *node = this->list.first;
    Task *task = (Task*)node->value;
    task->save(address);

}

void TaskManager::loadMainKernel(uint64 *address)
{    
    ds::Node *node = this->list.first;
    Task *task = (Task*)node->value;
   
    task->load(address);
}


ds::Node* TaskManager::findTaskPosition()
{
    ds::Node *currentNode = list.begin();
    if (currentNode)
    {
        ds::Node *nextNode = list.next();
        while (nextNode)
        {
            Task *currentTask = (Task*)currentNode->value;
            Task* nextTask = (Task*)nextNode->value;
            if (nextTask->context.rbp - currentTask->context.rbp >= TASK_STACK_SIZE * 2)
            {
                return currentNode;
            }
            currentNode = nextNode;
            nextNode = list.next();
        }
    }
    return currentNode;
}