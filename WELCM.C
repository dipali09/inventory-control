/*
	>>INVENTORY MANAGEMENT PROJCT<<
	TEAM  :	VIBHANSHU RAI, LAKSHMI, DIPALI, POONAM  >>SUPER 20 GROUP<<
	MENTOR: PROF. RAJEEV KUMAR SAHU
*/
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include"user.h"
#include"prod.h"
#include"order.h"
void main_menu();
void cat_list();
void create_cat();
void remove_cat();
void update_cat();


void main_menu()
{
	int ch,c_id;
	char oldscr[1024];
	gettext(1,1,30,9,oldscr); //reading text from console
	do
	{
		clrwin(1,1,80,25);
		puttext(1,1,30,9,oldscr);  //printing text to the console
		rect(32,1,50,9,1,"Main Menu"); //drawing a rectangle
		cprintf("\r\n%c%c1: Category\r\n  2: Products\r\n  3: Sell\r\n  4: Back",16,16);
		ch=select(1,2,4);
		switch(ch)
		{
			case 49:cat_list();
					break;
			case 50:all_prod_list();
					break;
			case 51:sell_menu();
					break;
			default: ch=100;
					break;
		}
	}while(ch!=100);
}

void cat_list()
{
	FILE *fl;
	Category Cat;
	int ch,id,i,ch1;
	do
	{
		ch=0;
		clrwin(1,11,80,25);    //clearing a specific part of consol
		rect(1,11,50,25,1,"CATEGORIES");
		gotoxy(1,1);
		textbackground(2);
		clreol();             //clear line from the cursor
		fl=fopen("Category.bin","rb");
		if(fl && !feof(fl))
		{
			cprintf("Cat_Id %c Cat_Name      %c   Time   | Date \r\n",179,179);
			textbackground(1);
			while(fread(&Cat,sizeof(Cat),1,fl))
			{
				cprintf("%-7d%c %-14s",Cat.Cat_Id,179,Cat.Cat_Name);
				cprintf("%c %.2d:%.2d:%.2d | %d/%d/%d\r\n",179,Cat.t.ti_hour,Cat.t.ti_min,Cat.t.ti_sec,Cat.d.da_day,Cat.d.da_mon,Cat.d.da_year);
				ch++;
				if(ch>9)
				{
					gotoxy(6,13);textbackground(3);pak();textbackground(1);//pak() for BLINKING press any kay
					clrscr();
					textbackground(2);
					clreol();
					cprintf("Cat_Id %c Cat_Name      %c   Time   | Date \r\n",179,179);
					textbackground(1);
					ch=0;
				}
			}
			gotoxy(18,13);textbackground(3);cprintf(":: END ::");textbackground(1);
			fclose(fl);
		}
		else
		{
			gotoxy(19,1);
			cprintf(":: No Category:: ");
		}
		rect(52,1,80,9,1,"CATEGORY");
		cprintf("\r\n%c%c1: Create\r\n  2: See Products\r\n  3: Add product\r\n  4: Update\r\n  5: Remove\r\n  6: Back",16,16);
		ch1=select(1,2,6);
		switch(ch1)
		{
			case 49: create_cat();
					break;
			case 50:usr_window(25,11,50,16,"Enter"); //drawing a window with shadow on consol
					cprintf("\r\nCat_Id : ");
					id=get_id();    //get only int value
					prod_list(id);
					break;
			case 51: New_Prod();
					getch();
					break;
			case 52:update_cat();
					break;
			case 53:remove_cat();
					break;
			default: ch1=100;
					break;
		}
	}while(ch1!=100);

}


void create_cat()
{
	FILE *fl=fopen("Category.bin","rb"),*temp=fopen("tmp.bin","wb");
	Category Cat,tmp;
	char ch;
	usr_window(25,11,58,18,"Create Category");
	cprintf("\r\nCat_Id : ");
	Cat.Cat_Id=get_id();
	cprintf("\r\n\r\nCat_Name: ");
	scanf("%s",&Cat.Cat_Name);
	getdate(&Cat.d);       //get system date and store in d
	gettime(&Cat.t);       //get system time and store in t
	while(fread(&tmp,sizeof(tmp),1,fl))
	{
		if(tmp.Cat_Id==Cat.Cat_Id || strcmpi(tmp.Cat_Name,Cat.Cat_Name)==0)
				{
				cprintf("\r\nThis Cat_Id or Cat_Name is already Exist.");
				pak(); //press any key blink;
				fclose(fl);
				fclose(temp);
				return;
				}
	}
	rewind(fl);
	if(fread(&tmp,sizeof(Cat),1,fl))
	{
		while(tmp.Cat_Id < Cat.Cat_Id && !feof(fl))
		{
			fwrite(&tmp,sizeof(tmp),1,temp);
			fread(&tmp,sizeof(tmp),1,fl);
		}
		fwrite(&Cat,sizeof(Cat),1,temp);
		if(!feof(fl))
			fwrite(&tmp,sizeof(tmp),1,temp);
		while(fread(&tmp,sizeof(tmp),1,fl))
			fwrite(&tmp,sizeof(tmp),1,temp);
	}
	else
		fwrite(&Cat,sizeof(Cat),1,temp);
	fclose(fl);
	fclose(temp);
	remove("Category.bin");
	rename("tmp.bin","Category.bin");
	cprintf("\r\nDone.");
	delay(1000);
	strcat(Cat.Cat_Name,".cat");
	fl=fopen(Cat.Cat_Name,"wb");
	fclose(fl);
}


