#pragma once

// Random generation of instructions
#define	MAX_JUMP_OFFSET	10
#define MAX_RAND_NUM	10

// Mutation params
#define DEF_HW_MUT_PROB			0.1

#define DEF_HW_MOD_CHANGE		0.5
#define DEF_HW_REG_CHANGE		0.5
#define DEF_HW_INSTR_SET_CHANGE	0.5
#define DEF_SW_INSTR_TYPE_CHANGE	0.5
#define DEF_SW_INSTR_PARAM_CHANGE	0.5
#define	DEF_SW_INSTR_IOS_CHANGE		0.5
#define DEF_SW_INSTR_POS_CHANGE		0.5
#define DEF_SW_INSTR_MODS_CHANGE	0.5

#define DEF_CROSS_HW	0.5
#define DEF_CROSS_SW	0.5

// Simulation params
#define DEF_SIMUL_MAX_TIME	200
