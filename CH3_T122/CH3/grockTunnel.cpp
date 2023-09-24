#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define WIDTH 31
#define HEIGHT 21
#define WALL 219
#define PATH ' '

int maze[HEIGHT][WIDTH];
int dir[4][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
int radius = 5;
int px = 1, py = 1;

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

void genFlashlight(){
	for(int i = 0; i<HEIGHT; i++){
		for(int j = 0; j<WIDTH; j++){
			
			if(i == 1 && j == 1) continue;
			
			if(maze[i][j] == 1){
				int randNum = rand()%17;
				if(randNum == 0){
					maze[i][j] = 2;
				}
			}
		}
	}
}

void printMaze() {
    for (int i = px-radius; i < px+radius; ++i) {
        for (int j = py-radius; j < py+radius; ++j) {
        	if(j >= WIDTH || i >= HEIGHT || j < 0 || i < 0){
        		printf("#");
			}else if(px == i && py == j){
        		printf("P");
			}else{	
				if(maze[i][j] == 0){
					printf("%c", WALL);
				}else if(maze[i][j] == 1){
					printf("%c", PATH);					
				}else if(maze[i][j] == 2){
					printf("%c", '*');
				}
			}
        }
        printf("\n");
    }
}

struct Data{
	char name[100];
	char pw[100];
	int score;
	int height;
	struct Data *left, *right;
}*root = NULL;

struct User{
	char name[100];
	char pw[100];
	int score;
};
User users[100];

struct Data *createNewNode(char name[], char pw[], int score){
	struct Data *newNode = (Data *)malloc(sizeof(Data));
	
	strcpy(newNode->name, name);
	strcpy(newNode->pw, pw);
	newNode->score = score;
	newNode->left = newNode->right = NULL;
	newNode->height = 1;
	
	return newNode;
}

int getHeight(struct Data *node){
	if(node == NULL) return 0;
	
	return node->height;
}

int getMax(int a, int b){
	if(a > b) return a;
	return b;
}

int getBalanceFactor(struct Data *node){
	return getHeight(node->left) - getHeight(node->right);
}

struct Data *leftRotate(struct Data *nodeX){
	struct Data *nodeY = nodeX->right;
	struct Data *nodeB = nodeY->left;
	
	nodeY->left = nodeX;
	nodeX->right = nodeB;
	
	nodeX->height = 1 + getMax(getHeight(nodeX->left), getHeight(nodeX->right));
	nodeY->height = 1 + getMax(getHeight(nodeY->left), getHeight(nodeY->right));
	
	return nodeY;
}

struct Data *rightRotate(struct Data *nodeX){
	struct Data *nodeY = nodeX->left;
	struct Data *nodeB = nodeY->right;
	
	nodeY->right = nodeX;
	nodeX->left = nodeB;
	
	nodeX->height = 1 + getMax(getHeight(nodeX->left), getHeight(nodeX->right));
	nodeY->height = 1 + getMax(getHeight(nodeY->left), getHeight(nodeY->right));
	
	return nodeY;
}

struct Data *insertNode(Data *node, char name[], char pw[], int score){
	if(node == NULL){
		return createNewNode(name, pw, score);
	}else if(score < node->score){
		node->left = insertNode(node->left, name, pw, score);
	}else if(score > node->score){
		node->right = insertNode(node->right, name, pw, score);
	}else{
		printf("value tidak boleh sama\n");
	}

	node->height = getMax(getHeight(node->left), getHeight(node->right)) + 1;
	int balanceFactor = getBalanceFactor(node);
	
	if(balanceFactor > 1 && getBalanceFactor(node->left) >= 0){
		//right rotate
		return rightRotate(node);
	}
	if(balanceFactor < -1 && getBalanceFactor(node->right) <= 0){
		// left rotate
		return leftRotate(node);
	}
	if(balanceFactor > 1 && getBalanceFactor(node->left) < 0){
		//lr + rr
		node->left = leftRotate(node->left);
		
		return rightRotate(node);
	}
	if(balanceFactor < -1 && getBalanceFactor(node->right) > 0){
		//rr + lr
		node->right = rightRotate(node->right);
		
		return leftRotate(node);
	}
	
	
	
	return node;
}


void preOrder(struct Data *node){
	if(node == NULL) return;
	printf("%-20s %-5d\n", node->name, node->score);
	preOrder(node->left);
	preOrder(node->right);
}

void inOrder(struct Data *node){
	if(node == NULL) return;
	inOrder(node->left);
	printf("%-20s %-5d\n", node->name, node->score);
	inOrder(node->right);
}

void postOrder(struct Data *node){
	if(node == NULL) return;
	postOrder(node->left);
	postOrder(node->right);
	printf("%-20s %-5d\n", node->name, node->score);
}

void highScore(){
	system("cls");
	printf("-----------------------------------------\n");
	printf("%-20s %-5s\n", "Name", "Highscore");
	printf("-----------------------------------------\n");

	preOrder(root);
	
	puts("");
	printf("Enter to continue...");
	getchar();
	system("cls");
}

// kruskal
typedef struct {
    int x, y;
} Node;

typedef struct {
    Node u, v;
} Edge;


int parent[HEIGHT][WIDTH];
Edge edges[HEIGHT * WIDTH];

int find(int x, int y) {
    if (parent[x][y] == x * WIDTH + y) return parent[x][y];
    return parent[x][y] = find(parent[x][y] / WIDTH, parent[x][y] % WIDTH);
}

void union_(Node u, Node v) {
    int uroot = find(u.x, u.y), vroot = find(v.x, v.y);
    if (uroot != vroot) parent[uroot / WIDTH][uroot % WIDTH] = vroot;
}


void kruskal_vis() {
    srand(time(NULL));

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i % 2 == 0 || j % 2 == 0) maze[i][j] = 0;
            else maze[i][j] = 1;

            parent[i][j] = i * WIDTH + j;
        }
    }

    int e = 0;
    for (int i = 1; i < HEIGHT; i += 2) {
        for (int j = 1; j < WIDTH; j += 2) {
            if (j + 2 < WIDTH) {
                edges[e++] = (Edge) {{i, j}, {i, j + 2}};
            }
            if (i + 2 < HEIGHT) {
                edges[e++] = (Edge) {{i, j}, {i + 2, j}};
            }
        }
    }

    for (int i = e - 1; i >= 0; i--) {
        int j = rand() % (i + 1);
        Edge t = edges[i]; edges[i] = edges[j]; edges[j] = t;
    }

    for (int i = 0; i < e; i++) {
        Node u = edges[i].u, v = edges[i].v;
        if (find(u.x, u.y) != find(v.x, v.y)) {
            maze[(u.x + v.x) / 2][(u.y + v.y) / 2] = 1;
            union_(u, v);
        }
    }
    
    printMaze();
}


