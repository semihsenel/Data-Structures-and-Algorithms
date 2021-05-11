#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAX 256 //  max buffer size
#define NUM_OF_BOOKS 8 // number of books
#define MAX_NAME_LENGTH 25 // max length for books' names

// To find similar k people, i used linked list.
// Added elements sorted to linked listed
struct node{
	double similarity; // similarty for one reader
	int index; // index of reader
	struct node* next; // pointer of next node
};

typedef struct node NODE;

void readFile(); // function for reading file
double similarity(int, int); // function for calculate similarity
NODE* createNode(int, double); // function for creating node for linked list
void insertSorted(NODE**, int, double); // function for adding element in sorted way
NODE* Knearest(int, int, int); // function for searching most similar k reader
int recommend(int, int); // function for recommend a book
void testOperations(int); // function for test the program
double user_mean_calculate(int);

char books[NUM_OF_BOOKS][MAX_NAME_LENGTH]; // string array for books' names
int points[MAX][NUM_OF_BOOKS]; // matrix to keep point of each book for each user
int test[MAX][NUM_OF_BOOKS]; // matrix to keep point of each book for each test user

int NUM_OF_USERS; // number of users
int NUM_OF_TEST_USERS; // number of test users

int main(){
	int k;
	readFile(); // input from file
	
	printf("Please Enter K value : ");
	scanf("%d", &k);
	
	testOperations(k); // Testing the program for k=3
	
	
	return 0;
}

void testOperations(int k){
	NODE* users;
	int i;
	int j;
	int book_idx;
	printf("-------MOST SIMILAR READERS-------\n\n");
	for(i = 0; i < NUM_OF_TEST_USERS; i++){
		users = Knearest(i,k,1);
		printf("%d readers that the most similar to User NU%d:\n", k, i+1);
		printf("-----------------------------------\n");
		printf("|    Reader    |    Similarity    |\n");
		printf("-----------------------------------\n");
		for(j = 0; j < k; j++){
			printf("|    U%-5d    |    %-10.4lf    |\n", users->index+1, users->similarity);
			printf("-----------------------------------\n");
			users = users->next;
		}
		printf("\n");
	}
	printf("\n\n");
	
	printf("-------------BOOK RECOMMENDATIONS------------\n\n");
	printf("---------------------------------------------\n");
	printf("|    Reader    |        Book                |\n");
	printf("---------------------------------------------\n");
	for(i = 0; i < NUM_OF_TEST_USERS; i++){
		book_idx = recommend(i,k);
		printf("|    NU%-4d    |    %-20s    |\n", i+1, books[book_idx]);
		printf("---------------------------------------------\n");
	}
	
	
	
}

int recommend(int test_user, int k){
	// function for recommend a book
	double top; // top side of the equation
	
	double bottom; // bot side of the equation
	double test_user_mean; // mean for test user
	int i; // loop variable
	double max_value; // variable to keep max value
	int max_idx; // variable to keep index of max value
	double value; // temporary variable
	int p; // loop variable for books
	int other_user; // each user
	NODE* head = Knearest(test_user, k, 1); // head of sorted linked list of similarities
	int sum = 0; // variable to calculate mean
	int counter = 0; // variable to calculate mean
	
	for(i = 0; i < NUM_OF_BOOKS; i++){
		// calculating the mean of test_user books
		sum += test[test_user][i];
		if(test[test_user][i])
			counter++;
	}
	test_user_mean = (double)sum/counter;
	
	max_value = -1;
	p = 0;
	while(p < NUM_OF_BOOKS){
		// for each book that the test user haven't read yet 
		top = 0;
		bottom = 0;
		if(!test[test_user][p]){
			for(i = 0; i < k; i++){
				// calculate calculate the equation
				other_user = head->index;
				top += similarity(other_user, test_user)*(points[other_user][p]-user_mean_calculate(other_user));
				bottom += similarity(other_user, test_user);
				head = head->next;
			}
			value = (top/bottom) + test_user_mean;
			if(value > max_value){
				// keep max value and index
				max_value = value;
				max_idx = p;
			}
		}
		p++;
	}
	return max_idx; // return the book that have the max point
}

double user_mean_calculate(int user){
	// function the calculate mean
	int i;
	int sum = 0;
	int counter = 0;
	// for each book that the user read
	for(i = 0; i < NUM_OF_BOOKS; i++){
		sum += points[user][i];
		if(points[user][i])
			counter++;
	}
	return (double)sum/counter;
}

// Adým 2
NODE* Knearest(int user, int k, int mode){
	NODE* head = NULL;
	int i;
	for(i = 0; i < NUM_OF_USERS; i++)
		insertSorted(&head, i, similarity(i, user)); // to find max k similarity
		// i added them to linked list in sorted way
	if(mode){
		return head; // if mode == 1 than return head, do not print anything
	}
	// if mode == 1 then print similarities
	printf("%d readers that the most similar to User %d:\n", k,user);
	for(i = 0; i < k; i++){
		printf("User %2d -> similarty : %.4lf\n", head->index, head->similarity);
		head = head->next;
	}
}

