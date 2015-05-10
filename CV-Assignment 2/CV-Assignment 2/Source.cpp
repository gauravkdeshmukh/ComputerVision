/*
GAURAV DESHMUKH
COSC 5356 COMPUTER VISION
ASSIGNMENT #2

***  APPLY FOLOWING MASKS TO GIVEN IMAGE:
  *  1. ROBERT'S
  *  2. SOBEL
  *  3. LAPLACIAN
     4. PREWITT
	 5. GRADIENT ***
*/

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <string.h>
#include <math.h>

/*bitwise operations macro*/
#define HI(num) (((num) & 0x0000FF00) >> 8)
#define LO(num) ((num) & 0x000000FF)

/*Structure to store PGM file*/
typedef struct _PGMData {
    int row;
    int col;
    int max_gray;
    int **matrix;
} PGMData;

/*function to allocate dyanamic matrix*/
int **allocate_dynamic_matrix(int row, int col)
{
    int **ret_val;
    int i;
    /*allocate dynamic memory for rows*/
    ret_val = (int **)malloc(sizeof(int *) * row);
    if (ret_val == NULL) {
        perror("memory allocation failure");
        exit(EXIT_FAILURE);
    }
   /*allocate dynamic memory for columns*/
    for (i = 0; i < row; ++i) {
        ret_val[i] = (int *)malloc(sizeof(int) * col);
        if (ret_val[i] == NULL) {
            perror("memory allocation failure");
            exit(EXIT_FAILURE);
        }
    }
 
    return ret_val;
}
 
/*function to de-allocate dynamic matrix */
void deallocate_dynamic_matrix(int **matrix, int row)
{
    int i;
 /*de-allocate dynamic memory for whole matrix*/
    for (i = 0; i < row; ++i)
        free(matrix[i]);
    free(matrix);
}

/*function to skip comments and spaces*/
void SkipComments(FILE *fp)
{
    int ch;
    char line[100];
    /*check for EOF and Space to skip*/
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
	/*All comments start with '#' , if found skip line*/
    if (ch == '#') {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    } else
        fseek(fp, -1, SEEK_CUR);
}

/*function to read PGM file*/
PGMData* readPGM(const char *file_name, PGMData *data)
{
    FILE *pgmFile;
    char version[3];
    int i, j;
    int lo, hi;
    /*open a input file to read*/
	pgmFile = fopen(file_name, "rb");
    if (pgmFile == NULL) {
        perror("cannot open file to read");
		getchar();
        exit(EXIT_FAILURE);
    }
    /*check for P5 version*/
    fgets(version, sizeof(version), pgmFile);
    if (strcmp(version, "P5")) {
        fprintf(stderr, "Wrong file type!\n");
		getchar();
        exit(EXIT_FAILURE);
    }
    /*store values of column , row and maxgrey in structures*/
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->col);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->row);
    SkipComments(pgmFile);
    fscanf(pgmFile, "%d", &data->max_gray);
    fgetc(pgmFile);

    /*store the higher byte in dynamic matrix*/
    data->matrix = allocate_dynamic_matrix(data->row, data->col);
    if (data->max_gray > 255)
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j) {
                hi = fgetc(pgmFile);
                lo = fgetc(pgmFile);
                data->matrix[i][j] = (hi << 8) + lo;
            }
    else
		/*store the lower byte in dynamic matrix*/
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j) {
                lo = fgetc(pgmFile);
                data->matrix[i][j] = lo;
            }
    /*close and return*/
    fclose(pgmFile);
    return data;
 
}
 
/*function to read PGM file*/ 
void writePGM(const char *filename, const PGMData *data)
{
    FILE *pgmFile;
    int i, j;
    int hi, lo;
    
	/*open a output file to write*/
	pgmFile = fopen(filename, "wb");
	if (pgmFile == NULL) {
        perror("cannot open file to write");
		getchar();
		getchar();
        exit(EXIT_FAILURE);
    }
 
	/*print version,column,row and maxgray values to output file*/
    fprintf(pgmFile, "P5\n ");
    fprintf(pgmFile, "%d %d\n ", data->col, data->row);
    fprintf(pgmFile, "%d \n", data->max_gray);
    
	/*print matrix values to file*/
    if (data->max_gray > 255) {
        for (i = 0; i < data->row; ++i) {
            for (j = 0; j < data->col; ++j) {
                hi = HI(data->matrix[i][j]);
                lo = LO(data->matrix[i][j]);
                fputc(hi, pgmFile);
                fputc(lo, pgmFile);
            }
 
        }
    } else {
        for (i = 0; i < data->row; ++i)
            for (j = 0; j < data->col; ++j) {
                lo = LO(data->matrix[i][j]);
                fputc(lo, pgmFile);
            }
    }
 
	/*close and free memory*/
    fclose(pgmFile);
    deallocate_dynamic_matrix(data->matrix, data->row);
}