// prim
typedef struct Point {
    int x;
    int y;
} Point;

Point point(int x, int y) {
    Point p = {x, y};
    return p;
}

Point frontier[WIDTH * HEIGHT];
int frontier_size = 0;

void carve(int x, int y) {
    int d, i, j, x1, y1, count;

    while (1) {
        count = 0;
        for (d = 0; d < 4; ++d) {
            i = dir[d][0] * 2;
            j = dir[d][1] * 2;
            x1 = x + i;
            y1 = y + j;
            if (x1 > 0 && x1 < WIDTH && y1 > 0 && y1 < HEIGHT && maze[y1][x1] == 0) {
                ++count;
            }
        }
        if (!count) {
            return;
        }

        while (1) {
            d = rand() % 4;
            i = dir[d][0] * 2;
            j = dir[d][1] * 2;
            x1 = x + i;
            y1 = y + j;
            if (x1 > 0 && x1 < WIDTH && y1 > 0 && y1 < HEIGHT && maze[y1][x1] == 0) {
                maze[y][x] = 1;
                maze[y + dir[d][1]][x + dir[d][0]] = 1;
                maze[y1][x1] = 1;
                carve(x1, y1);
                break;
            }
        }
    }
}

void add_frontier(int x, int y) {
    for (int d = 0; d < 4; ++d) {
        int x1 = x + dir[d][0] * 2;
        int y1 = y + dir[d][1] * 2;
        if (x1 > 0 && x1 < WIDTH && y1 > 0 && y1 < HEIGHT && maze[y1][x1] == 0) {
            maze[y1][x1] = -1;
            frontier[frontier_size++] = point(x1, y1);
        }
    }
}

void prim_vis(){
	for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = 0;
        }
    }

    carve(1, 1);

    maze[1][1] = 2;
    maze[HEIGHT - 2][WIDTH - 2] = 3;

    printMaze();
}

