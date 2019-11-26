#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "intal.h"

typedef struct integer{
	int* digit;
	int length;
}integer;
void* zeroremoval(void* intal);


void* intal_create(const char* str)
{
	if(strlen(str)==0)
		return NULL;
	if(strcmp(str,"NaN")==0)
		return NULL;
	integer* head;
	if(isalpha(str[0]))
	{
		head=(integer*)malloc(sizeof(integer));
		head->length=1;
		head->digit=(int*)malloc(sizeof(int));
		head->digit[0]=0;
		return (void*)head;
	}
	int n=strlen(str);
	head=(integer*)malloc(sizeof(integer));
	head->length=0;
	head->digit=(int*)malloc(sizeof(int)*n);
	for(int i=0;i<n;i++)
	{
		head->digit[i]=str[i]-'0';  //ascii value of zero is 48
		head->length++;
	}
	return (void*)head;   //typecasting
}


char* intal2str(void* intal)
{
	if(intal==NULL)
	{
		char str1[4]="NaN";
		char* str=(char*)malloc(sizeof(char)*4);
		int i;
		for(i=0;i<3;i++)
			str[i]=str1[i];
		str[i]='\0';
		return str;
	}
	char* s;
	int i;
	intal=zeroremoval(intal);
	int intal_length=((integer*)intal)->length;
	s=(char*)malloc(sizeof(char)*(intal_length+1)); //intal_length+1 because at the end we need to add null character also
	for(i=0;i<intal_length;i++)
	{
		s[i]=((integer*)intal)->digit[i]+48;
	}
	s[i]='\0';
	return s;
}


void* zeroremoval(void* intal)
{
	if(intal==NULL)
		return NULL;
	int leadingzeros=0;
	int f=0;
	integer* pointer=(integer*)intal;
	int new_length=pointer->length;
	while(pointer->digit[f]==0 && new_length>1)
	{
		new_length--;
		f++;
		leadingzeros=1;
	}
	if(leadingzeros==0)
		return intal;
	else
	{
		for(int i=0;i<(pointer->length-f);i++)
		{
			pointer->digit[i]=pointer->digit[f+i];
		}
		pointer->length=pointer->length-f;
		pointer->digit=(int*)realloc(pointer->digit,sizeof(int)*(pointer->length)); //reallocating the memory
	}
	return (void*)pointer;
}

void intal_destroy(void* intal)
{
	if(intal!=NULL)
	{
		integer* t=(integer*)intal;
		free(t->digit);
		free(t);
		intal=NULL;
	}
}

void* intal_increment(void* intal)
{
	if(intal==NULL)
		return NULL;
	intal=zeroremoval(intal);
	int n=((integer*)intal)->length;
	int last_digit=((integer*)intal)->digit[n-1];
	if(last_digit!=9)
	{
		((integer*)intal)->digit[n-1]+=1;
		return (void*)intal;
	}
	int i=n-1;
	while(((integer*)intal)->digit[i]==9 && i>0)
	{
		((integer*)intal)->digit[i]=0;
		i--;
	}
	if(i!=0)
		((integer*)intal)->digit[i]++;
	else if(i==0 && ((integer*)intal)->digit[i]!=9)
		((integer*)intal)->digit[i]++;
	else
	{
		integer *new;
		new=(integer*)malloc(sizeof(integer));
		new->length=n+1;
		new->digit=(int*)malloc(sizeof(int)*(n+1));
		new->digit[0]=1;
		int j=1;
		while(j<=n)
		{
			new->digit[j]=0;
			j++;
		}
		intal_destroy(intal);
		return (void *)new;
	}
	return (void*)intal;
}

void* intal_decrement(void* intal)
{
	if(intal==NULL)
		return NULL;
	intal=zeroremoval(intal);
	if(((integer*)intal)->length==1 && ((integer*)intal)->digit[0]==0)   //if number is zero
		return (void*)intal;
	int n=((integer*)intal)->length;
	int value=((integer*)intal)->digit[n-1]-1;
	if(value>=0)
	{
		((integer*)intal)->digit[n-1]--;
		return (void*)intal;
	}
	int i=n-1;
	while(((integer*)intal)->digit[i]==0 && i>0)
	{
		((integer*)intal)->digit[i]=9;
		i--;
	}
	if(((integer*)intal)->digit[i]!=0 && i!=0)
	{
		((integer*)intal)->digit[i]--;
		intal=zeroremoval(intal);
		return intal;
	}
	if(i==0 && ((integer*)intal)->digit[i]>1)
	{
		((integer*)intal)->digit[i]--;
		intal=zeroremoval(intal);
		return intal;
	}
	if(i==0 && ((integer*)intal)->digit[i]==1)
	{
		integer *new;
		new=(integer*)malloc(sizeof(integer));
		new->length=n-1;
		new->digit=(int*)malloc(sizeof(int)*(n-1));
		int j=0;
		while(j<=n-2)
		{
			new->digit[j]=9;
			j++;
		}
		intal_destroy(intal);
		return (void *)new;
	}
	return (void*)intal;
}

