#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
struct node
{
	int val;
	struct node *next;
};
struct basin
{
	int *nodes_in_basin;
	int basin_size;
	int *nodes_in_loop;
	int loop_size;
};
void appender(int* arr, int size_of_arr, int item)
{
	arr=(int*)realloc(arr,(size_of_arr+1)*sizeof(int));
	arr[size_of_arr]=item;
}
int bin_to_dec(int* state,int state_size)
{
	int r=0,i;
	for(i=0;i<state_size;i+=1)
		r=r*2+state[i];
	return r;
}
void dec_to_bin(int x,int* a,int size)
{
	int i;
	for(i=size-1;i>-1;i-=1)
	{  
		a[i]=x%2;
		x=x/2;
	}
}
void Next_state_generation(int** Rule_matrix,int* present,int present_size)
{
	int i,j,s,e,val;
	int t[3];
	int* x=(int*)malloc(present_size*sizeof(int)); 
	for(i=0;i<present_size;i++)
	{
		s=i-1;
		e=i+1;
		if(s==-1)
		{
			t[0]=0;
			t[1]=present[0];
			t[2]=present[1];
		}
		else if(e==present_size)
		{
			t[0]=present[present_size-2];
			t[1]=present[present_size-1];
			t[2]=0;
		}
		else
			for(j=s;j<=e;j++)
				t[j-s]=present[j];
		val=bin_to_dec(t,3);
		x[i]=Rule_matrix[i][8-1-val];
	}
	for(i=0;i<present_size;i+=1)
		present[i]=x[i];
	free(x);
}
void next_states_maker(int* next_states,int* flagOfStates,int** Rule_matrix,int noOfPosStates,int size)
{
	int j;
	int *global_state=(int*)calloc(size,sizeof(int));
	while(1)
	{
	 	j=bin_to_dec(global_state,size);
	 	if(flagOfStates[j]==1)
	 		j=0;
		while(j<noOfPosStates && flagOfStates[j]==1)
		{
			j+=1;
		}
		if(j==noOfPosStates)
			break;
		int *temp1=(int*)malloc(size*sizeof(int));
		dec_to_bin(j,temp1,size);
		int x;
		for(x=0;x<size;x+=1)
			global_state[x]=temp1[x];
		free(temp1);
		flagOfStates[j]=1; //visited global state corresponding to decimal value j
		Next_state_generation(Rule_matrix,global_state,size);
		next_states[j]=bin_to_dec(global_state,size);
	}
	free(global_state);
}
int self_loop_counter(int* next_states,int noOfPosStates)
{
	int i,c=0;
	for(i=0;i<noOfPosStates;i+=1)
		if(i==next_states[i])
			c+=1;
	return(c);
}
int loop_finder(int  nodes_in_basin[],int basin_size,int* nodes_in_loop,int* next_states)
{
	int i,j,head,loop_size;
	for(i=0;i<basin_size;i+=1)
	{
		int start=nodes_in_basin[i];
		int temp=start,c=0;
		while(c<=basin_size)
		{
			c+=1;
			temp=next_states[temp];
			if(start==temp)
				break;
		}
		if(start==temp)
		{
			loop_size=c;
			head=start;
			break;
		}
	}
	for(i=0;i<loop_size;i+=1)
	{
		appender(nodes_in_loop,i,head);
		head=next_states[head];
	}
	return(loop_size);
}
int basin_former(struct basin** basins,struct node** states,int* next_states,int* flagOfStates,int noOfPosStates)
{
	int i=0,j,tmp;
	int basin_count=0;
	while(1) //creating the basins one at a time
	{
		while(i<noOfPosStates && flagOfStates[i]<0)
			i+=1;
		if(i==noOfPosStates)
			break;
		j=i;
		tmp=0;//denotes the current size of the basin
		basins[basin_count]=(struct basin*)malloc(sizeof(struct basin));
		basins[basin_count]->nodes_in_basin=(int*)malloc(sizeof(int));
		basins[basin_count]->nodes_in_basin[0]=-1;
		while(flagOfStates[i]==0) //inserting nodes part of a particular cycle in the basin
		{
			appender(basins[basin_count]->nodes_in_basin,tmp,states[i]->val);
			tmp+=1;
			flagOfStates[i]=-1; //state[i] won't be visited again
			i=next_states[i];
		}
		int x=0,x1;
		while(x<tmp)//check and add nodes which have their next set as one of the nodes present in the basin
		{
			x1=basins[basin_count]->nodes_in_basin[x];
			i=0;
			while(i<noOfPosStates)
			{
				if(flagOfStates[i]==-1)
					i+=1;
				else
				{
					if(next_states[i]==x1)
					{
						appender(basins[basin_count]->nodes_in_basin,tmp,i);
						tmp+=1;
						flagOfStates[i]=-1; //state[i] won't be visited again
					}
					i+=1;
				}
			}
			x+=1;
		}
		basins[basin_count]->basin_size=tmp;
		basin_count+=1;
		i=j+1;
	}
	return(basin_count);
}
void main ()
{
	int i, size, noOfPosStates;
	int **Rule_matrix,*Rule_vector,*flagOfStates,*next_states;
	printf("Enter number of cells : ");
	scanf("%d",&size);
	noOfPosStates=(int)pow(2,size);
	flagOfStates=(int*)calloc(noOfPosStates,sizeof(int)); //initialised to 0
	next_states=(int*)malloc(noOfPosStates*sizeof(int));
	memset(next_states,-1,noOfPosStates*sizeof(int)); //initialised to -1
	Rule_vector=(int*)malloc(size*sizeof(int));
	Rule_matrix=(int**)calloc(size,sizeof(int*));
	for(i=0;i<size;i++)
		Rule_matrix[i]=(int*)calloc(8,sizeof(int));
	printf("Enter the rule vector : ");
	for(i=0;i<size;i+=1) //constructing the rule matrix
	{
		scanf("%d",&Rule_vector[i]);
		dec_to_bin(Rule_vector[i],Rule_matrix[i],8);
	}
	free(Rule_vector);//freeing rule vector
	next_states_maker(next_states,flagOfStates,Rule_matrix,noOfPosStates,size);
	for(i=0;i<size;i+=1)//freeing rule matrix since no use
		free(Rule_matrix[i]);
	free(Rule_matrix);

	struct node **states=(struct node**)malloc(noOfPosStates*sizeof(struct node*)); //creating an array of nodes
	for(i=0;i<noOfPosStates;i+=1)//creating the nodes one at a time
	{
		states[i]=(struct node*)malloc(sizeof(struct node));
		states[i]->val=i;
	}

	struct basin **basins=(struct basin**)malloc(noOfPosStates*sizeof(struct basin*)); //creating an array of basins
	memset(flagOfStates,0,noOfPosStates*sizeof(int));//setting all flags to zero again
	int basin_count=basin_former(basins,states,next_states,flagOfStates,noOfPosStates);

	int max=basins[0]->basin_size;	
	int min=basins[0]->basin_size;
	for(i=1;i<basin_count;i++)
	{
		if(basins[i]->basin_size>max)
			max=basins[i]->basin_size;
		else if(basins[i]->basin_size<min)
			min=basins[i]->basin_size;		
	}
			
	for(i=0;i<basin_count;i+=1)
	{
		basins[i]->nodes_in_loop=(int*)malloc(sizeof(int));
		basins[i]->nodes_in_loop[0]=-1;
		basins[i]->loop_size=loop_finder(basins[i]->nodes_in_basin,basins[i]->basin_size,basins[i]->nodes_in_loop,next_states);
	}

	printf("\nBiggest basin size : %d\nSmallest basin size : %d",max,min); 		 		
	  
	printf("\nNo. of basins = %d\n",basin_count);
	
	printf("\nNo. of self loop states are : %d\n",self_loop_counter(next_states,noOfPosStates));

	int *nrs;
	nrs=(int*)malloc(sizeof(int));
	nrs[0]=-1;
	int k=0;
	for(i=0;i<noOfPosStates;i+=1)
	{
		int flag=1;
		for(int j=0;j<noOfPosStates;j+=1)
		{
			if(i==next_states[j])
			{
				flag=0;
				break;
			}
		}
		if(flag==1)
		{
			appender(nrs,k,i);
			k+=1;
		}
	}
	if(k!=0)
	{
		printf("NRSs are : ");
		for(i=0;nrs[0]!=-1 && i<k;i+=1)
			printf("%d ",nrs[i]);
		printf("\nThis is a non-group CA");
	}
	else
	{
		printf("There are no non-reachable states\nThis is a group CA");
		if(max==noOfPosStates)
			printf("\nThis is also a supermaxlength CA");
		else if(max==noOfPosStates-1)
			printf("\nThis is also a maxlength CA");
	}
	int num_of_posTS=0,TS_printer=0;
	for(i=0;i<basin_count;i+=1)
	{
		num_of_posTS=0;
		int *posTS;
		posTS=(int*)malloc(sizeof(int));
		posTS[0]=-1;
		int j;
		for(j=0;j<basins[i]->basin_size;j+=1)
		{
			int x2;
			int flag=1;
			for(x2=0;x2<k;x2+=1)
			{
				if(nrs[x2]==basins[i]->nodes_in_basin[j])
				{
					flag=0;
					break;
				}
			}
			if(flag==1)
			{
				appender(posTS,num_of_posTS,basins[i]->nodes_in_basin[j]);
				num_of_posTS+=1;
			}
		}
		for(j=0;j<num_of_posTS;j+=1)
		{
			int x2;
			int flag=1;
			for(x2=0;x2<basins[i]->loop_size;x2+=1)
			{
				if(posTS[j]==basins[i]->nodes_in_loop[x2])
				{
					flag=0;
					break;
				}
			}
			if(flag==1)
			{
				if(TS_printer==0)
				{
					printf("\nTSs are : ");
					TS_printer=1;
				}
				printf("%d ",posTS[j]);
			}
		}
		free(posTS);
	}
	if(TS_printer==0)
		printf("\nNo TSs found");
	char ch;
	printf("\nDo you want to print the basins? Press Y or N ");
	getchar();
	scanf("%c",&ch);
	if(ch=='Y' || ch=='y')
	{
		for(i=0;i<basin_count;i+=1)
		{
			int j;
			printf("Basin %d has size %d and has elements ",i+1,basins[i]->basin_size);
			for(j=0;j<basins[i]->basin_size;j+=1)
				printf("%d ",basins[i]->nodes_in_basin[j]);
			printf("Loop size is %d and loop elements are : ",basins[i]->loop_size);
			for(j=0;j<basins[i]->loop_size;j+=1)
				printf("%d ",basins[i]->nodes_in_loop[j]);
			printf("\n");
		}
	}
	//freeing
	free(nrs);
	for(i=0;i<noOfPosStates;i+=1)//creating the nodes one at a time
		free(states[i]);
	free(states);
	for(i=0;i<basin_count;i+=1)
	{
		free(basins[i]->nodes_in_basin);
		free(basins[i]->nodes_in_loop);
		free(basins[i]);
	}
	free(basins);
}
