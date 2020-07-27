#ifndef _FAAC_API_H_
#define _FAAC_API_H_
void aac_encode_init(faacEncHandle *a_pfaacHandle, int a_dInputFormat, int a_dSampleRate, int a_dChannels, int a_dmpegVersion, int *a_pdSamplesInput, int *a_pdMaxBytesOutput);
int aac_encode_run(faacEncHandle hEncoder,void *inputBuffer,unsigned int samplesInput,unsigned char *outputBuffer,unsigned int bufferSize);
void aac_encode_close(faacEncHandle *hEncoder);
#endif