void update_cat()
{
	FILE *fl=fopen("Category.bin","rb"),*tmp=fopen("tmp.bin","wb"),*C;
	Category Cat,Ca;
	int f=0,t;
	char cp[25],cp1[25],cp2[25],oldscr[2048];
	gettext(25,11,50,18,oldscr);
	usr_window(25,11,47,16,"Update Category");
	cprintf("\r\nCat_Id : ");
	t=get_id();
	while(fread(&Cat,sizeof(Cat),1,fl))
	{
		if(Cat.Cat_Id==t)
		{
			cprintf("\r\n\r\nNew Name: ");
			scanf("%s",cp);
			strcpy(cp1,cp);
			strcat(cp1,".cat");
			C=fopen(cp1,"rb");
			if(C)
			{
				cprintf("\r\nCategory Name already Exist. ");
				pak();
				fclose(C);
				fclose(tmp);
				fclose(fl);
				puttext(25,11,60,20,oldscr);
				return;
			}
			else
			{
				fclose(C);
			}
			strcpy(cp2,Cat.Cat_Name);
			strcat(cp2,".cat");
			rename(cp2,cp1);
			strcpy(Cat.Cat_Name,cp);
			getdate(&Cat.d);
			gettime(&Cat.t);
			f++;
		}
		fwrite(&Cat,sizeof(Cat),1,tmp);
	}
	if(f)
	{
		cprintf("\r\nUpdation Successful.");
	}
	else
	{
		cprintf("\r\nCategory Not Exist.");
	}
	fclose(fl);
	fclose(tmp);
	remove("Category.bin");
	rename("tmp.bin","Category.bin");
	delay(1000);
	puttext(25,11,60,20,oldscr);
}
void remove_cat()
{
	int f=0,t;
	Category Cat;
	FILE *fl=fopen("Category.bin","rb"),*tmp=fopen("tmp.bin","wb"),*t2;
	char oldscr[2048];;
	gettext(25,11,60,18,oldscr);
	usr_window(25,11,58,16,"Remove Category");
	cprintf("\r\nCat_Id : ");
	t=get_id();
	while(fread(&Cat,sizeof(Cat),1,fl))
	{
		if(t!=Cat.Cat_Id)
		{
			fwrite(&Cat,sizeof(Cat),1,tmp);
		}
		else
		{
			strcat(Cat.Cat_Name,".cat");
			remove(Cat.Cat_Name);
			f++;
		}
	}
	if(!f)
	{
		cprintf("\r\nNo Such Category Exist.\r\n       ");pak();
	}
	else
		cprintf("\r\nCategory Removed.\r\n      ");delay(1000);
	fclose(fl);
	fclose(tmp);
	remove("Category.bin");
	rename("tmp.bin","Category.bin");
	puttext(25,11,60,20,oldscr);
}


void main()
{
	int ch,ch1,ch2,R;
	char U_Name[25];
	do
	{
		_setcursortype(0);    //hide cursor on consol
		textbackground(BLUE);
		window(1,1,80,25);
		clrscr();
		rect(1,1,80,25,1," WELCOME ");
		gotoxy(31,22);
		textcolor(8);
		cprintf("INVENTORY MANAGEMENT");
		gotoxy(36,23);
		textcolor(8+128);                 //BLINKING
		cprintf("Version 1.0");
		textcolor(15); //WHITE text color;
		gotoxy(18,4);
		cprintf("%c%c1: Log in",16,16);
		gotoxy(20,5);
		cprintf("2: Exit  ");
		ch=select(18,4,2);
		switch(ch)
		{

			 case 49: R=Old_User(U_Name);strupr(U_Name);
				 if(R==0)
				 {
					clrscr();
					gotoxy(18,4);
					printf("You 1st Have to create an admin User.\n");
					New_User();
					delay(1000);

				 }
				 else
				 if(R==1)
				 {
				   do
				   {
					clrwin(1,1,80,25);
					rect(1,1,30,9,1,U_Name);
					cprintf("\r\n%c%c1: Create new user\r\n  2: See All User's Detail\r\n  3: Main Menu\r\n  4: Change password\r\n  5: Log out",16,16);
					ch1=select(1,2,5);
					switch(ch1)
					{
						case 49: New_User();
								delay(2000);
								break;
						case 50: see_user();
								break;
						case 51: main_menu();
								break;
						case 52: change_pass(R);
								break;
						case 53:log_out();
								break;
					}
				   }while(ch1!=53);
				 }
				 else
				 if(R>1)
				 {
					do
					{
					clrwin(1,1,80,25);
					rect(1,1,30,9,1,U_Name);
					cprintf("\r\n%c%c1: Main Menu\r\n  2: Change Password\r\n  3: Log out\r\n",16,16);
					ch1=select(1,2,3);
					switch(ch1)
					{
						case 49: main_menu();
								break;
						case 50: change_pass(R);
								break;
						case 51:log_out();
								break;
					}
					}while(ch1!=51);
				 }
				 else
				 {
					clrwin(30,23,40,23);
					cprintf("Retry...");
						delay(2000);
				 }
				 break;
			case 50: good_bye();
				break;
		}
	}while(ch!=50);
}