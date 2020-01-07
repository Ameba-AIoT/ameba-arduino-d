#ifndef _ALC5660_H_
#define _ALC5660_H_
void alc5660_reg_dump(void);
void alc5660_index_dump(void);
void alc5660_init(void);
void alc5660_set_word_len(int len_idx);
void alc5660_init_interface1(void);
void alc5660_init_interface2(void);
#endif