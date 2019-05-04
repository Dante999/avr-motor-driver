#ifndef MENUE_H
#define MENUE_H



#define MENUE_MODE_EDIT 0
#define MENUE_MODE_VIEW 1

void menue_init();
void menue_refresh(uint8_t mode, int8_t encoder_value);


#endif // MENUE_H
