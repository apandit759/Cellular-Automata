#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void file_input(int* Rule_vector,int size)
{
	FILE* f;
	int i;
	f=fopen("input.txt","r");
	for(i=0;i<size;i+=1)
		fscanf(f,"%d",&Rule_vector[i]);
}
void user_input(int* Rule_vector,int size)
{
	int i;
	for(i=0;i<size;i+=1)
		scanf("%d",&Rule_vector[i]);
}
void uniform_maker(int* Rule_vector,int size,int cell)
{
	int i;
	for(i=0;i<size;i+=1)
		Rule_vector[i]=cell;
}
void additive_random(int* Rule_vector,int size)
{
	int arr[]={105,150};
	int cell;
	cell=arr[rand()%2];
	uniform_maker(Rule_vector,size,cell);
}
void linear_random(int* Rule_vector,int size)
{
	int arr[]={1,254};
	int cell;
	cell=arr[rand()%2];
	uniform_maker(Rule_vector,size,cell);
}
int linear_cell_selector()
{
	int arr[]={1,105,150,254};
	printf("lcs\n");
	return(arr[rand()%4]);
}
int non_linear_cell_selector()
{
	int arr[252],i;
	printf("nlcs\n");
	for(i=0;i<252;i+=1)
		if(i==1 || i==105 || i==150 || i==254)
			continue;
		else
			arr[i]=i;
	return(arr[rand()%252]);
}
void random_mix(int* Rule_vector,int size,int pct)
{
	int linear_cells,non_linear_cells,flag1=0,flag2=0,i,x;
	non_linear_cells=(int)((double)size*((double)pct/100.0));
	linear_cells=size-non_linear_cells;
	for(i=0;i<size;i+=1)
	{
		//determinig randomly whether ith cell is linear or not
		if(flag1==non_linear_cells)
			Rule_vector[i]=linear_cell_selector();
		else if(flag2==linear_cells)
			Rule_vector[i]=non_linear_cell_selector();
		else
		{
			if(rand()%2==0)//this is for non-linear selections
			{
				Rule_vector[i]=non_linear_cell_selector();
				flag1+=1;
			}
			else
			{
				Rule_vector[i]=linear_cell_selector();
				flag2+=1;
			}
		}
	}
}
void complete_random(int* Rule_vector,int size)
{
	int i;
	for(i=0;i<size;i+=1)
		Rule_vector[i]=rand()%256;
}
void input_maker(int* Rule_vector,int size)
{	
	char a;int choice,cell,pct;
	printf("Is input Uniform?: Y/N \n");
	scanf("%c",&a);
	getchar();
	if(a=='y' || a=='Y')
	{
		printf("Is input Linear?: Y/N \n");
		scanf("%c",&a);
		getchar();
		if(a=='y' || a=='Y')
		{
			printf("Is input Additive?: Y/N \n");
			scanf("%c",&a);
			if(a=='y' || a=='Y')
			{	
				additive_random(Rule_vector,size);
			}
			else
			{
				linear_random(Rule_vector,size);
			}
		}
		else
		{
			printf("Enter a cell of the rule vector : ");
			scanf("%d",&cell);
			uniform_maker(Rule_vector,size,cell);
		}
	}
	else
	{
		
		getchar();
		printf("Is input Random?: Y/N \n");
		scanf("%c",&a);
		getchar();
		if(a=='y' || a=='Y')
		{
			printf("Press 1 for non-linear mix and 2 for complete random: ");
			scanf("%d",&choice);
			switch(choice)
			{
				case 1 : printf("Enter the non-linear mix percentage : ");
					 scanf("%d",&pct);
					 random_mix(Rule_vector,size,pct);
					 break;
				case 2 : complete_random(Rule_vector,size);
			}
		}
		else
		{
			printf("Press 1 for file input and 2 for user input: ");
			scanf("%d",&choice);
			switch(choice)
			{
				case 1 : file_input(Rule_vector,size);
				         break;
				case 2 : user_input(Rule_vector,size);
			}
		}
	}
}
