/*
GAURAV DESHMUKH
COSC 5356 COMPUTER VISION
ASSIGNMENT #1

***  A PROGRAM TO READ PGM FILE 
  *  TO DO FOLOWING:
  *  1. CREATE HISTOGRAM
  *  2. PERFORM LINEAR STRECHING TRANSFORMATION
  *  3. PERFORM HISTOGRAM EQUALIZATION    ***

*/

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <string.h>

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

/*Structure to store Histogram*/
int Histogram[255]={0};
/*Global variables*/
int minGreyVal=0,maxGreyVal=255;

int numofpixels = 0;

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

/*function to create Histogram out of PGM data*/
void createHistogram(const PGMData *data)
{
	int i,j,greyVal=0,freq=0;

	        /*create histogram by counting frequency of each grey value from input matrix*/
	        for (i = 0; i < data->col; ++i)
			   for (j = 0; j < data->row; ++j) 
			   {
				   for(greyVal=0; greyVal < 256; greyVal++)
				   {
					   if(data->matrix[i][j] == greyVal)
					   {
						   Histogram[greyVal]=Histogram[greyVal]+1;
					   }
				   }
               }
}

/*function to display histogram*/
void showHistogram()
{
	int i,j=0;
	printf("\nGrayValue\tFrequency\n");
	for(i=0;i<256;i++)
	{
		printf("%d\t\t%d\t",i,Histogram[i]);
		if(Histogram[i] < 55)
		{
		    for(j=Histogram[i]; j > 0; j--)
				printf("*");
		}
		else
		{
			for(j = 0 ;j < 55;j++)
			printf("*");
		}
		printf("\n");
	}
}

/*function to get minimum and maximum value*/
void getMinMax(int array[255])
{
	int i,cnt=0,maxcnt=0;
	for(i=0;i<256;i++)
	{
		if(array[i] != 0 && cnt == 0)
		{
			minGreyVal=i;
			cnt++;
		}
	}

	for(i=0;i<255;i++)
	{
		if(array[i] != 0)
		{
			maxcnt=i;
		}
	}
	maxGreyVal=maxcnt;
}

/*function to perform linear streching*/
void linearstrech(PGMData *data)
{
	int i,j,Omin=0,Omax=0;
	float y=0,m=0,c=0,diff=0;
	int choice;
	printf("\n\tLinear Streching");
	printf("\n\t\t1.Darkness Streching");
	printf("\n\t\t2.Brightness Streching");
    printf("\n\t\t3.Mid-Range Streching");
	printf("\nEnter your choice: ");
	scanf("%d",&choice);
	switch(choice)
	{
	   case 1:  printf("\nInput desired Maximum Grey Value:");
	            scanf("%d",&Omax);
			    if(Omax >= maxGreyVal)
			    {
				   printf("\nValue greater than input maximum");
				   printf("Setting value to input maximum ... for optimum performance");
				   Omax = maxGreyVal;
			    }
			    break;

	   case 2:  printf("\nInput desired Minimum Grey Value:");
	            scanf("%d",&Omin);
			    if(Omin <= maxGreyVal)
			    {
				   printf("\nValue lesser than input maximum");
				   printf("Setting value to input maximum ... for optimum performance");
				   Omin = maxGreyVal+1;
				   Omax = 255;
			    }
			    break;

	   case 3:  printf("\nInput desired Minimum Grey Value:");
	            scanf("%d",&Omin);
	            printf("\nInput desired Maximum Grey Value:");
	            scanf("%d",&Omax);
			    break;

	   default: printf("Invaild choice\n Exiting program...");
		        exit(1);
				break;

	}
	/*Linear streching using y=mx+c*/
	m = labs((Omax - Omin) / (maxGreyVal - minGreyVal));
	c = labs(Omin - (m*minGreyVal));

	        for (i = 0; i < data->col; i++)
			   for (j = 0; j < data->row; j++) 
			   {
				   if(data->matrix[i][j] <= minGreyVal)
					   {
						   y = Omin;
						   data->matrix[i][j]=int(y);
					   }
				   else if(data->matrix[i][j] > maxGreyVal)
				      {
							y = Omax;
				      }
				   else
				     {
						 y = labs((m * data->matrix[i][j]) + c);
								data->matrix[i][j]=int(y);
				     }
			   }
			 
}

/*function to clear histogram*/
void flushistogram(){
	for (int i=0; i<=255; i++)
	{
		Histogram[i]=0;
	}
}

/*function for histogram equalization*/
void histogramequalizer(PGMData *image)
{
	int i=0,j=0,greyVal=0;
	double numofpixels=0;
	for(i=0;i<256;i++)
		
		numofpixels=numofpixels+Histogram[i];
	//Probability Mass function table
	
	double prt[256]={0};
	prt[0]=(Histogram[0]/numofpixels);
    for(i=1;i<256;i++)
		prt[i]=prt[i]+(Histogram[i]/numofpixels);
 
	//CumulativeDistributionFunction
	double cdf[256]={0};

	cdf[0] = prt[0];

	for(i=1;i<256;i++)
	{
        cdf[i]=cdf[i]+(prt[i]+cdf[i-1]);
	}

	/*rewriting values to matrix according to distribution calculated*/
	for (i = 0; i < image->col; i++)
        for (j = 0; j < image->row; j++) 
		{
				    for(greyVal=0; greyVal < 256; greyVal++)
				   {
					   if(image->matrix[i][j] == greyVal)
					   {
						   image->matrix[i][j]=cdf[greyVal]*(maxGreyVal-1);
					   }
				   }
		}

		flushistogram();
		createHistogram(image);
	    showHistogram();
				 
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
	do
	{
	printf("\n************WELCOME***************");
	printf("\nEnter image file to read(pgm format) : ");
	scanf("%s",&filename);
	source_fileName=filename;
	
    printf("Reading image \"%s\" ...\n", source_fileName);
 
	
    data = readPGM(source_fileName, data);
    orig = readPGM(source_fileName, orig);
	flushistogram();
	createHistogram(orig);
	showHistogram();
	getMinMax(Histogram);
	
	printf("\nMinGreyVal = %d",minGreyVal);
	printf("\nMaxGreyVal = %d",maxGreyVal);
	
     printf("\n*********Image Transformation Options*************");
	 printf("\n\t1.Linear Streching");
	 printf("\n\t2.Histogram Equalization");
	 printf("\nEnter your selection here:");
	 scanf("%d",&selection);
	 switch (selection)
	 {
		case 1:  linearstrech(data);
	             flushistogram();
	             createHistogram(data);
	             showHistogram();
				 break;

		case 2:  histogramequalizer(data);
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