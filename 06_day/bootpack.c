/* bootpack */


/* 编译器见到下面一行，便会替换成所指定文件（bootpack.h）的内容*/
/* 双引号表示该头文件与源文件位于同一个文件夹中‘
   尖括号表示该头文件位于编译器所提供的文件夹里*/
#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{
	struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
	char s[40], mcursor[256];
	int mx, my;

	init_gdtidt();
	init_pic();
	io_sti(); /* IDT/PIC的初始化已经完成，于是开放CPU的中�?*/
			  /* 仅仅执行STI指令，它是CLI的逆指令*/	
	init_palette();
	init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
	mx = (binfo->scrnx - 16) / 2; /* 计算画面的中心坐*/
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	io_out8(PIC0_IMR, 0xf9); /* 开放PIC1和键盘中�?11111001) */
	io_out8(PIC1_IMR, 0xef); /* 开放鼠标中�?11101111) */

	for (;;) {
		io_hlt();
	}
}