PGMData * SobelMask(PGMData *image)
{
	int i,j;
	
	static int A[512][512];
	static int X[512][512] = {0};
	static int Y[512][512] = {0};
	static int B[512][512] = {0};

		/* Initializing 3x3 GY Sobel mask
   GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
   GX[1][0] = -2; GX[1][1] = 0; GX[1][2] = 2;
   GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;
   */

   /* Initializing 3x3 GX Sobel mask
   GY[0][0] =  1; GY[0][1] =  2; GY[0][2] =  1;
   GY[1][0] =  0; GY[1][1] =  0; GY[1][2] =  0;
   GY[2][0] = -1; GY[2][1] = -2; GY[2][2] = -1;
   */
   
   for (i=0; i < image->row - 1; i++)
	   for(j=0; j < image->col - 1; j++)
	   {
		   A[i][j] = image->matrix[i][j];
	   }

   for(i=0;i<image->row-1;i++)
	   for(j=0;j<image->col-1;j++)
	   {
		   /*skip pixels in 1st column & 1st row also last column and last row*/
		   if ((i==0)||(i==image->row-1)||(j==0)||(j==image->col-1))
            {
               X[i+1][j+1]=0;
               Y[i+1][j+1]=0;
            }
		   else
		   {
			   /*convolution of G * S */
			   X[i+1][j+1] = abs((A[i][j]+(2*A[i][j+1])+A[i][j+2]) - (A[i+2][j]+(2*A[i][j+1])+A[i][j+2]));
			   Y[i+1][j+1] = abs((A[i][j]+(2*A[i+1][j])+A[i+2][j])- (A[i][j+2]+(2*A[i+1][j+2])+A[i+2][j+2]));
						 
		   }

	   }

	   
		   for(i=0;i<512;i++)
			   for(j=0;j<512;j++)
			   {
					/*comparison of X and Y */
				   if(X[i][j] > Y[i][j])
						B[i][j] = X[i][j];
					else
						B[i][j] = Y[i][j];
			   }

	   for (i=0; i < image->row; i++)
			for(j=0; j < image->col; j++)
			{
					image->matrix[i][j]=B[i][j];
			}

	   return image;
}

PGMData * RobertMask(PGMData *image)
{
	int i,j;
	static int A[512][512];
	static int B[512][512]={255};
	/*
	Initializing 2x2 RX Robert mask
	RX[0][0]=1; RX[0][1]=0;
    RX[1][0]=0; RX[1][1]=-1;
	 Initializing 2x2 RY Robert mask
	RY[0][0]=0; RY[0][1]=1;
    RY[1][0]=-1; RY[1][1]=0;
	*/
	for (i=0; i < image->row; i++)
	   for(j=0; j < image->col; j++)
	   {
		   A[i][j] = image->matrix[i][j];
		   B[i][j] = A[i][j];
	   }

	for(i=0;i<image->row;i++)
		for(j=0;j<image->col;j++)
		{
			/*convolution of G * R */  
			B[i][j]= abs(A[i][j] - A[i+1][j+1]) + abs(A[i][j+1] - A[i+1][j]);		 
		}
	
		 for (i=0; i < image->row; i++)
			for(j=0; j < image->col; j++)
			{
			    image->matrix[i][j] = B[i][j];				
			}
	
	return image;
}

PGMData * LaplacianMask(PGMData *image)
{
	int i,j;

	static int A[512][512];
	static int B[512][512]={255};
	
	for (i=0; i < image->row; i++)
	   for(j=0; j < image->col; j++)
	   {
		   A[i][j] = image->matrix[i][j];
		   B[i][j] = A[i][j];
	   }

	for(i=0;i<image->row;i++)
		for(j=0;j<image->col;j++)
		{
			/*Laplacian operator*/  
			B[i+1][j+1] = abs(A[i+1][j] + A[i-1][j] + A[i][j+1] + A[i][j-1] - (4*A[i][j]));
		}
	
		 for (i=0; i < image->row; i++)
			for(j=0; j < image->col; j++)
			{
				
				image->matrix[i][j] = B[i][j];
			}
	
	return image;
}

