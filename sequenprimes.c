#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void flagNonPrime(int *arr, int size)
{
	int i,j;
	for(j=2;j<=floor((size+1)/2);j++)
	{
		for(i=2;i<=size;i++)
		{
			if(arr[i]!=0 && arr[j]!=0 && arr[i]%arr[j]==0 && arr[i]!=arr[j])
				arr[i]=0;
		}
	}
}

int main(int argc, char **argv)
{
	FILE *f;
	char buf[100];
	snprintf(buf, 100, "%s.txt", argv[1]);

    int size=atoi(argv[1])-2;
    int *arr=malloc(size*sizeof(int));
    int i;

    f = fopen(buf, "w");
  
	if (f == NULL) 
	{
	    printf("Failed to create the file.\n");
	    return -1;
	}
    
    for (i=2; i<=size; i++) 
        arr[i]=i;

    flagNonPrime(arr, size);


    for (i=2; i<=size; i++)
    {
    	if(arr[i]!=0)
    		fprintf(f,"%d ", arr[i]); 
    }

    free(arr);
    fclose(f);

    return 0;
}
