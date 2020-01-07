#ifndef RESAMPLE_LIBRARY_H_
#define RESAMPLE_LIBRARY_H_

/*******************************************************************
 * resample_fractional.c
 ******************************************************************/
extern void Resample48khzTo32khz(const int* In, int* Out, int K);
extern void Resample32khzTo24khz(const int* In, int* Out, int K);
extern void Resample44khzTo32khz(const int* In, int* Out, int K);

/*******************************************************************
 * resample_48khz.c
 ******************************************************************/
typedef struct {
	int S_48_48[16];
	int S_48_32[8];
	int S_32_16[8];
} RESAMPLE_STAT48TO16;

extern void Resample48khzTo16khz(const short* in, int len, short* out, RESAMPLE_STAT48TO16* state, int *temp);
extern void ResampleReset48khzTo16khz(RESAMPLE_STAT48TO16* state);

typedef struct {
	int S_16_32[8];
	int S_32_24[8];
	int S_24_48[8];
} RESAMPLE_STAT16TO48;

extern void Resample16khzTo48khz(const short* in, int len, short* out, RESAMPLE_STAT16TO48* state, int* temp);
extern void ResampleReset16khzTo48khz(RESAMPLE_STAT16TO48* state);

typedef struct {
	int S_48_24[8];
	int S_24_24[16];
	int S_24_16[8];
	int S_16_8[8];
} RESAMPLE_STAT48TO8;

extern void Resample48khzTo8khz(const short* in, int len, short* out, RESAMPLE_STAT48TO8* state, int* temp);
extern void ResampleReset48khzTo8khz(RESAMPLE_STAT48TO8* state);

typedef struct {
	int S_8_16[8];
	int S_16_12[8];
	int S_12_24[8];
	int S_24_48[8];
} RESAMPLE_STAT8TO48;

extern void Resample8khzTo48khz(const short* in, int len, short* out, RESAMPLE_STAT8TO48* state, int* temp);
extern void ResampleReset8khzTo48khz(RESAMPLE_STAT8TO48* state);

/*******************************************************************
 * resample_by_2_internal.c
 ******************************************************************/
extern void ResampleDownBy2IntToShort(int *in, int len, short *out, int *state);
extern void ResampleDownBy2ShortToInt(const short *in, int len, int *out, int *state);
extern void ResampleUpBy2ShortToInt(const short *in, int len, int *out, int *state);
extern void ResampleUpBy2IntToInt(const int *in, int len, int *out, int *state);
extern void ResampleUpBy2IntToShort(const int *in, int len, short *out, int *state);
extern void ResampleLPBy2ShortToInt(const short* in, int len, int* out, int* state);
extern void ResampleLPBy2IntToInt(const int* in, int len, int* out, int* state);
#endif  // RESAMPLE_LIBRARY_H_
