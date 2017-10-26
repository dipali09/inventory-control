/*
	ORDER.H
	THIS HEADER FILE IS FOR INVENTORY MANAGEMENT PROJECT
*/
void sell_menu();
void sell();
void sold_today();
void sold_month();
void sold_total();

void sell_menu()
{
	int ch;
	char oldscr[2048];
	gettext(1,1,51,9,oldscr);
	do
	{
		clrwin(1,1,80,25);
		puttext(1,1,51,9,oldscr);
		rect(52,1,80,9,1,"SELL");
		cprintf("\r\n%c%c1: Sell New\r\n  2: Sold Today\r\n  3: Sold in Month\r\n  4: Complete Sell\r\n  5: Back",16,16);
		ch=select(1,2,5);
		switch(ch)
		{
			case 49:sell();
					break;
			case 50:sold_today();
					break;
			case 51:sold_month();
					break;
			case 52:sold_total();
					break;
			default:ch=100;
		}
	}while(ch!=100);
}	//END of sell_menu();

void sell()
{
	FILE *C,*P,*tmp,*O;
	Category Cat;
	Product Pro;
	Order Ord;
	int Qty;
	char ch,c_n[25],p_n[25];
	usr_window(21,3,54,19,"NEW SELL");
	C=fopen("Category.bin","rb");
	if(!C)
	{
		  cprintf("No Any Category Exist.\r\n    ");
		  pak();
		  return;
	}
	cprintf("\r\nCategory : \r\n\r\nPro_Name : \r\n\r\nQty      : ");
	gotoxy(12,2);
	cprintf("<select>");
	gotoxy(15,3);
	cprintf("%c",31);
	gotoxy(5,14);
	textcolor(15+128);
	cprintf("<<Press Esc to Exit>>");
	textcolor(15);
	while((ch=getch())!=13)
	{
		gotoxy(12,2);
		clreol();
		switch(ch)
		{
			case 80: if(fread(&Cat,sizeof(Cat),1,C))
					 {
						gotoxy(12,2);
						cprintf("%s",Cat.Cat_Name);
					 }
					 else
					 {
						 rewind(C);
						 fread(&Cat,sizeof(Cat),1,C);
						 {
							gotoxy(12,2);
							cprintf("%s",Cat.Cat_Name);
						}
					 }
					 clreol();
					 break;
			case 27: fclose(C);
					 return;
			default: gotoxy(12,2);
					 cprintf("%s",Cat.Cat_Name);
					 break;
		}
	}
	gotoxy(15,3);
	cprintf("   ");
	strcpy(c_n,Cat.Cat_Name);
	strcat(c_n,".cat");
	P=fopen(c_n,"rb");
	if(fread(&Pro,sizeof(Pro),1,P)==0)
	{
		gotoxy(12,4);
		fclose(P);
		fclose(C);
		cprintf("<No Item> ");
		pak();
		return;
	}
	rewind(P);
	gotoxy(12,4);
	cprintf("<select>");
	gotoxy(15,5);
	cprintf("%c",31);
	while((ch=getch())!=13)
	{
		switch(ch)
		{
			case 80: gotoxy(12,4);
					 clreol();
					 if(fread(&Pro,sizeof(Pro),1,P))
					 {
						gotoxy(12,4);
						cprintf("%s",Pro.Pro_Name);
					 }
					 else
					 {
						 rewind(P);
						 fread(&Pro,sizeof(Pro),1,P);
						 {
							gotoxy(12,4);
							cprintf("%s",Pro.Pro_Name);
						}
					 }
					 break;
			case 27: fclose(C);
					 fclose(P);
					 return;
			default: gotoxy(12,4);
					 clreol();
					 cprintf("%s",Pro.Pro_Name);
					 break;
		}
	}
	gotoxy(15,5);
	cprintf("  ");
	gotoxy(30,22);
	clreol();
	gotoxy(5,14);
	clreol();
	gotoxy(12,8);
	cprintf(" [%d] <-Available",Pro.Qty);
	gotoxy(12,6);
	_setcursortype(2);
	Qty=get_id();
	gotoxy(8,9);
	if(Qty<1)
	{
		cprintf("\r\n\r\nInvalid Quantity\r\n  ");
		_setcursortype(0);
		pak();
		fclose(P);
		fclose(C);
		return;
	}
	_setcursortype(0);
	if(Qty <= Pro.Qty)
	{
		strcpy(p_n,Pro.Pro_Name);
		rewind(P);
		tmp=fopen("tmp.cat","wb");
		while(fread(&Pro,sizeof(Pro),1,P))
		{
			if(strcmp(p_n,Pro.Pro_Name)==0)
			{
				Pro.Qty-=Qty;
				O=fopen("order.ord","ab");
				fseek(O,0,SEEK_END);
				Ord.O_Id=(((int)ftell(O))/sizeof(Ord))+1;
				Ord.Pro_Id=Pro.Pro_Id;
				Ord.Qty=Qty;
				strcpy(Ord.Pro_Name,Pro.Pro_Name);
				strcpy(Ord.Cat_Name,Cat.Cat_Name);
				Ord.Price=Pro.Price*Qty;
				getdate(&Ord.d);
				gettime(&Ord.t);
				fwrite(&Ord,sizeof(Ord),1,O);
				fclose(O);
			}
			fwrite(&Pro,sizeof(Pro),1,tmp);
		}     //End of if
		fclose(tmp);
		remove(c_n);
		rename("tmp.cat",c_n);
		cprintf("\r\n    DONE");

	}
	else
	{
		cprintf("Insufficient Quantity.");
	}
	fclose(P);
	delay(1000);
}	//END of sell();


