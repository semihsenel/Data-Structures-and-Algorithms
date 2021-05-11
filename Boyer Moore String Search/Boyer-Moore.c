#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX 1024 // Max Test Size


void readFromFile(char*);// Read from file
void readFromTerminal(char*); // Read from terminal
void shiftTable(char*, int*, int); // Shift table for boyer-moore algorithm
void search(char*, char*, int*, char); // search algorithm
void replace(char*, char*, char*, int*); // replace algorithm

int main(){
	struct timespec start,stop;// to calculate the time
    int i=0,type;      
    int index_arr[MAX];// the keep the indexes of the substrings
	char pat[MAX]; // to keep the substring we search
    char text[MAX]; // to keep the text
    char replc[MAX]; // to new substring
    char result;
    
    printf("Enter 1 to input from terminal\nEnter 2 to input from a file\n");
    scanf("%d",&type);
    // Ask to user for input 

    if(type == 1) readFromTerminal(text);
    else if(type == 2) readFromFile(text);
	else{printf("Error!"); exit(0);}
    // Call function according to user's choice
    

    printf("Find : ");
    scanf("%s", pat);
    // ask for the substring that will search
    
    printf("Replace : ");
    gets(replc); // fgets and gets fucntion takes the input from last scanf. Takes \n as input
    fgets(replc,MAX,stdin);// ask for input to replace the substring
    strtok(replc,"\n");
    // get rid of the \n character that comes with fgets function
    
    printf("Is it case sensitive ?\nEnter Y or N : ");
    scanf("%c",&result);
    // Ask for is it case sensitive or not
    clock_gettime(CLOCK_MONOTONIC,&start);
    // get the system time to calculate the execution time
    search(pat,text,index_arr,result);
    // call search func
    printf("Text : %s\n", text);
	replace(text,pat,replc,index_arr);
    // call replace func
    printf("New Text : %s\n",text);
    
    while(index_arr[i] != -1){
    	i++;	
	}
    // count how much string found

	printf("Found and Replaced : %d\n",i);
	clock_gettime(CLOCK_MONOTONIC,&stop);
	printf("Running Time : %ld nanosecond", ((long int)(stop.tv_sec-start.tv_sec)*1000000000 + (stop.tv_nsec-start.tv_nsec)));
    // calculate time time and print

    return 0;
}

void readFromFile(char* text){
    char file[32];
    FILE* fp;
    int i;

    printf("Enter the name of the text file : ");
    scanf("%s", file);
    fp = fopen(file, "r");
    // open the file
    
    if(!fp){
        printf("Error when openning the file.");
        exit(0);
    }
    // Throw error when couldn't open
    
    for(i = 0; !feof(fp); i++){
        fscanf(fp,"%c",&text[i]);
    }
    // read data

	fclose(fp);
    // close file

}

void readFromTerminal(char* text){
	
	printf("Enter the text : ");
	gets(text);
	fgets(text,MAX,stdin);
    // take input as text from terminal 
	
}

void shiftTable(char PAT[], int table[], int size) 
{ 
    // create a shift table for the algorithm
    int i; 
  
    for (i = 0; i < MAX; i++) 
        table[i] = -1; 
    // first fill the table with -1

    for (i = 0; i < size; i++)
        table[(int) PAT[i]] = i; 
    // then fill the peace which is exist in pat

} 

void search(char PAT[],  char TEXT[], int* index_arr, char result) 
{	
    int m = strlen(PAT); 
    int n = strlen(TEXT);
    // calculate the length of strings
    int i = 0,k = 0, skip = 0,j;
  
    int table[MAX]; 
    // declare a table
    shiftTable(PAT, table, m);
    // fill the table 

    int range = n-m;

    while(i <= range){
        j = m-1;
        
        if(result == 'Y' || result == 'y'){
            while((j >= 0) && (PAT[j] == TEXT[i+j])){
                j--;
                // if it case sensitive
                // then look for a exact match
            }
        }
        else{
            while ((j >= 0) && ( (PAT[j] == TEXT[i+j]) || (abs(PAT[j] - TEXT[i+j]) == 32) ))
            {
                j--;
                // if it is not case sensitive
                // then the difference between a upper case and a lower case is 32. (in ascii table)
                // look for a match
            }
            
        }  

        if(j >= 0){
            // when couldn't find any match.

        	if(TEXT[i+j] < 92){
        		skip = j - (table[TEXT[i+j] + 32] >= table[TEXT[i+j]] ? table[TEXT[i+j] + 32] : table[TEXT[i+j]]);
                
			}
			else{
				skip = j - (table[TEXT[i+j] - 32] >= table[TEXT[i+j]] ? skip = table[TEXT[i+j] - 32] : table[TEXT[i+j]]);

            // Due to I may used case ignored match. It may lead to a problem.
            // I choose skip by finding upper case or lower case in the table is greater.
            // for example
            // table['a'] = 12 but case ignored then i may be search for table['A']
            // when table['A'] is -1 it is a problem
            // so i choice the greater one
			}

            if(skip <= 0){
                skip = 1;
            }
            // result of the calculations if the skip <= 0 
            // give value 1 to skip
        }
        else{
            // when found the match
            index_arr[k] =  i; // keep index in the array
            k++; 
            if((i+m) < n){
            	skip = m - table[TEXT[i+j]];
                // calculate the skip
			}
			else{
				skip = 1;
			}
        }
        i += skip;
    }
    index_arr[k] =  -1;// to understand where the array is over
}

void replace(char* text, char* pat, char* new, int* index){

    int newSize = strlen(new);
    int m = strlen(pat), n = strlen(text);
    int i,j,k,diff;


    if(newSize < m){
        // when the new substring is smaller than the old one
        // calculate the difference
        diff = m - newSize;
        for(i = 0; index[i] != -1; i++){
            for(j = index[i]+newSize - i*diff; j < n; j++){
                text[j] = text[j+diff];
            }
        }
        // slide the text for the new version

        for(i = 0; index[i] != -1; i++){
            for(j = index[i] - i*diff, k = 0; k < newSize; j++, k++){
                text[j] = new[k];
            }
        }
        // place the new substring 

    }
    else{
        // when the new substring is grater than the old one
        diff = newSize - m;
        for(i = 0; index[i] != -1; i++){
            for(j = n+diff; j >= index[i] + m; j--){
                text[j] = text[j-diff];
            }
            n = strlen(text);
        }
        // slide the text to make room


        for(i = 0; index[i] != -1; i++){
            for(j = index[i] + i*diff, k = 0; k < newSize; j++, k++){
                text[j] = new[k];
            }
        }
        // place the new substring
        

    }
    

}