int intal_compare(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL)
		return -2;
	intal1=zeroremoval(intal1);
	intal2=zeroremoval(intal2);
	int length1=((integer*)intal1)->length;
	int length2=((integer*)intal2)->length;
	if(length1>length2)
		return 1;
	else if(length1<length2)
		return -1;
	else
	{
		int i=0;
		while(((integer*)intal1)->digit[i]==((integer*)intal2)->digit[i] && i<length1-1)
			i++;
		if(((integer*)intal1)->digit[i]>((integer*)intal2)->digit[i])
			return 1;
		if(((integer*)intal1)->digit[i]<((integer*)intal2)->digit[i])
			return -1;
		else
			return 0;
	}
}

void* intal_add(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL)
		return NULL;
	void* new_intal1;
	void* new_intal2;
	integer* sum;
	sum=(integer*)malloc(sizeof(integer));
	intal1=zeroremoval(intal1);
	intal2=zeroremoval(intal2);
	int n1=((integer*)intal1)->length;
	int n2=((integer*)intal2)->length;
	//making first array greater number of elements
	if(n1<n2)
	{
		new_intal1=intal2;
		new_intal2=intal1;
	}
	else
	{
		new_intal1=intal1;
		new_intal2=intal2;
	}
	n1=((integer*)new_intal1)->length;
	n2=((integer*)new_intal2)->length;
	int k=n1-1;
	int i=n1-1;
	int j=n2-1;
	int max=n1;
	sum->digit=(int *)malloc((max)*sizeof(int));
	sum->length=max;
	int carry=0;
	int s=0;
	while (j >= 0)
	{
		s=((integer*)new_intal1)->digit[i]+((integer*)new_intal2)->digit[j]+carry;
 		sum->digit[k]=(s%10);
        	carry = s / 10; 
		k--; 
        	i--; 
        	j--; 
   	}
	while (i >= 0)
	{
  		// Add carry to first array elements. 
       		s = ((integer*)new_intal1)->digit[i]+ carry; 
		sum->digit[k]=(s%10);
		carry=s/10;
		i--;
		k--;
	}
	if(carry)
	{
		int p=n1-1;
		sum->digit=(int *)realloc(sum->digit,sizeof(int)*(max+1));
		while(p>=0)
		{
			sum->digit[p+1]=sum->digit[p];
			p--;
				
		}
		sum->length++;
		sum->digit[p+1]=carry;
	}
	
	return (void *)sum; 
}
  
void* intal_diff(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL)
		return NULL;
	intal1=zeroremoval(intal1);
	intal2=zeroremoval(intal2);
	int n1=((integer*)intal1)->length;
	int n2=((integer*)intal2)->length;
	char *temp="0";
	if(intal_compare(intal1,intal2)==0)
		return intal_create(temp);
	integer* difference=(integer*)malloc(sizeof(integer));
	difference->length=0;
	int k=0;
	int i=n1-1;
	int j=n2-1;
	int borrow=0;
	int maxlength=n1;
	if(n2>=n1)
	  maxlength=n2;
	if(intal_compare(intal1,intal2)==1)
	{
		k=maxlength;
		int y;
		y=k-1;
		difference->digit=(int *)malloc((maxlength)*sizeof(int));   
		while(i>=0 && j>=0)
		{
			difference->digit[y]=(((integer*)intal1)->digit[i])-(((integer*)intal2)->digit[j])-borrow;
			difference->length++;
			if(difference->digit[y]<0)
			{
				difference->digit[y]+=10; 
			 	borrow=1;
			}
			else
				borrow=0;
			y--;
			i--;
			j--;
		}
		if(i>=0)
		{
			while(i>=0)
			{
				difference->digit[y]=(((integer *)intal1)->digit[i])-borrow;
				difference->length++;
				if(difference->digit[y]<0)
				{
					difference->digit[y]+=10;
					borrow=1;
				}
				else
					borrow=0;
				y--;
				i--;
			}
		}
		difference=zeroremoval((void*)difference);
		return (void*)difference;
	}
	
	else if(intal_compare(intal1,intal2)==-1)
	{
		intal_diff(intal2,intal1);
	}
	return (void*)difference;
}

