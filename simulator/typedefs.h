#pragma once

#define NULL    0

#ifdef _DEBUG
    #define MYDEBUG_NEW   new( _NORMAL_BLOCK, __FILE__, __LINE__)
    // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
    //allocations to be of _CLIENT_BLOCK type
#else
    #define MYDEBUG_NEW
#endif // _DEBUG

typedef unsigned int	UINT;
typedef unsigned long	DWORD;
typedef unsigned char	BYTE;

typedef int	REGISTER_TYPE;
#define REGISTER_UNDEF 0xDEADBEEF
#define REG_DEF_VAL	0

/*#define FP_REGISTER_TYPE
typedef float	REGISTER_TYPE;
#define REGISTER_UNDEF 0xDEADBEEF
#define REG_DEF_VAL 0.0*/

// Header masks
#define	HEADER_MASK	0xFF000000
#define CONST_LOAD_MASK 0x0F000000
#define MOV_MASK 0x80000000
#define LOAD_MASK	0x90000000
#define IN_CONNECTION_MASK 0xA0000000
#define OUT_CONNECTION_MASK 0xC0000000
#define JXX_MASK 0x70000000
#define CONDITIONAL_JUMP_MASK 0x60000000

enum JUMPS {
	JMP = 1,
	JZ,
	JS,
	JNZ,
	JNS
};

// Delays definitions
#define REG_MANIPULATION_DELAY 5
#define IO_DELAY 10
#define JUMP_DELAY 5