void sold_today()
{
	FILE *O=fopen("order.ord","rb");
	Order Ord;
	int i,j,count=0;
	struct date today;
	clrwin(1,11,80,25);
	rect(1,11,80,25,1,"SOLD TODAY");
	gotoxy(1,1);
	textbackground(2);
	clreol();
	if(!O || feof(O))
	{
		gotoxy(10,1);
		cprintf(":: No any Product Sold ::");
		gotoxy(20,10);
		pak();
		fclose(O);
		return;
	}
	getdate(&today);
	gotoxy(24,1);
	cprintf("Item Sold On : %.2d/%.2d/%d",today.da_day,today.da_mon,today.da_year);
	gotoxy(1,2);
	textbackground(3);
	clreol();
	cprintf("Ord_Id %c Pro_Id %c Cat_Name %c Prod_Name %c Qty %c Total Price %c  Time   \r\n",179,179,179,179,179,179);
	i=3;
	textbackground(1);
	while(fread(&Ord,sizeof(Ord),1,O))
	{
		 if(Ord.d.da_day == today.da_day && Ord.d.da_mon == today.da_mon && Ord.d.da_year == today.da_year)
		 {
			count++;
			cprintf("%d",Ord.O_Id);
			gotoxy(8,i);
			cprintf("%c %d",179,Ord.Pro_Id);
			gotoxy(17,i);
			if(strlen(Ord.Cat_Name) > 10)
				cprintf("%c %.9s...",179,Ord.Cat_Name);
			else
				cprintf("%c %s",179,Ord.Cat_Name);
			gotoxy(28,i);
			if(strlen(Ord.Pro_Name) > 10)
				cprintf("%c %.9s...",179,Ord.Pro_Name);
			else
				cprintf("%c %s",179,Ord.Pro_Name);
			gotoxy(40,i);
			if(Ord.Qty > 99)
				cprintf("%c  99+",179);
			else
				cprintf("%c  %d",179,Ord.Qty);
			gotoxy(46,i);
			cprintf("%c %.2lf",179,Ord.Price);
			gotoxy(60,i++);
			cprintf("%c  %.2d:%.2d:%.2d\r\n",179,Ord.t.ti_hour,Ord.t.ti_min,Ord.t.ti_sec);
			if(i>9)
			{
				gotoxy(6,13);textbackground(3);pak();textbackground(1);
				i=3;
				clrscr();
				textbackground(2);
				clreol();
				gotoxy(24,1);
				cprintf("Item Sold On : %.2d/%.2d/%d",today.da_day,today.da_mon,today.da_year);
				gotoxy(1,2);
				textbackground(3);
				clreol();
				cprintf("Ord_Id %c Pro_Id %c Cat_Name %c Prod_Name %c Qty %c Total Price %c  Time   \r\n",179,179,179,179,179,179);
				textbackground(1);
			}
		 }
	}
	fclose(O);
	if(count)
	{
		gotoxy(10,13);textbackground(3);cprintf(":: END ::");textbackground(1);
	}
	else
		cprintf("No any Iteam sold today.\r\n  ");
	pak();
	clrwin(1,11,80,25);
}	//END of sold_today();


