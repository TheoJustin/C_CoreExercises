#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

struct Stasiun{
	char nama[100];
	Stasiun *utara;
	Stasiun *timur;
	Stasiun *selatan;
	Stasiun *barat;
	bool flag;
}*root = NULL;

struct chest{
	char name[100];
	char rarity[100];
	int score;
	
	chest *next;
	chest *prev;
}*head[5], *tail[5];

char maps[100][100] = {};

int hashing(const char *name){
	int key = -1;
	if(strcmp(name, "Common")==0){
		key = 0;
	}else if(strcmp(name, "Uncommon")==0){
		key = 1;
	}else if(strcmp(name, "Rare")==0){
		key = 2;
	}else if(strcmp(name, "Epic")==0){
		key = 3;
	}else{
		key = 4;
	}
	
	return key;
}

struct chest *newNode(char name[], char rarity[], int score){
	struct chest *newNode = (chest *)malloc(sizeof(chest));
	newNode->score = score;
	strcpy(newNode->name, name);
	strcpy(newNode->rarity, rarity);
	
	return newNode;
}

void insertTable(char name[], char rarity[], int score){
	int key = hashing(rarity);
	struct chest *curr = newNode(name, rarity, score);
	
	if(head[key] == NULL){
		head[key] = tail[key] = curr;
	}else{
		tail[key]->next = curr;
		curr = tail[key];
	}
}

