#include "threads.h"

const osThreadAttr_t can_attributes = {
  .name = "can",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

void can_th_fn(void *argument)
{
  for(;;)
  {
      osMutexAcquire(sensorVal_mtx, osWaitForever);
      CANFrame tx_frame0 = CANFrame_init(PRESSURE_SENSOR_HIGH);
      CANFrame_set_field(&tx_frame0, PRESURE_HIGH, FLOAT_TO_UINT(pressure[2]));

      CANFrame tx_frame1 = CANFrame_init(PRESSURE_SENSOR_LOW_1);
      CANFrame_set_field(&tx_frame1, PRESSURE_LOW_1, FLOAT_TO_UINT(pressure[0]));

      CANFrame tx_frame2 = CANFrame_init(PRESSURE_SENSOR_LOW_2);
      CANFrame_set_field(&tx_frame2, PRESSURE_LOW_2, FLOAT_TO_UINT(pressure[1]));
      osMutexRelease(sensorVal_mtx);

      osDelay(HEARTBEAT_PERIODICITY);
  }
}
