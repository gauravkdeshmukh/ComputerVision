/*
GAURAV DESHMUKH
COSC 5356 COMPUTER VISION
ASSIGNMENT #4

***  ROTATING A GIVEN IMAGE 
*/

/*Includes*/
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <string.h>
#include <math.h>

# define PI           3.14  /* pi */
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

void rotate(PGMData* Image)
{
    int newrow, newcol;
	int deg;
	float r=0;
	float o=0,sx=0,sy=0,max=0;
	printf("\n\t ***** Image Rotation ****** ");
	printf("\nEnter the value of thoeta: ");
	scanf("%d",&deg);
    /*coverting to radians*/
	o = (2*PI*deg)/360;
	sx = ((Image->row * cos(o)) - (Image->col * sin(o)));
	sy = ((Image->col * cos(o)) + (Image->row * sin(o)));
	if(sx > sy)
		max =sx;
	else
		max=sy;

	newcol = max;
	newrow = max;
	r = max / Image->col;
	newj=&newImage2;
	newj->row=newrow;
	newj->col=newcol;
	newj->max_gray=orig->max_gray;
	newj->matrix = allocate_dynamic_matrix(newj->row,newj->col);
	printf("The Scaled Image is of size %d X %d",newj->row,newj->col);
    
	int u=0,v=0;
	int dx=0,dy=0;
	float m=0,n=0;
	for(int x=0;x<Image->row;x++)
		{
			for(int y=0;y<Image->col;y++)
				{
					u = abs((x*cos(o) + y*sin(o)));
					v = abs((y*cos(o) - x*sin(o)));
					u = int((u/r));
					v =  int((v/r));
					newj->matrix[x][y]=Image->matrix[u][v];
				}
		}
					
}

/*Main program*/
void main()
{
	char filename[100],ch,ans;
	int selection;
	const char * source_fileName;
    const char * modified_fileName = "output.pgm";
	const char * inte_fileName = "intermediate.pgm";
	do
	{
	printf("\n************WELCOME***************");
	printf("\nEnter image file to read(pgm format) : ");
	scanf("%s",&filename);
	source_fileName=filename;
    printf("Reading image \"%s\" ...\n", source_fileName);
    orig = readPGM(source_fileName, orig);
     
	rotate(orig);
			
	writePGM(modified_fileName,newj);
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