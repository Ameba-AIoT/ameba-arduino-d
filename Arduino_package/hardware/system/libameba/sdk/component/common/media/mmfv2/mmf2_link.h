#ifndef MMF2_INTERCONNECT_H
#define MMF2_INTERCONNECT_H

// MMIC : media moudle inter connection
//-----------------------------------------------------------------------------
// Inter connection module command
#define MMIC_CMD_ADD_INPUT        	0x00
#define MMIC_CMD_ADD_INPUT0       	0x00
#define MMIC_CMD_ADD_INPUT1       	0x01
#define MMIC_CMD_ADD_INPUT2       	0x02
#define MMIC_CMD_ADD_INPUT3       	0x03

#define MMIC_CMD_ADD_OUTPUT       	0x10
#define MMIC_CMD_ADD_OUTPUT0      	0x10
#define MMIC_CMD_ADD_OUTPUT1      	0x11
#define MMIC_CMD_ADD_OUTPUT2      	0x12
#define MMIC_CMD_ADD_OUTPUT3      	0x13

#define MMIC_CMD_SET_STACKSIZE	  	0x20
#define MMIC_CMD_SET_TASKPRIORITY 	0x21

#define MMIC_STAT_EXIT            	0x00
#define MMIC_STAT_RUN             	0x01
#define MMIC_STAT_PAUSE            	0x02
#define MMIC_STAT_SET_EXIT        	0x10
#define MMIC_STAT_SET_PAUSE			0x20
#define MMIC_STAT_SET_RUN			0x40

//
#define MM_OUTPUT_MASK				0xf0
#define MM_OUTPUT					0x10
#define MM_OUTPUT0					0x10
#define MM_OUTPUT1					0x20
#define MM_OUTPUT2					0x40
#define MM_OUTPUT3					0x80
#define MM_INPUT_MASK				0x0f
#define MM_INPUT					0x01
#define MM_INPUT0					0x01
#define MM_INPUT1					0x02
#define MM_INPUT2					0x04
#define MM_INPUT3					0x08

#define MMIC_DEP_INPUT0				MM_INPUT0
#define MMIC_DEP_INPUT1				MM_INPUT1
#define MMIC_DEP_INPUT2				MM_INPUT2
#define MMIC_DEP_INPUT3				MM_INPUT3

#endif