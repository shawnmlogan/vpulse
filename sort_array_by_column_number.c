# include "vpulse.h"

void sort_array_by_column_number(double *parray,int rows,int cols, int sort_column_number)
{
int i = 0, j = 0, l;
double temp;

for(i=0;i<rows;i++)
  {
		for(j=i+1;j<rows;j++)
			{
/*printf("i = %d, j = %d, comparing %.0f and %.0f.\n",i,j,*(parray + i*cols + (sort_column_number - 1)),*(parray + j*cols + (sort_column_number - 1)));*/
			if(*(parray + i*cols + (sort_column_number - 1)) > *(parray + j*cols + (sort_column_number - 1)))
				{
				for(l=0;l<cols;l++)
					{
					temp = *(parray + i*cols + (sort_column_number - 1) + l);
					*(parray + i*cols + (sort_column_number - 1) + l) = *(parray + j*cols + (sort_column_number - 1) + l);
					*(parray + j*cols + (sort_column_number - 1) + l) = temp;
					}
				}
			}
	}
}