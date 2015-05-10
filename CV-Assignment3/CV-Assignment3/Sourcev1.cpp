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


void near(PGMData * Image)
{
	
	int newrow, newcol;
	float sx,sy;
	int x,y;
	printf("\n\t *****Nearest Neighbour ****** ");
	printf("\nEnter the X-scale factor between 0.5 and 2.0");
	scanf("%f0.2",&sx);
	printf("\nEnter the Y-scale factor between 0.5 and 2.0");
	scanf("%f0.2",&sy);
	
	newrow = int((sx) * orig->row);
	newcol = int((sy) * orig->col);

	newi=&newImage;
	newi->row=newrow;
	newi->col=newcol;
	newi->max_gray=orig->max_gray;
	newi->matrix = allocate_dynamic_matrix(newi->row,newi->col);

	printf("The Scaled Image is of size %d X %d",newi->row,newi->col);
	
	if(sx > 1 || sy > 1)
	{
	for(int m=1;m<(newi->row/sx)-1;++m)
		for(int n=1;n<(newi->col/sy)-1;++n)
		{
				{
					int ax,bx,cx,dx;
	     			int ay,by,cy,dy;
					ax = sx * m;
					ay = sy * n;
					bx = ax - 1; 
					by = ay;
					cx = ax;
					cy = ay - 1;
					dx = bx;
					dy = cy;
					newi->matrix[ax][ay]=Image->matrix[m][n];
					newi->matrix[bx][by]=Image->matrix[m][n];
					newi->matrix[cx][cy]=Image->matrix[m][n];
					newi->matrix[dx][dy]=Image->matrix[m][n];

				/*	newi->matrix[ax][n]=Image->matrix[m][n];
				    newi->matrix[m][ay]=Image->matrix[m][n];
				    newi->matrix[ax][ay]=Image->matrix[m][n]; */
				}
		}
	}
	else 
	{
		for(int m=1;m<(newi->row/sx)-1;++m)
		      for(int n=1;n<(newi->col/sy)-1;++n)
				{
					int x,y;
					x = sx * m;
					y = sy * n;
					newi->matrix[x][y]=Image->matrix[m][n];	
				}
  }
}

void bipol(PGMData* Image)
{
	int newrow, newcol;
	float sx,sy;
	float nx,ny;
	float dx,dy;
	float dx1,dy1,dx2,dy2;
	float d;
	int x,y,u,v;
	int u1,u2,v1,v2;
	printf("\n\t *****Bilinear Interpolation ****** ");
	printf("\nEnter the X-scale factor between 0.5 and 2.0");
	scanf("%f0.2",&sx);
	printf("\nEnter the Y-scale factor between 0.5 and 2.0");
	scanf("%f0.2",&sy);
	
	newrow = int((sx) * orig->row);
	newcol = int((sy) * orig->col);

	newi=&newImage;
	newi->row=newrow;
	newi->col=newcol;
	newi->max_gray=orig->max_gray;
	newi->matrix = allocate_dynamic_matrix(newi->row,newi->col);

	printf("The Scaled Image is of size %d X %d",newi->row,newi->col);
	/*
	for(int m=1;m<(newi->row/sx)-1;++m)
		for(int n=1;n<(newi->col/sy)-1;++n)
		{
				{
					nx = sx * m;
					ny = sy * n;
					
					u1 = m-1;
					u2 = m +1;
					v1 = n-1;
					v2 = n+1;

					dx1 = abs(nx - u1);
					dx2 = abs(nx - u2);
					dy1 = abs(ny - v1);
					dy2 = abs(ny - v2);

					if(dx1 > dx1)
					{
						dx = dx1;
					}
					else
					{
					   dx = dx2;
					}
					if(dy1 > dy1)
					{
						dy = dy1;
					}
					else
					{
					   dy= dy2;
				
					}

					if(dx > dy)
						d = dx;
					else 
						d = dy;

					int ax,bx,cx,dx;
				    int ay,by,cy,dy;
					ax = int(nx);
					ay = int(ny);
					bx = ax - 1; 
					by = ay;
					cx = ax;
					cy = ay - 1;
					dx = bx;
					dy = cy;
					
					newi->matrix[ax][ay]= Image->matrix[u][v];
					newi->matrix[bx][by]=Image->matrix[u][v];
					newi->matrix[cx][cy]=Image->matrix[u][v];
					newi->matrix[dx][dy]=Image->matrix[u][v];

				}
		}
		printf("\n");

		for(int i=0;i<20;i++)
		{
			for(int j=0;j<20;j++)
			{
				printf("%d",newi->matrix[i][j]);
				printf("\t");
			}
			printf("\n");
		}
*/
}

/*Main program*/
void main()
{
	char filename[100],ch;
	int selection;
	const char * source_fileName;
    const char * modified_fileName = "output.pgm";

    

	printf("\n************WELCOME***************");
	printf("\nEnter image file to read(pgm format) : ");
	scanf("%s",&filename);
	source_fileName=filename;
	
    printf("Reading image \"%s\" ...\n", source_fileName);
  do
	{		
    orig = readPGM(source_fileName, orig);

	
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

	writePGM(modified_fileName,newi);
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