void sold_month()
{
	FILE *O=fopen("order.ord","rb");
	Order Ord;
	int i,j,count=0;
	struct date month;
	clrwin(1,11,80,25);
	rect(1,11,80,25,1,"MONTHLY SELL");
	gotoxy(1,1);
	textbackground(2);
	clreol();
	if(!O || feof(O))
	{
		gotoxy(10,1);
		cprintf(":: No any Product Sold ::");
		gotoxy(20,10);
		pak();
		fclose(O);
		return;
	}
	getdate(&month);
	gotoxy(24,1);
	cprintf("Item Sold in Month : %d/%d",month.da_mon,month.da_year);
	gotoxy(1,2);
	textbackground(3);
	clreol();
	cprintf("Ord_Id %c Pro_Id %c Cat_Name %c Prod_Name %c Qty %c Total Price %c  Time\r\n",179,179,179,179,179,179);
	i=3;
	textbackground(1);
	while(fread(&Ord,sizeof(Ord),1,O))
	{
		 if(Ord.d.da_mon == month.da_mon && Ord.d.da_year == month.da_year)
		 {
			count++;
			cprintf("%d",Ord.O_Id);
			gotoxy(8,i);
			cprintf("%c %d",179,Ord.Pro_Id);
			gotoxy(17,i);
			if(strlen(Ord.Cat_Name) > 10)
				cprintf("%c %.9s...",179,Ord.Cat_Name);
			else
				cprintf("%c %s",179,Ord.Cat_Name);
			gotoxy(28,i);
			if(strlen(Ord.Pro_Name) > 10)
				cprintf("%c %.9s...",179,Ord.Pro_Name);
			else
				cprintf("%c %s",179,Ord.Pro_Name);
			gotoxy(40,i);
			if(Ord.Qty > 99)
				cprintf("%c  99+",179);
			else
				cprintf("%c  %d",179,Ord.Qty);
			gotoxy(46,i);
			cprintf("%c %.2lf",179,Ord.Price);
			gotoxy(60,i++);
			cprintf("%c  %.2d:%.2d:%.2d | %.2d\r\n",179,Ord.t.ti_hour,Ord.t.ti_min,Ord.t.ti_sec,Ord.d.da_day);
			if(i>9)
			{
				gotoxy(6,13);textbackground(3);pak();textbackground(1);
				i=3;
				clrscr();
				textbackground(2);
				clreol();
				gotoxy(24,1);
				printf("Item Sold in Month : %d/%d",month.da_mon,month.da_year);
				gotoxy(1,2);
				textbackground(3);
				clreol();
				cprintf("Ord_Id %c Pro_Id %c Cat_Name %c Prod_Name %c Qty %c Total Price %c  Time\r\n",179,179,179,179,179,179);
				textbackground(1);
			}
		 }
	}
	fclose(O);
	if(count)
	{
		gotoxy(10,13);textbackground(3);cprintf(":: END ::");textbackground(1);
	}
	else
		cprintf("No any Item sold in this month.\r\n  ");
	pak();
	clrwin(1,11,80,25);
}	//END of sold_month();


void sold_total()
{
	FILE *O=fopen("order.ord","rb");
	Order Ord;
	int i,j,count=0;
	clrwin(1,11,80,25);
	rect(1,11,80,25,1,"COMPLETE SELL");
	gotoxy(1,1);
	textbackground(2);
	clreol();
	if(!O || feof(O))
	{
		gotoxy(10,1);
		cprintf(":: No Any Product Sold ::");
		gotoxy(20,10);
		pak();
		fclose(O);
		return;
	}
	cprintf("Ord_Id %c Pro_Id %c Cat_Name %c Prod_Name %c Qty %c Total Price %c  Date\r\n",179,179,179,179,179,179);
	i=2;
	textbackground(1);
	while(fread(&Ord,sizeof(Ord),1,O))
	{
			count++;
			cprintf("%d",Ord.O_Id);
			gotoxy(8,i);
			cprintf("%c %d",179,Ord.Pro_Id);
			gotoxy(17,i);
			if(strlen(Ord.Cat_Name) > 10)
				cprintf("%c %.9s...",179,Ord.Cat_Name);
			else
				cprintf("%c %s",179,Ord.Cat_Name);
			gotoxy(28,i);
			if(strlen(Ord.Pro_Name) > 10)
				cprintf("%c %.9s...",179,Ord.Pro_Name);
			else
				cprintf("%c %s",179,Ord.Pro_Name);
			gotoxy(40,i);
			if(Ord.Qty > 99)
				cprintf("%c  99+",179);
			else
				cprintf("%c  %d",179,Ord.Qty);
			gotoxy(46,i);
			cprintf("%c %.2lf",179,Ord.Price);
			gotoxy(60,i++);
			cprintf("%c %.2d/%.2d/%d\r\n",179,Ord.d.da_day,Ord.d.da_mon,Ord.d.da_year%100);
			if(i>9)
			{

				gotoxy(6,13);textbackground(3);pak();textbackground(1);
				i=2;
				clrscr();
				textbackground(2);
				clreol();
				cprintf("Ord_Id %c Pro_Id %c Cat_Name %c Prod_Name %c Qty %c Total Price %c  Date\r\n",179,179,179,179,179,179);
				textbackground(1);
			}
	}
	fclose(O);
	if(count)
	{
		gotoxy(10,13);textbackground(3);cprintf(":: END ::");textbackground(1);
	}
	else
		cprintf("No any Item sold.\r\n  ");
	pak();
	clrwin(1,11,80,25);
}	//END of sold_total();
