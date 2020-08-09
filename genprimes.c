#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
	int pid, n, elemsPerProc, elemsRcieved;

	FILE *f;
	char buf[50];
	snprintf(buf, 50, "%s.txt", argv[1]);

	f = fopen(buf, "w");

	if (f == NULL) 
	{
	    printf("Failed to create the file.\n");
	    return -1;
	}

	MPI_Init(&argc, &argv);

	MPI_Status status;

	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &n);

	int size=atoi(argv[1]);
	int *arr=malloc((size+2)*sizeof(int));
	int *myarr=malloc(((size/n)+2)*sizeof(int));

	if(pid==0) {
		int index, i=2;
		elemsPerProc=size/n;

		//create array
		for(i; i<=size; i++)
			arr[i]=i;

		if(n>1) {
			for (i=1; i<n-1; i++) {
				//distribute parts of the array amongst processes
				index=i*elemsPerProc;

				MPI_Send(&elemsPerProc, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&arr[index], elemsPerProc, MPI_INT, i, 0, MPI_COMM_WORLD);
			}
			//last process takes remaining elements
			index=i*elemsPerProc;
			int elemsLeft=size-index;

			MPI_Send(&elemsLeft, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&arr[index], elemsLeft, MPI_INT, i, 0, MPI_COMM_WORLD);

			//seting non-prime numbers to 0
			int j;
	        for(i=2; i<elemsPerProc; i++)
			{
				if(arr[i]!=0)
				{
					for(j=2;j<=floor((size+1)/2);j++)
					{
						if(arr[i]!=0 && arr[i]%j==0 && arr[i]!=j)
							arr[i]=0;
					}
				}
			}
			//MPI_Gather(&myarr, elemsRcieved, MPI_INT, &arr, elemsPerProc, MPI_INT, 0, MPI_COMM_WORLD);

		}
	}
	else{
		MPI_Recv(&elemsRcieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&myarr, elemsRcieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		//setting non-prime numbers to 0
		int i,j;
		for(i=2; i<elemsRcieved; i++)
		{
			if(myarr[i]!=0){
				for(j=2;j<=floor((size+1)/2);j++)
				{
					if(myarr[i]%j==0 && myarr[i]!=j)
						myarr[i]=0;
				}
			}
		}
		//MPI_Gather(&myarr, elemsRcieved, MPI_INT, &arr, elemsPerProc, MPI_INT, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();

	fclose(f);
	free(arr);
	free(myarr);

	return 0;


}