PGMData * PrewittMask(PGMData *image)
{
	int i,j;
	
	static int A[512][512];
	static int X[512][512] = {0};
	static int Y[512][512] = {0};
	static int B[512][512] = {0};

		/* Initializing 3x3 PY Prewitt mask
   GX[0][0] = -1; GX[0][1] = 0; GX[0][2] = 1;
   GX[1][0] = -1; GX[1][1] = 0; GX[1][2] = 1;
   GX[2][0] = -1; GX[2][1] = 0; GX[2][2] = 1;
   */

   /* Initializing 3x3 PX Sobel mask
   GY[0][0] =  1; GY[0][1] =  1; GY[0][2] =  1;
   GY[1][0] =  0; GY[1][1] =  0; GY[1][2] =  0;
   GY[2][0] = -1; GY[2][1] = -1; GY[2][2] = -1;
   */
   
   for (i=0; i < image->row-1; i++)
	   for(j=0; j < image->col-1; j++)
	   {
		   A[i][j] = image->matrix[i][j];
	   }

   for(i=0;i<image->row-1;i++)
	   for(j=0;j<image->col-1;j++)
	       {
		   /*skip pixels in 1st column & 1st row also last column and last row*/
		   if ((i==0)||(i==image->row-1)||(j==0)||(j==image->col-1))
            {
               X[i+1][j+1]=0;
               Y[i+1][j+1]=0;
            }
		   else
	        {
			   /*Convolution of G * P */
			   X[i+1][j+1] = abs((A[i][j]+(A[i][j+1])+A[i][j+2]) - (A[i+2][j]+(A[i][j+1])+A[i][j+2]));
			   Y[i+1][j+1] = abs((A[i][j]+(A[i+1][j])+A[i+2][j])- (A[i][j+2]+(A[i+1][j+2])+A[i+2][j+2]));
		    }			 

	   }
		   for(i=0;i<512;i++)
			   for(j=0;j<512;j++)
			   {
					if(X[i][j] > Y[i][j])
						B[i][j] = X[i][j];
					else
						B[i][j] = Y[i][j];
			   }

	   for (i=0; i < image->row; i++)
			for(j=0; j < image->col; j++)
			{
					image->matrix[i][j]=B[i][j];
			}

	   return image;
}

PGMData * GradientMask(PGMData *image)
{
	int i,j;

	static int A[512][512];
	static int B[512][512]={255};
	
	for (i=0; i < image->row; i++)
	   for(j=0; j < image->col; j++)
	   {
		   A[i][j] = image->matrix[i][j];
		   B[i][j] = A[i][j];
	   }

	for(i=0;i<image->row;i++)
		for(j=0;j<image->col;j++)
		{
			 /*Gradient Operator*/
			  B[i][j] = abs(A[i][j] - A[i+1][j+1]);
		}
	
		 for (i=0; i < image->row; i++)
			for(j=0; j < image->col; j++)
			{
				
				image->matrix[i][j] = B[i][j];
			}
	
	return image;
}

/*Main program*/
void main()
{
	char filename[100],ch;
	int selection;
	const char * source_fileName;
    const char * modified_fileName = "output.pgm";

    PGMData pgm_image, pgm_orig;
    PGMData * data = &pgm_image;
    PGMData * orig = &pgm_orig;

	printf("\n************WELCOME***************");
	printf("\nEnter image file to read(pgm format) : ");
	scanf("%s",&filename);
	source_fileName=filename;
	
    printf("Reading image \"%s\" ...\n", source_fileName);
  do
	{		
    data = readPGM(source_fileName, data);
    orig = readPGM(source_fileName, orig);
	
     printf("\n*********Image Masking Options*************");
	 printf("\n\t1.Sobel's Mask");
	 printf("\n\t2.Robert's Mask");
	 printf("\n\t3.Laplacian");
	 printf("\n\t4.Prewitt");
	 printf("\n\t5.Gradient");
	 printf("\nEnter your selection here:");
	 scanf("%d",&selection);
	 switch (selection)
	 {
		case 1: data= SobelMask(orig);
				break;

		case 2: data = RobertMask(orig);
			    break;

		case 3: data = LaplacianMask(orig);
			    break;

		case 4: data = PrewittMask(orig);
			    break;

		case 5: data = GradientMask(orig);
			    break;
		default:  printf("Invaild choice\n Exiting program...");
		         exit(1);
		         break;
	 }

    writePGM(modified_fileName,data);
    printf("\nFile Writing complete");
    printf("\nopening outputfile......");
	/***** open a output file in openseeit ****/
    char program[100] = "OpenSeeIt.exe ";
    char *openme;
    openme= strcat(program,modified_fileName);

    system(openme);

	printf("\nDo you want to continue?(Y/N)\nEnter your answer here:");
	getchar();
	ch=getchar();
	}while(ch=='Y' || ch=='y');
	
}