void* intal_multiply(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL)
		return NULL;
	char* str_intal1;
	char* str_intal2;
	intal1=zeroremoval(intal1);
	intal2=zeroremoval(intal2);
	str_intal1=intal2str(intal1);
	str_intal2=intal2str(intal2);
	int n1=((integer*)intal1)->length;
	int n2=((integer*)intal2)->length;
	char *temp="0";
	if(((integer*)intal1)->digit[0]==0 || ((integer*)intal2)->digit[0]==0)
		return intal_create(temp);
	int maxlength=0;
	integer* product=(integer*)malloc(sizeof(integer));
	if(n1>n2)
		maxlength=n1;
	else
		maxlength=n2;
	product->digit=(int *)calloc(2*maxlength,sizeof(int)); //to initialise all the entries with zero
	integer* final_product=(integer*)malloc(sizeof(integer));
	int i_n1 = 0; 
	int i_n2 = 0; 
	int count;
	for (int i=n1-1; i>=0; i--)
	{
		int carry = 0;
		int n11 = str_intal1[i] - '0';
		i_n2 = 0;     
		count=-1;    
		for (int j=n2-1; j>=0; j--)
		{
		    int n22 = str_intal2[j] - '0';
		    int sum = n11*n22 + product->digit[i_n1 + i_n2] + carry;
	 	    carry = sum/10;
	 	    product->digit[i_n1 + i_n2] = sum % 10;
		    i_n2++;
		    count++;
		}

		if (carry > 0)
		{
		    product->digit[i_n1 + i_n2] += carry;
		    count++;
		}
		i_n1++;
		
	}
	int j;
	j=0;
	int m=i_n1+count;
	final_product->digit=(int*)malloc(sizeof(int)*m);
	for(int t=(i_n1+count)-1;t>=0;t--)
	{
		final_product->digit[j]=product->digit[t];	
		final_product->length++;
		j++;
	}
	char* final_string=intal2str(final_product);
    	return intal_create(final_string);
}

void* intal_divide(void* intal1, void* intal2)
{
	
	if(intal1==NULL || intal2==NULL)
		return NULL;
	intal1=zeroremoval(intal1);
	intal2=zeroremoval(intal2);
	char *temp="0";
	if(((integer*)intal2)->digit[0]==0)
		return NULL;
	if(((integer*)intal1)->digit[0]==0)
		return intal_create(temp);
	if(intal_compare(intal1,intal2)==0)
	{
		integer* result;
    		result= (integer*)malloc(sizeof(integer));
    		result->length = 1;
    		result->digit = (int*)malloc(sizeof(int)*1);
    		result->digit[0] = 1;
    		return (void*)result;
	}
	if(intal_compare(intal1,intal2)==-1)
		return intal_create(temp);
	if(((integer*)intal2)->length==1 && ((integer*)intal2)->digit[0]==1)
		return intal1;
	integer* res=(integer*)malloc(sizeof(integer));
	res->length=1;
	res->digit=(int*)malloc(sizeof(int));
	res->digit[0]=1;
	char* num2=intal2str(intal2);
	void* diff=intal_diff(intal1,intal2);
	char* rr=intal2str(diff);
	void* res1=(void*)res;
	while(strcmp(rr,num2)>0)
	{
		res1=intal_increment(res);
		diff=intal_diff(diff,intal2);
		rr=intal2str(diff);
	}
	return res1;
}

void* intal_pow(void* intal1, void* intal2)
{
	if(intal1==NULL || intal2==NULL)
		return NULL;
	intal1=zeroremoval(intal1);
	intal2=zeroremoval(intal2);
	char *temp="0";
	char * str="2";
	char *str2="1";
	void* intal3=intal_create(str);
	void* intal4=intal1;
	if(((integer*)intal1)->digit[0]==0)
		return intal_create(temp);
	if(((integer*)intal2)->digit[0]==0)
		return intal_create(str2);
	if(((integer*)intal2)->digit[0]==1 && ((integer*)intal2)->length==1)
		return (void*)intal1;
	
	int n=((integer*)intal2)->length;
	int last_digit=((integer*)intal2)->digit[n-1];
	
	if(last_digit%2==0)
	{
		intal2=intal_divide(intal2,intal3);
		intal1=intal_pow(intal1,intal2);
		intal1=intal_multiply(intal1,intal1);
		return (void*)intal1;
	}
	else
	{
		intal2=intal_decrement(intal2);
		intal2=intal_divide(intal2,intal3);
		intal1=intal_pow(intal1,intal2);
		intal1=intal_multiply(intal1,intal1);
		intal1=intal_multiply(intal1,intal4);
		return (void*)intal1;
	}
	
	
}
	
		
		

	
	































