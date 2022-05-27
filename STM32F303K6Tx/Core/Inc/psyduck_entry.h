#pragma once

#define DISENGAGE_BRAKES()      (HAL_GPIO_WritePin(CONTROL_GPIO_Port, CONTROL_Pin, GPIO_PIN_RESET))
#define ENGAGE_BRAKES()         (HAL_GPIO_WritePin(CONTROL_GPIO_Port, CONTROL_Pin, GPIO_PIN_SET))

void _initialize_can();
void psyduck_entry();
