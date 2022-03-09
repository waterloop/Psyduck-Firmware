#pragma once
#include <string.h>
#include "cmsis_os.h"
#include "can.h"

class RTOSThread {
    public:
        RTOSThread();
        RTOSThread(char const * name, uint32_t stack_size, osPriority_t priority, void callback(void* arg));    
        
        void suspendThread();
        void resumeThread();
        
        uint32_t setFlag(uint32_t flag);

        osThreadId_t getThreadId();    
    private:
        osThreadId_t thread;
};

class TestingThread {
    public:
        TestingThread();
    private:
        RTOSThread thread;
        static void testsRun(void *args);
};