// Adým 1
double similarity(int user, int test_user){
	// function to calculate similarity
	double top = 0; // top side of the equation
	double bottom = 0; // bot side of the equation
	double bottom2 = 0; // bot side of the equation
	int common_books[NUM_OF_BOOKS]; // array for common books' indexes
	int idx = 0; // counter for common books
	int sum; // variable to calculate mean
	int sum2; // variable to calculate mean
	int i; // loop variable
	int p; // loop variable
	double user_mean; // user mean
	double test_user_mean; // test user mean
	
	for(i = 0; i < NUM_OF_BOOKS; i++){
		if(points[user][i] && test[test_user][i])
			common_books[idx++] = i;
	}
	// found common books' indexes
		
	sum = 0;
	sum2 = 0;
	for(i = 0; i < idx; i++){
		p = common_books[i];
		sum += points[user][p];
		sum2 += test[test_user][p];
	}
	// calculated mean of common books for user and test user
	
	user_mean = (double)sum/idx;
	test_user_mean = (double)sum2/idx;
	
	for(i = 0; i < idx; i++){
		p = common_books[i];
		top += (points[user][p] - user_mean)*(test[test_user][p] - test_user_mean);
		bottom += pow(points[user][p] - user_mean, 2);
		bottom2 += pow(test[test_user][p] - test_user_mean, 2);
	}
	bottom = sqrt(bottom) * sqrt(bottom2);
	// calculated the equation and return the result
	return top / bottom;
	
}

void readFile(){
	FILE* fp; // file pointer
	char buffer[MAX]; // buffer for each line
	char* word; // variable for strtok
	int i = 0; // loop variable
	int j; // loop variable
	fp = fopen("RecomendationDataSet.csv","r"); // open the file in read mode
	if(!fp){ // if does not open then terminate the program
		printf("Dosya Acilamadi!!");
		exit(0);
	}
	
	fgets(buffer, MAX, fp); 
	word = strtok(buffer, "\n");
	word = strtok(word, ",");
	while(word){
		// Read the first line
		// first line keeps name of books
		strcpy(books[i], word);
		i++;
		word = strtok(NULL, ",");
	}
	
	
	fgets(buffer, MAX, fp);
	word = strtok(buffer, "\n");
	i = 0;
	while(word[0] == 'U'){
		// then read the lines that starts with 'U'
		// because the lines that contains points of users start with 'U*' 
		word = strtok(word, ",");
		word = strtok(NULL, ",");
		// i don't use each word because it is number of user
		j = 0;
		while(word){
			if(word[0] == ' '){
				// if there is no points then it is 0
				points[i][j] = 0;
			}
			else{
				// if there is a point than convert it to integer
				points[i][j] = word[0] - '0';
			}
			j++;
			word = strtok(NULL, ",");
		}
		i++;
		// keep going it for all lines that start with 'U*'
		fgets(buffer, MAX, fp);
		word = strtok(buffer, "\n");
	}
	
	NUM_OF_USERS = i; // assign the number of users
	// all the remain lines contain the points of test users
	i = 0;
	while(!feof(fp)){
		// read the points like below
		fgets(buffer, MAX, fp);
		word = strtok(buffer, "\n");
		word = strtok(word, ",");
		j = 0;
		while(word){
			if(word[0] != 'N'){
				// don't use the first word because it is number of the user
				if(word[0] == ' '){
					// if there is no points then it is 0
					test[i][j] = 0;
				}
				else{
					// if there is a point than convert it to integer
					test[i][j] = word[0] - '0';
				}
				j++;
			}
			word = strtok(NULL, ",");
		}
		i++;
	}
	
	NUM_OF_TEST_USERS = i; // assign the number of test users
	
	fclose(fp); // close the file
}

NODE* createNode(int index, double similar){
	NODE* newNode;
	newNode = (NODE*) malloc(sizeof(NODE));
	// allocation for new node
	newNode->index = index;
	newNode->similarity = similar;
	// assign values
	newNode->next = NULL;
	return newNode; // return new node
}

void insertSorted(NODE** head, int index, double similarity){
	// function to add element to linked list in sorted way
	NODE* newNode = createNode(index, similarity); // created a node that consists of index and similarity
	NODE* temp; // temporary variable in order not to change head
	if(!(*head) || (*head)->similarity <= newNode->similarity){
		// if head is NULL or newNode's greater equal than newNode's similarty then add element to front
		newNode->next = *head;
		*head = newNode;
		return;
	}
	
	// else
	temp = *head;
	while(temp->next != NULL && newNode->similarity < temp->next->similarity){
		// go until to find less equal element than newNode
		temp=temp->next;
	}
	// when find add element before that element
	newNode->next = temp->next;
	temp->next = newNode;	
}


















