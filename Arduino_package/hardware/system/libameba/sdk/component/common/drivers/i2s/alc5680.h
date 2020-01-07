#ifndef _ALC5680_H_
#define _ALC5680_H_

#define RT5680_DEVICE_ID			0x6385


void alc5680_status();
void alc5680_write_data_to_flash(const unsigned char *data_value,unsigned int len,unsigned int addr);
void alc5680_erase();
void alc5680_i2c_init(void);
u8 alc5680_reg_read_16(u32 reg, u16* val);
u8 alc5680_reg_read_32(u32 reg, u32* val);
u8 alc5680_reg_write_32(u32 reg, u32 val);
u8 alc5680_reg_write_16b(u32 reg, u16 val);

void alc5680_get_info(unsigned char *buffer);
void alc5680_handler_function(unsigned char *buffer,int len,unsigned int index);
void alc5680_check_sum();
int alc5680_get_version();
void set_alc5680_volume(unsigned char left,unsigned char right);
void get_alc5680_volume(unsigned short *value);//high byte is left and low byte is high
#endif
