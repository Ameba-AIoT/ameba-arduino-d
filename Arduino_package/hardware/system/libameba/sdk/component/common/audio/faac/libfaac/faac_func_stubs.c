#include "rom_faac_func_stubs.h"

extern const faac_func_stubs_t __rom_stubs_faac;

float *faac_get_pow43_res_table(void)
{
	return __rom_stubs_faac.pow43_res_table;
}

float *faac_get_adj43_res_table(void)
{
	return __rom_stubs_faac.adj43_res_table;
}