void movePlayer(){
	genFlashlight();
	system("cls");
	while(true){
		cls();	
		int flag = -999;
		if(kbhit()){
			switch(getch()){
				case 'w':
					if(maze[px-1][py] != 0){
						px--;
						flag = 2;
						
						if(maze[px-1][py] == 2){
							maze[px-1][py] = 1;
							radius += 1;
						}
					}
					break;
				case 'a':
					if(maze[px][py-1] != 0){
						py--;
						flag = 1;
						
						if(maze[px][py-1] == 2){
							maze[px][py-1] = 1;
							radius += 1;
						}
					}
					break;
				case 's':
					if(maze[px+1][py] != 0){
						px++;
						flag = -1;
						
						if(maze[px+1][py] == 2){
							maze[px+1][py] = 1;
							radius += 1;
						}
					}
					break;
				case 'd':
					if(maze[px][py+1] != 0){
						py++;
						flag = -2;
						
						if(maze[px][py+1] == 2){
							maze[px][py+1] = 1;
							radius += 1;
						}
					}
					break;
			}
		}
		
		printMaze();
	}
}

void playGame(char name[]){
	printf("Hello %s !\n", name);
	
	int inp = -1;
	do{
		printf("Choose your preferred maze generator,\n--------------\n1. Prim\n2. Kruskal\n3. Exit\n>> ");
		scanf("%d", &inp);
		getchar();
		
		switch(inp){
			case 1:
				prim_vis();
				movePlayer();
				break;
			case 2:
				kruskal_vis();
				movePlayer();
				break;
		}
	}while(inp != 3);
}

void scanningUser(struct Data *node){
	if(node == NULL) return;
	inOrder(node->left);
	printf("%s %s %d ->", node->name, node->pw, node->score);
	inOrder(node->right);
}

int manyUser = 0;
void loginMenu(){
	
	FILE *fp;
	fp=fopen("user.txt","r");
	while(!feof(fp)){
      	fscanf(fp,"%[^#]#%[^#]#%d\n", &users[manyUser].name, &users[manyUser].pw, &users[manyUser].score);
      	manyUser++;
    }
    fclose(fp);
    
	char name[100];
	char pw[100];
    
	printf("Login\n--------\nInput name ['q' to go back]: ");
    scanf("%s", &name);
    getchar();
    printf("Input password: ");
    scanf("%s", &pw);
    getchar();
    
    int val = 0;
    for(int i = 0; i<manyUser; i++){
    	if(strcmp(users[i].name, name) == 0 && strcmp(users[i].pw, pw) == 0){
    		playGame(name);
			printf("ketemu\n");
			val = 1;
		}
	}
	
	if(val == 0){
		printf("User password invalid\n");
		getchar();
	}
}

void registerMenu(){
	char name[100];
	char pw[100];
	
	printf("Register\n--------\nInput name [must be unique]: ");
	scanf("%s", &name);
    getchar();
    
    int len;
    do{
	    printf("Input password [minimal 5 char]: ");
    	scanf("%s", &pw);
    	getchar();
	    len = strlen(pw);
	}while(len < 5);
	
	int score = 0;
	
	FILE *fp;
	fp=fopen("user.txt","a");
   	fprintf(fp,"%s#%s#%d\n", &name, &pw, &score);
    fclose(fp);
}



int main(){
	hideCursor();
	srand(time(NULL));
	
	int putAvl = 0;
	if(putAvl == 0){
		char name[100];
		char pw[100];
		int score;
		
		FILE *fp;
		fp=fopen("user.txt","r");
		while(!feof(fp)){
	      	fscanf(fp,"%[^#]#%[^#]#%d\n", &name, &pw, &score);
			root = insertNode(root, name, pw, score);
	    }
	    fclose(fp);
		
		putAvl = 1;
	}
	
	
	int inp = -1;
	do{
		printf("Welcome to GrocktunneL,\n----------------------\n1. Login\n2. Register\n3. Highscore\n4. Quit\n>> ");
		scanf("%d", &inp);
		getchar();
		
		switch(inp){
			case 1:
				loginMenu();
				break;
			case 2:
				registerMenu();
				break;
			case 3:
				highScore();
				break;
		}
	}while(inp != 0);
	
	return 0;
}
