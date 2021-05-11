#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Liste
{
	struct Liste* prev;
	struct Liste* next;
	int counter;
	char adr[50];
	
};
typedef struct Liste LIST;

void display(LIST*);
void DeleteLast(LIST*);
LIST* insert(LIST*, char*);
LIST* whereIs(LIST*, char*);
LIST* move(LIST*, LIST*);
void inputFromFile();
void inputFromTerminal();
void inputType();

static edge = 0;

int main()
{
	inputType();


	return 0;
}



void inputType(){
	int val;
	
	printf("Dosyadan input almak icin 1\nTerminalden input girmek icin 2 giriniz\n");
	scanf("%d",&val);
	while(val!= 1 && val!= 2){
		printf("Yanlis giris yaptiniz. Tekrar giriniz\n");
		scanf("%d",&val);
	}
	if(val == 1)
		inputFromFile();
	else if(val == 2)
		inputFromTerminal();

}
void inputFromTerminal(){
	char str[50];
	int T,L;
	char ctrl;
	LIST* tmp,*head;
	head = NULL;

	printf("Esik degerini giriniz : ");
	scanf("%d",&T);
	printf("Kapasiteyi giriniz : ");
	scanf("%d",&L);
	printf("\n\n\nCikmak icin -1\nTemizlemek icin clear giriniz\n\n\n");

	printf("Sayfa giriniz : ");
	scanf("%s",str);

	while(strcmp(str,"-1")!=0){
		if(strcmp(str,"-1")==0){
		printf("Cikis yapildi\n");
		return;
		}

		else{
			if(strcasecmp(str,"clear")==0){
				system("cls");
				printf("\n\n\nCikmak icin -1\nTemizlemek icin clear giriniz\n\n\n");
				while(head!=NULL){
					tmp = head;
					head = head->next;
					free(tmp);
				}
				edge = 0;
				printf("Yeni sayfayi giriniz : ");
				scanf("%s",str);
			}
			tmp = whereIs(head,str);
			if(edge < L){

				if(tmp == NULL){
					head = insert(head,str);
					edge++;
					head->counter = 1;
				}
				else{
					tmp->counter++;
					if(tmp->counter > T && head != tmp)
						head = move(head,tmp);
				}
			}

			else{

				if(tmp == NULL){
					DeleteLast(head);
					head = insert(head,str);
					head->counter = 1;
				}
				else{
					tmp->counter++;
					if (tmp->counter > T && head != tmp)
						head = move(head,tmp); 
					
				}
			}
			display(head);
			printf("Yeni sayfayi giriniz : ");
			scanf("%s",str);

		}

	}
}
void inputFromFile(){
	char str[50];
	char file[50];
	FILE* fptr;
	int T,L;
	LIST* tmp,*head;
	head = NULL;


	printf("Sayfalarin oldugu dosyanin adini uzantisiyla beraber giriniz : ");	
	scanf("%s",file);

	if(!(fptr = fopen(file,"r"))){
	printf("Dosya bulunamadi!!!");
	exit(0);
}
	fscanf(fptr,"%d",&T);
	fscanf(fptr,"%d",&L);

while(!feof(fptr)){
	fscanf(fptr,"%s",str);
	tmp = whereIs(head,str);
	if(edge<L){
		
		if(tmp==NULL){
			head = insert(head,str);
			edge++;
			head->counter = 1;
		}
		else{
			tmp->counter++;
			if(tmp->counter>T && head != tmp){
				head = move(head,tmp);
			}

		}
	}
	else{
		
		if(tmp==NULL){
			DeleteLast(head);
			head = insert(head,str);
			head->counter = 1;
		}
		else{
			tmp->counter++;
			if(tmp->counter>T && head != tmp){
				head = move(head,tmp);
			}
		}
	}
	display(head);

		
	}
	fclose(fptr);
}

LIST* whereIs(LIST* s, char* str){
	LIST* iter = s;
	while(iter!=NULL){
		if(strcmp(iter->adr,str)==0){
			return iter;
		}
		iter=iter->next;	
	}
	return NULL;
}

void display(LIST* s){
	LIST* iter;
	iter = s;
	while(iter!=NULL){
		printf("%s,%d  ",iter->adr,iter->counter);
		iter = iter->next;
	}
	printf("\n");
}

LIST* insert(LIST* s, char* adr){
	if(s == NULL){
		s = (LIST*) malloc(sizeof(LIST));
		strcpy(s->adr,adr);
		s->next = NULL;
		s->prev = NULL;
		return s;	
	}
	else{
	LIST* tmp;
	tmp = (LIST*) malloc(sizeof(LIST));

	tmp->prev = NULL;
	strcpy(tmp->adr,adr);
	tmp->next = s;
	s->prev = tmp;
	return tmp;
	}
}

void DeleteLast(LIST* s){
	 
	LIST* iter;
	iter = s;
	if (iter->next == NULL){
		iter = NULL;
	}
	else{
		while(iter->next->next!=NULL){
			iter = iter->next;
		}
		free(iter->next);
		iter->next = NULL;
	}
}

LIST* move(LIST* s,LIST* new){

	LIST* tmp;
	tmp = new->prev;
	tmp->next = new->next;
	if (new->next!=NULL)
		new->next->prev = tmp;
	s->prev = new;
	new->prev = NULL;
	new->next = s;
	

	return new;

}
