/*
GAURAV DESHMUKH
COSC 5356 COMPUTER VISION
ASSIGNMENT #3

***  SCALING GIVEN IMAGE USING:
	1. Nearest Neighbour
	2. Bilinear Interpolation
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

PGMData newImage;
PGMData * newi;

PGMData ImageData;
PGMData * imgdata;

PGMData newImage2;
PGMData * newj;

PGMData  pgm_orig;
PGMData * orig = &pgm_orig;
	
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

/*Nearest Neighbor Algorithm*/
void near(PGMData * Image)
{
	
	int newrow, newcol;
	float sx,sy;
	//int x=0,y=0;
	printf("\n\t *****Nearest Neighbour ****** ");
	printf("\nEnter the X-scale factor between 0.5 and 2.0\t :");
	scanf("%f0.2",&sx);
	printf("\nEnter the Y-scale factor between 0.5 and 2.0\t :");
	scanf("%f0.2",&sy);
	newcol = int((sx) * imgdata->row);
	newrow = int((sy) * imgdata->col);
	newi=&newImage;
	newi->row=newrow;
	newi->col=newcol;
	newi->max_gray=imgdata->max_gray;
	newi->matrix = allocate_dynamic_matrix(Image->row,newi->col);
	newj=&newImage2;
	newj->row=newrow;
	newj->col=newcol;
	newj->max_gray=orig->max_gray;
	newj->matrix = allocate_dynamic_matrix(newj->row,newj->col);
	printf("The Scaled Image is of size %d X %d",newj->row,newj->col);
	/*Calculation for columns*/
	float x=0,y=0;
	int u=0,v=0;
	for(int m=0;m<Image->row;m++)
		{
			for(int n=0;n<newi->col;n++)
				{
					/*Rounding off column number*/
					y = (1/sx)*n;
					v = int(y);
					newi->matrix[m][n]=Image->matrix[m][v];	
				}
		}
		/*Calculation for rows*/
	     for(int m=0;m<newi->row;m++)
			{
				/*Rounding off row number*/
				x = (1/sy) * m;
				u = int(x);
				for(int n=1;n<newj->col;n++)
				{			
					newj->matrix[m][n]=newi->matrix[u][n];	
				}
			}
}
/*Bilinear Interpolation Algorithm*/
void bipol(PGMData* Image)
{
int newrow, newcol;
	float sx,sy;

	printf("\n\t ***** Bilinear Interpolation ****** ");
	printf("\nEnter the X-scale factor between 0.5 and 2.0\t :");
	scanf("%f0.2",&sx);
	printf("\nEnter the Y-scale factor between 0.5 and 2.0\t :");
	scanf("%f0.2",&sy);
	newcol = int((sx) * imgdata->row);
	newrow = int((sy) * imgdata->col);
	newi=&newImage;
	newi->row=newrow;
	newi->col=newcol;
    newi->max_gray=orig->max_gray;
	newi->matrix = allocate_dynamic_matrix(Image->row,newi->col);
	newj=&newImage2;
	newj->row=newrow;
	newj->col=newcol;
	newj->max_gray=orig->max_gray;
	newj->matrix = allocate_dynamic_matrix(newj->row,newj->col);
	printf("The Scaled Image is of size %d X %d",newj->row,newj->col);

	float x=0,y=0;
	float dx,dy,dyL,dxL;
	float value;
	int u1=0,v1=0,u2=0,v2=0;
	for(int m=1;m<Image->row-1;m++)
		{
			for(int n=1;n<newi->col-1;n++)
				{
					/*Interpolating column number*/
					y = (1/sx)*n;
					dy = abs(y - floor(y));
					dyL = 1 - dy;
					v1 = floor(y);
					v2 = ceil(y);
					value = (dyL*Image->matrix[m][v1]) + (dy*Image->matrix[m][v2]);
					newi->matrix[m][n]= value;	
				}
		}
			
	     for(int m=1;m<newi->row-1;m++)
			{
				/*Interpolating row number*/
				x = (1/sy) * m;
				dx = abs(x - floor(x));
				dxL = 1 - dx;
				u1 = floor(x);
				u2 = ceil(x);
				for(int n=1;n<newj->col-1;n++)
				{			
					value = (dxL*newi->matrix[u1][n]) + (dx*newi->matrix[u2][n]);
					newj->matrix[m][n] = value;
				}
			}

}

/*Main program*/
void main()
{
	char filename[100],ch,ans;
	int selection,width,height;
	const char * source_fileName;
    const char * modified_fileName = "output.pgm";
	const char * inte_fileName = "intermediate.pgm";
	printf("\n************WELCOME***************");
	printf("\nEnter image file to read(pgm format) : ");
	scanf("%s",&filename);
	source_fileName=filename;
    printf("Reading image \"%s\" ...\n", source_fileName);
  do
	{		
     orig = readPGM(source_fileName, orig);
	 printf("\nDo you want to Enter Window size for the image?(Y/N)");
	 printf("\nEnter your ans here:");
	 getchar();
	 scanf("%c",&ans);

	 if(ans == 'y' || ans =='Y')
	 {
		 printf("\nEnter width < %d of window here:",orig->row);
		 scanf("%d",&width);
		 printf("\nEnter height < %d of window here:",orig->col);
		 scanf("%d",&height);
		 imgdata=&ImageData;
		 imgdata->row=width;
		 imgdata->col=height;
		 imgdata->max_gray=orig->max_gray;
		 imgdata->matrix = allocate_dynamic_matrix(imgdata->row,imgdata->col);
		 for(int m=0;m<width;m++)
			 for(int n=0;n<height;n++)
				{
					imgdata->matrix[m][n]=orig->matrix[m][n];	
				}

	 }
	 else
		 imgdata = orig;

     printf("\n*********Image Scaling Options*************");
	 printf("\n\t1.Nearest Neighbour Method");
	 printf("\n\t2.Bilinear Interpolation");
	 printf("\nEnter your selection here:");
	 scanf("%d",&selection);
	 switch (selection)
	 {
		case 1: near(orig);
				break;

		case 2: bipol(orig);
			    break;

		default:  printf("Invaild choice\n Exiting program...");
		         exit(1);
		         break;
	 }

	writePGM(modified_fileName,newj);
	writePGM(inte_fileName,imgdata);
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