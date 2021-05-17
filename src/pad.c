#include "pad.h"

//Pad state
typedef struct
{
    u_char	stat;
    u_char	len:4;
    u_char	type:4;
    u_short	btn;
    u_char	rs_x,rs_y;
    u_char	ls_x,ls_y;
} PADTYPE;

static u_short pad_buff[2][34/2];
Pad pad_state;

//Pad functions
void Pad_Init()
{
	//Initialize and start pad interface
	pad_state.held = 0;
	pad_state.press = 0;
	
	InitPAD((char*)pad_buff[0], 34, (char*)pad_buff[1], 34);
	pad_buff[0][0] = 0xFFFF;
	pad_buff[1][0] = 0xFFFF;
	StartPAD();
}

void Pad_Update()
{
	//Read port 1 state
	PADTYPE *pad = (PADTYPE*)pad_buff[0];
	
	if (pad->stat == 0)
	{
		//Read pad information
		if ((pad->type == 0x4) ||
			(pad->type == 0x5) ||
			(pad->type == 0x7))
		{
			//Set pad state
			pad_state.press = (~pad->btn) & (~pad_state.held);
			pad_state.held = ~pad->btn;
			pad_state.left_x  = pad->ls_x;
			pad_state.left_y  = pad->ls_y;
			pad_state.right_x = pad->rs_x;
			pad_state.right_y = pad->rs_y;
		}
	}
}