void cls(){
	COORD cursorPosition;
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void hideCursor(){
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

Stasiun *createNewStasiun(char nama[]){
	Stasiun *newStasiun = (Stasiun *)malloc(sizeof(Stasiun));
	strcpy(newStasiun->nama, nama);
	newStasiun->utara = newStasiun->barat = newStasiun->timur = newStasiun->selatan = NULL;
	newStasiun->flag = false;
	
	return newStasiun;
}

void pushUtara(Stasiun *curr, char nama[]){
	if(curr->utara != NULL) return;
	
	Stasiun* newStasiun = createNewStasiun(nama);
	curr->utara = newStasiun;
	newStasiun->selatan = curr;
}

void pushTimur(Stasiun *curr, char nama[]){
	if(curr->timur != NULL) return;
	
	Stasiun* newStasiun = createNewStasiun(nama);
	curr->timur = newStasiun;
	newStasiun->barat = curr;
}

void pushBarat(Stasiun *curr, char nama[]){
	if(curr->barat != NULL) return;
	
	Stasiun* newStasiun = createNewStasiun(nama);
	curr->barat = newStasiun;
	newStasiun->timur = curr;
}

void pushSelatan(Stasiun *curr, char nama[]){
	if(curr->selatan != NULL) return;
	
	Stasiun* newStasiun = createNewStasiun(nama);
	curr->selatan = newStasiun;
	newStasiun->utara = curr;
}

void joinVertical(Stasiun *utara, Stasiun *selatan){
	utara->selatan = selatan;
	selatan->utara = utara;
}

void joinHorizontal(Stasiun *barat, Stasiun *timur){
	timur->barat = barat;
	barat->timur = timur;
}

void howToPlay(){
	printf("===========================\nSymbol Details\n===========================\nC -> is resemble a Chest\nS -> is resemble Spawn Point\n^ -> is resemble a Spike\nO -> is resemble a rock\n===========================\nDifficulty Details");
 	printf("===========================\n1. Beginner\n- Steps: Infinite\n- Item Chances: 60% Common, 35% Uncommon, 5% Rare\n2. Normal");
    printf("- Start Steps: 750\n- Item Chances: 40% Common, 25% Uncommon, 20% Rare, 10% Epic, 5% Legend\n- Extra Step After Open Chest: 80\n- Score Multiplier: 2x");
 	printf("3. Hard\n- Start Steps: 500\n- Item Chances: 20% Common, 25% Uncommon, 30% Rare, 15% Epic, 10% Legend\n- Extra Step After Open Chest: 75\n- Score Multiplier: 3x\n\n ===========================\nPress Enter to Continue...");
	getchar();
}


void tutorial_menu(){
	
}

void changeMap(Stasiun *curr){
	int i = 0;
	FILE *fp;
	if(strcmp(curr->nama, "Home") == 0){
	    fp=fopen("Maps/Home.txt","r");	
	}else if(strcmp(curr->nama, "A") == 0){
	    fp=fopen("Maps/A.txt","r");	
	}else if(strcmp(curr->nama, "F") == 0){
	    fp=fopen("Maps/F.txt","r");	
	}else if(strcmp(curr->nama, "H") == 0){
	    fp=fopen("Maps/H.txt","r");	
	}else if(strcmp(curr->nama, "G") == 0){
	    fp=fopen("Maps/G.txt","r");	
	}else if(strcmp(curr->nama, "C") == 0){
	    fp=fopen("Maps/C.txt","r");	
	}else if(strcmp(curr->nama, "B") == 0){
	    fp=fopen("Maps/B.txt","r");	
	}else if(strcmp(curr->nama, "E") == 0){
	    fp=fopen("Maps/E.txt","r");	
	}else if(strcmp(curr->nama, "D") == 0){
	    fp=fopen("Maps/D.txt","r");	
	}
	
	
    while(!feof(fp)){
      fscanf(fp,"%[^\n]\n", &maps[i]);
      i++;
    }
    fclose(fp);
}

void playGame(){
	Stasiun *curr = root;
	int px = 11;
	int py = 21;
	while(true){
		cls();	
		int flag = -999;
		if(kbhit()){
			switch(getch()){
				case 'w':
					if(maps[px-1][py] != '#'){
						px--;
						flag = 2;
					}
					break;
				case 'a':
					if(maps[px][py-1] != '#'){
						py--;
						flag = 1;
					}
					break;
				case 's':
					if(maps[px+1][py] != '#'){
						px++;
						flag = -1;
					}
					break;
				case 'd':
					if(maps[px][py+1] != '#'){
						py++;
						flag = -2;
					}
					break;
			}
		}
		
		if(maps[px][py] == '^'){
			curr = root;
			changeMap(curr);
			px = 11;
			py = 21;
		}
		
		if(py == 0 && maps[px][py] == '|'){
			changeMap(curr->barat);
			curr = curr->barat;
			py = 44;
			for(int i = 0; i<25; i++){
				if(maps[i][py] == '|'){
					px = i;
					break;
				}
			}
			py = 43;
		}
		
		if(px == 0 && maps[px][py] == '-'){
			changeMap(curr->utara);
			curr = curr->utara;
			px = 24;
			for(int i = 0; i<45; i++){
				if(maps[px][i] == '|'){
					py = i;
					break;
				}
			}
			px = 23;
		}
		
		if(py == 44 && maps[px][py] == '|'){
			changeMap(curr->timur);
			curr = curr->timur;
			py = 0;
			for(int i = 0; i<25; i++){
				if(maps[i][py] == '|'){
					px = i;
					break;
				}
			}
			py = 1;
		}
		
		if(px == 24 && maps[px][py] == '-'){
			changeMap(curr->selatan);
			curr = curr->selatan;
			px = 0;
			for(int i = 0; i<45; i++){
				if(maps[px][i] == '|'){
					py = i;
					break;
				}
			}
			px = 1;
		}
		
		if(maps[px][py] == 'C'){
			int x = rand()%100;
			system("cls");
			if(x < 60){
				struct chest *current = head[0];
				printf("You Get %s\n", current->name);
			}else if(x < 95){
				struct chest *current = head[1];
				printf("You Get %s\n", current->name);
			}else{
				struct chest *current = head[2];
				printf("You Get %s\n", current->name);
			}
			
			maps[px][py] = ' ';
			getchar();
		}
		
		for(int i = 0; i<25; i++){
			for(int j = 0; j<45; j++){
				if(px == i && py == j){
	 	     		printf("P");
	 	     		continue;
				}
	      		printf("%c", maps[i][j]);
			}
			printf("\n");
		}
	}
	
	
}

void new_game(){
	printf("Enter Your name [1..5]");
	char name[50];
	do{
		scanf("%s", name);
		getchar();
	}while(strlen(name)<5);
	
	
	printf("Welcome %s!\nThis is the story of your Adventure following the\nfootstep of the legendary adventurer JeDi", name);
 	printf("to uncover all the treasure in this dungeon\nPress Enter to Continue...\n");
 	getchar();
 	
 	system("cls");
 	
	// drawing overall map 	
 	root = createNewStasiun("Home");
	Stasiun* curr = root;
	
	pushUtara(curr, "H");
	pushBarat(curr, "A");
	pushTimur(curr, "F");
	pushSelatan(curr, "G");
	curr = curr->timur;
	pushTimur(curr, "E");
	curr = curr->timur;
	pushSelatan(curr, "D");
	curr = curr->selatan;
	pushBarat(curr, "C");
	curr = curr->barat;
	joinVertical(curr, root->selatan);
	pushBarat(curr, "B");
	curr = curr->barat;
	joinVertical(curr, root->barat);
	curr = root;
	
	int i = 0;
	FILE *fp;
    fp=fopen("Maps/Home.txt","r");
    while(!feof(fp)){
      fscanf(fp,"%[^\n]\n", &maps[i]);
      i++;
    }
    fclose(fp);
    
    playGame();
	
	puts("");
	
}

void fillinghash(){
    char name[100];
    char rarity[100];
    int score;
    
	int i = 0;
	FILE *fp;
    fp=fopen("ListItem.csv","r");
    while(!feof(fp)){
    	
      	fscanf(fp,"%[^,],%[^,],%d\n", &name, &rarity, &score);
      	insertTable(name, rarity, score);
    }
    fclose(fp);
}

int main(){
	hideCursor();
	
	fillinghash();
	
	int i = 0;
	FILE *fp;
    fp=fopen("Maps/Home.txt","r");
    while(!feof(fp)){
      fscanf(fp,"%[^\n]\n", &maps[i]);
      i++;
    }
    fclose(fp);
	
	int inp;
	do{
		printf("JeDi's  Adventure!\n==================\n1. Start New Game\n2. How To Play\n3. High Score\n4. Exit\n>>");
		scanf("%d", &inp);
		getchar();
		switch(inp){
			case 1:
				new_game();
				break;
			case 2:
				tutorial_menu();
				break;
			case 3:
				break;
		}
	}while(inp != 4);
	return 0;
}
