#ifndef INC_PSYDUCK_ENTRY_H_
#define INC_PSYDUCK_ENTRY_H_

extern uint16_t ADCValues[64];
extern float pressure[3];
extern float current;
extern float offset[4];

int psyduck_entry();

#endif /* INC_PSYDUCK_ENTRY_H_ */
