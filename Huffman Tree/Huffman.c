//Huffman Tree Coding
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 250

// Linked List node. Also it will be use for tree 
typedef struct node{

	char letter;
	int frequency;
	struct node *left,*right,*next;

}NODE;

//Function to read the text from terminal
void readText(char*);
//Function to create a new node
NODE* createNode(char);
//Function to create a linked list from letters and frequency
NODE* addItems(char*);
//Function to sort the linked list
void insertionSort(NODE**);
//Function to add element sorted. It is being used for insertion sort
void sortedInsert(NODE**, NODE*);
//Function to convert the linked list to tree
void convertTree(NODE**);
//Function to print the linked list
void printList(NODE*);
//Function to level order print to tree
void printTree(NODE*);
//Function to print the every level of the tree
void printLevel(NODE*, int);

int main(){

	NODE* head = NULL;
	char text[SIZE];
	readText(text);
	head = addItems(text);
	NODE* i;
	printList(head);
	printf("********************************\n");
	insertionSort(&head);
	printList(head);
	printf("\n\n");
	convertTree(&head);
	printTree(head);

	return 0;
}

void readText(char *text){
	FILE* fp;
	int val;
	char filename[50];
	printf("Enter 1 to input from terminal\nEnter 2 to input from a file\n");
	scanf("%d",&val);
	while(val != 1 && val!= 2){
		printf("Incorrect input. Try Again!!\n");
		scanf("%d",&val);		
	}
	if(val == 1){
	printf("Enter the text : ");
	fgets(text,SIZE,stdin);
	fgets(text,SIZE,stdin);
	/*fgets function gets the "\n" character at the end of the string.
	So I used strtok to delete the "\n" character						*/
	strtok(text,"\n");
	}
	else{
		printf("Enter the name of the input file : ");
		scanf("%s",filename);
		fp = fopen(filename,"r");
		fgets(text,SIZE,fp);
		fclose(fp);
	}
	
	
}

NODE* createNode(char letter){
	NODE* temp;

	// Allocate new node
	temp = (NODE*) malloc(sizeof(NODE));

	//Initialize the node
	temp->letter = letter;
	temp->frequency = 1;
	temp->next = temp->left = temp->right = NULL;
	
	//return the new node
	return temp;
}

NODE* addItems(char* text){
	// create new head
	NODE *head = NULL;

	NODE *temp;
	
	int i;
	// loop for every letter of the text
	for(i = 0; i < strlen(text); i++){
		temp = head;
		
		/* Loop for letters
		If letter already exists increase the frequency
		If not exists create a new node				*/
		while(temp != NULL && temp->letter != text[i]){
			temp = temp->next;
		}

		if(temp == NULL){
			temp = createNode(text[i]);
			temp->next = head;
			head = temp;
		}
		else{
			temp->frequency++;
		}

	}
	
	//return new head
	return head;

}

void insertionSort(NODE** head){

	//Initialize the new sorted list
	NODE* newHead = NULL;
	NODE *temp = *head;

	//loop for every element of list
	while(temp != NULL){
		NODE* next = temp->next;
		
		//insert temp in sorted linked list
		sortedInsert(&newHead,temp);

		temp = next;
	}	
	
	// Update the head reference
	*head = newHead;

}

void sortedInsert(NODE** head, NODE* node){
	NODE* temp;
	temp = *head;

	//List is empty or the number is smallest
	if((*head) == NULL || node->frequency <= (*head)->frequency){
		
		node->next = *head;
		*head = node;
	}
	else{
		//go for find the greater number
		while(temp->next != NULL && temp->next->frequency <= node->frequency){
			temp = temp->next;
		}
		node->next = temp->next;
		temp->next = node;
	}


}

void printList(NODE *head){
	NODE* i;
	// from head to the end printing the list
	// letter : frequency \n
	for(i = head; i != NULL; i = i->next){
		printf("%c : %d\n", i->letter, i->frequency);
	}


}

void convertTree(NODE** head){
// Linked List to Tree	
	NODE *i,*j;
	NODE* temp;
	i = *head;
// If head == NULL then do nothing
	if(i == NULL){
		return;
	}
// from beginning to the element before the end	
	while(i->next != NULL){
		j = i;
		// New node that is sum of frequencies of 2 elements 
		temp = createNode(j->letter);
		temp->frequency = j->frequency + j->next->frequency;
		// There will be no letter because it is Huffman tree
		temp->letter = NULL;
		// Insert the new node in sorted
		sortedInsert(head,temp);
		// Other 2 elements will be the left and right node of the new node
		temp->left = j;
		temp->right = j->next;
		
		// we used 2 elemenets so we jump the next element
		i = i->next->next;
		
		// new head will be i because we used 2 elements and moved them
		*head = i;
	}
}

int height(NODE* head) 
{	
// Calculate the height of the tree

	// If head == NULL we reached the leaf
    if (head==NULL) 
        return 0; 
    else
    { 
        /* compute the height of each subtree */
        int lheight = height(head->left); 
        int rheight = height(head->right); 
  
        /* use the larger tree */
        if (lheight > rheight){
            return (lheight+1); 
        }
        else{
        	return (rheight+1);
        }
    } 
} 


void printTree(NODE* head){
// Printing to tree
	int i;
	// n is height of the tree
	int n = height(head);

	// For every level print level of the tree
	for(i = 0; i < n; i++){
		printLevel(head, i);
		printf("\n");
	}
}

void printLevel(NODE* head, int level){
// Printing level of the theree	

	if (head == NULL){
		printf(" ");
		return;
		// If one node was node then print " "
	}
	if (level == 0){
	// Till the reach NULL node print and call the function 
		if(head->letter)
			printf("%d%c ", head->frequency, head->letter);
		else
			printf("%d  ", head->frequency);
	}
	else if (level > 0){
		printLevel(head->left, level-1); 
        printLevel(head->right, level-1);
	}

}
