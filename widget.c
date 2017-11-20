#include "widget.h"

void DrawCursor(int x,int y){
    Bdisp_DrawLineVRAM(6*x-6,8*y-8,6*x-6,8*y-2);
    Bdisp_DrawLineVRAM(6*x-5,8*y-8,6*x-5,8*y-2);
}
int EditLine(int x,int y,char text[],int maxlen,int w){
	char tmp[256];
	unsigned int k;
	int len,start=0,cur=0,i,j;
	//复制字符串，计算长度.
	for(i=0;text[i];i++){
		tmp[i]=text[i];
	}
	tmp[i]=0;
	len=i;
	while(1){
		locate(x,y);
		for(i=0;i<w;i++){
			j=start+i;
			PrintC(j<len?tmp+j:" ");
		}
		DrawCursor(x+cur-start,y);
		GetKey(&k);
		if(len<maxlen&&(k>='0'&&k<='9'||k>='A'&&k<='Z')){
			tmp[len+1]=0;
			for(i=len;i>cur;i--)
				tmp[i]=tmp[i-1];
			tmp[cur]=k;
			len++;
			cur++;
		}
		if(k==KEY_CTRL_DEL){
			for(i=cur-1;tmp[i];i++)
				tmp[i]=tmp[i+1];
			len--;
			cur--;
		}
		if(k==KEY_CTRL_LEFT)	cur--;
		if(k==KEY_CTRL_RIGHT)	cur++;
		if(k==KEY_CTRL_AC){
			for(i=0;tmp[i];i++)	tmp[i]=0;
			len=0;
		}
		if(k==KEY_CTRL_EXE){
			for(i=0;tmp[i];i++)	text[i]=tmp[i];
			text[i]=0;
			return len;
		}
		if(k==KEY_CTRL_EXIT)	return -1;
		if(cur<0)			cur=0;
		if(cur>len)			cur=len;
		if(cur-start<1)		start--;
		if(cur-start>=w)	start++;
		if(start<0)			start=0;
	}
}
int ScrollSelector(int x,int y,char *name[],int num,int w,int h){
	int i,st=0,sel=0,cy=0;
	char blank[]="                     ";
	unsigned int k;
	while(1){
		for(i=0;i<h;i++){
			locate(x,y+i);Print(blank);
			if(st+i<num){
				locate(x,y+i);Print(name[st+i]);
			}
		}
		Bdisp_AreaReverseVRAM(6*x-6,8*(cy+y)-8,6*(x+w-1)-1,8*(cy+y)-1);
		GetKey(&k);
		if(k==KEY_CTRL_UP){
			sel--;cy--;
			if(sel<0)	sel=num-1;
			if(cy<0)	{cy=0;st--;}
			if(st<0)	st=num-1;
		}
		if(k==KEY_CTRL_DOWN){
			sel++;cy++;
			if(sel>=num){sel=0;st=0;cy=0;}
			if(cy>=h)	{cy=h-1;st++;}
		}
		if(k==KEY_CTRL_EXE){
			return sel;
		}
		if(k==KEY_CTRL_EXIT){
			return -1;
		}
	}
}
