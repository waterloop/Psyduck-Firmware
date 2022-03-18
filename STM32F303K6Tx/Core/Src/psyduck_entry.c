#include "psyduck_entry.h"
#include "main.h"
#include "string.h"
#include <stdio.h>
#include "can.h"

void _initialize_can() {
  if (CANBus_init(&hcan, &htim7) != HAL_OK) {
    Error_Handler();
  }
  if (CANBus_subscribe(STATE_CHANGE_REQ) != HAL_OK) {
    Error_Handler();
  }
}

void psyduck_entry() {
  printf("\r\n");
  printf("pressure sensor is live...\r\n");

  printf("initializing CAN bus...\r\n");
  _initialize_can();

  while (1) {

    // RX Code
    if (!Queue_empty(&RX_QUEUE)) {
      CANFrame rx_frame = CANBus_get_frame();
      CANFrame tx_frame;
      uint8_t state_id = CANFrame_get_field(&rx_frame, STATE_ID);

      if ((state_id == ARMED) || (state_id == AUTO_PILOT) ||
          (state_id == ACCELERATING)) { // when brakes should be disengaged
        HAL_GPIO_WritePin(CONTROL_GPIO_Port, CONTROL_Pin, GPIO_PIN_RESET);
        tx_frame = CANFrame_init(PRESSURE_SENSOR_STATE_CHANGE_ACK_NACK);
        CANFrame_set_field(&tx_frame, PRESSURE_SENSOR_STATE_CHANGE_ACK_ID,
                           state_id);
        CANFrame_set_field(&tx_frame, PRESSURE_SENSOR_STATE_CHANGE_ACK, 0x00);
        CANBus_put_frame(&tx_frame);
      }

      else if ((state_id == BRAKING) || (state_id == EMERGENCY_BRAKE) ||
               (state_id == SYSTEM_FAILURE) ||
               (state_id ==
                MANUAL_OPERATION_WAITING)) { // when brakes should be engaged
        HAL_GPIO_WritePin(CONTROL_GPIO_Port, CONTROL_Pin, GPIO_PIN_SET);
        tx_frame = CANFrame_init(PRESSURE_SENSOR_STATE_CHANGE_ACK_NACK);
        CANFrame_set_field(&tx_frame, PRESSURE_SENSOR_STATE_CHANGE_ACK_ID,
                           state_id);
        CANFrame_set_field(&tx_frame, PRESSURE_SENSOR_STATE_CHANGE_ACK, 0x00);
        CANBus_put_frame(&tx_frame);
      }
    }

    // TX Code
    CANFrame tx_frame1;
    CANFrame tx_frame2;
    CANFrame tx_frame3;

    tx_frame1 = CANFrame_init(PRESSURE_SENSOR_HIGH);
    CANFrame_set_field(&tx_frame1, PRESSURE_HIGH, FLOAT_TO_UINT(pressure[0]));
    CANBus_put_frame(&tx_frame1);

    tx_frame2 = CANFrame_init(PRESSURE_SENSOR_LOW_1);
    CANFrame_set_field(&tx_frame2, PRESSURE_LOW_1, FLOAT_TO_UINT(pressure[1]));
    CANBus_put_frame(&tx_frame2);

    tx_frame3 = CANFrame_init(PRESSURE_SENSOR_LOW_2);
    CANFrame_set_field(&tx_frame3, PRESSURE_LOW_2, FLOAT_TO_UINT(pressure[2]));
    CANBus_put_frame(&tx_frame3);

    HAL_Delay(200);
  }
}
