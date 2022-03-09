#include "rtos_test.hpp"
#include "main.h"
#include "can.h"

RTOSThread::RTOSThread(char const * name, uint32_t stack_size, osPriority_t priority, void callback(void* arg)) {
    const osThreadAttr_t thread_attrs = {
        .name = name,
        .stack_size = stack_size,
        .priority = priority
    };

    this->thread = osThreadNew(
        callback,
        NULL,
        &thread_attrs
    );

}

RTOSThread::RTOSThread() {}

void RTOSThread::suspendThread() {
    osThreadSuspend(this->thread);
}

void RTOSThread::resumeThread() {
    osThreadResume(this->thread);
}

uint32_t RTOSThread::setFlag(uint32_t flag) {
    return osThreadFlagsSet(this, flag);
}

osThreadId_t RTOSThread::getThreadId() {
    return this->thread;
}

TestingThread::TestingThread() {
    this->thread = RTOSThread(
        "Testing thread",
        1024,
        osPriorityAboveNormal,
        this->testsRun
    );
}

void TestingThread::testsRun(void* args) {
    while (1) {

        osDelay(100);

        // send CAN message here
        CANFrame tx_frame = CANFrame_init(0);
        CANFrame_set_field(&tx_frame, PRESSURE_HIGH, 0x00);

        CANBus_put_frame(&tx_frame);
    }
}