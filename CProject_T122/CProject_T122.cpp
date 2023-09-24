#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>


int array[100] = {};
int length = 100;

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


#define WIDTH 31
#define HEIGHT 21
#define WALL 219
#define PATH ' '

//backtracking
int maze[HEIGHT][WIDTH];
int dir[4][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};

void printMaze() {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
        	if(maze[i][j] == 0){
        		printf("%c", WALL);
			}else if(maze[i][j] == 1){
				printf("%c", PATH);
			}else if(maze[i][j] == 2){
				printf("%c", 'V');
			}else if(maze[i][j] == 3){
				printf("%c", 'E');
			}else{
				printf("%c", '#');
			}
        }
        printf("\n");
    }
}

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
                cls();
                printMaze();
                break;
            }
        }
    }
}


void backtracking_vis(){
	for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            maze[i][j] = 0;
        }
    }

    carve(1, 1);

	system("cls");
    printMaze();
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
            cls();
            printMaze();
            union_(u, v);
        }
    }
    
    system("cls");
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

	system("cls");
    printMaze();
}


// sorting algo
void printBars(){
    
    int maxLength = -1;
    for(int i = 0; i<length; i++){
        if(maxLength < array[i]){
            maxLength = array[i];
        }
    }
    
    for(int i = maxLength; i>0; i--){
        for(int j = 0; j<length; j++){
            if(array[j] >= i){
                printf("%c", 219);
            }else{
                printf(" ");
            }
        }
        puts("");
    }
}

void swap_quick(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    if(*a != *b){
        cls();
        printBars();
    }
    usleep(1 * 100);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int array[], int low, int high) {
    int pivot = array[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (array[j] <= pivot) {
            i++;
            swap_quick(&array[i], &array[j]);
        }
    }
    swap_quick(&array[i + 1], &array[high]);
    return (i + 1);
}

void quickSort(int array[], int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}


// merge
void merge(int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (int i = 0; i < n1; i++)
        L[i] = array[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[m + 1 + j];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        }
        else {
            array[k] = R[j];
            j++;
        }
        k++;
        cls();
        printBars();
        usleep(1 * 50);
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
        cls();
        printBars();
        usleep(1 * 50);
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
        cls();
        printBars();
        usleep(1 * 50);
    }
}

void mergeSort(int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(l, m);
        mergeSort(m + 1, r);

        merge(l, m, r);
    }
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap_quick(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap_quick(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}



void random_arr(){
    for(int i = 0; i<length; i++){
        array[i] = (i+4)/4;
    }
    
    for(int i = 0; i<length*30; i++){
        int x1 = rand()%length;
        int x2 = rand()%length;
        swap(&array[x1], &array[x2]);
    }
}

void sorting_vis(){
    random_arr();
    printBars();
    
    int inp = -1;
    do{
        printf("\n\n1. Quick Sort\n2. Merge Sort\n3. Heap Sort\n4. Back\n");
        scanf("%d", &inp);
        getchar();
        
        switch(inp){
            case 1:
                system("cls");
    			printBars();
                quickSort(array, 0, length - 1);
			    random_arr();
                break;
            case 2:
                system("cls");
    			printBars();
				mergeSort(0, length-1);            	
			    random_arr();
                break;
            case 3:
                system("cls");
    			printBars();
				heapSort(array, length);
			    random_arr();
                break;
        }
    }while(inp < 4);
}

#define MAX_HEIGHT 100
#define MAX_WIDTH 100

//typedef struct Point {
//    int x;
//    int y;
//} Point;

//dfs

Point path[MAX_HEIGHT * MAX_WIDTH];
int path_len = 0;

bool isValid(int x, int y){
    return (x>=0 && y>=0 && x < HEIGHT && y < WIDTH && maze[x][y] == 1);
}

bool dfs_algo(int x, int y){
    if(!isValid(x, y)) return false;

    if(x == HEIGHT - 2 && y == WIDTH - 2){
        path[path_len++] = (Point) {x, y};
        return true; 
    }

    maze[x][y] = 2;

    Point dirs[] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
    for (int i = 0; i < 4; i++) {
        int newX = x + dirs[i].x, newY = y + dirs[i].y;
        if(dfs_algo(newX, newY)) {
            path[path_len++] = (Point) {x, y}; 
            return true;
        }
    }
    return false;
}

void print_path() {
	for (int i = 0; i < path_len; ++i) {
        Point p = path[i];
        if(!(p.x == 1 && p.y == 1) && !(p.x == HEIGHT - 2 && p.y == WIDTH - 2)){
            maze[p.x][p.y] = 4;
        }
    }

    printMaze();
	
	
//    for(int i = path_len - 1; i >= 0; i--){
//        printf("(%d, %d)\n", path[i].x, path[i].y);
//    }
}

Point create_point(int x, int y) {
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

#define ROWS 21
#define COLS 31

void print_path(Point start, Point end, Point parent_map[ROWS][COLS]);

int dijkstra(Point start, Point end, Point parent_map[ROWS][COLS]) {
    int distances[ROWS][COLS];
    bool visited[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            distances[i][j] = INT_MAX;
            visited[i][j] = false;
        }
    }

    distances[start.x][start.y] = 0;
    parent_map[start.x][start.y] = create_point(-1, -1);

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!visited[end.x][end.y]) {
        int min_distance = INT_MAX;
        Point current;

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                if (!visited[i][j] && distances[i][j] < min_distance) {
                    min_distance = distances[i][j];
                    current = create_point(i, j);
                }
            }
        }

        visited[current.x][current.y] = true;

        for (int i = 0; i < 4; i++) {
            int x = current.x + dx[i];
            int y = current.y + dy[i];

            if (x >= 0 && x < ROWS && y >= 0 && y < COLS && !visited[x][y] && maze[x][y] == 1) {
                int new_distance = distances[current.x][current.y] + 1;

                if (new_distance < distances[x][y]) {
                    distances[x][y] = new_distance;
                    parent_map[x][y] = current;
                }
            }
        }
    }

    return distances[end.x][end.y];
}

void print_path(Point start, Point end, Point parent_map[ROWS][COLS]) {
    Point current = end;

    while (current.x != -1 && current.y != -1) {
    	cls();
		for(int i = 0;i<ROWS;i++){
			for(int j = 0;j<COLS;j++){
				if((current.x == i && current.y == j) || maze[i][j]==2){
					maze[i][j]=2;
					printf("#");
					continue;
				}
				if(maze[i][j]==1){
					printf(" ");
				}
				else{
					printf("%c", 219);
				}
			}
			printf("\n");
		}
		current = parent_map[current.x][current.y];
    }
}

// djikstra
void djikstra_algo(){
	Point start = create_point(1, 1);
    Point end = create_point(HEIGHT-2, WIDTH-2);
    Point parent_map[ROWS][COLS];
	system("cls");
	for(int i = 0;i<HEIGHT;i++){
		for(int j = 0;j<WIDTH;j++){
			printf("%d", maze[i][j]);
		}
		printf("\n");
	}
    int shortest_path = dijkstra(start, end, parent_map);

    print_path(start, end, parent_map);
}

void pathFinding(){
	int inp = -1;   
    do{
    	printf("1. BFS\n2. DFS\n3. Djikstra's\n4. A*\n");
        scanf("%d", &inp);
        getchar();
        switch(inp){
            case 1:
                break;
            case 2:
            	if(dfs_algo(1, 1)){
            		print_path();
				}
                break;
            case 3:
            		djikstra_algo();
                break;
            case 4:
            	break;
        }
    }while(true);
}



void maze_vis(){
	int inp = -1;   
    do{
    	printf("1. Backtracking\n2. Prim\n3. Kruskal\n");
        scanf("%d", &inp);
        getchar();
        switch(inp){
            case 1:
            	backtracking_vis();
            	pathFinding();
                break;
            case 2:
            	prim_vis();
            	pathFinding();
                break;
            case 3:
            	kruskal_vis();
            	pathFinding();
                break;
        }
    }while(true);
}

HWND currentWindow;
HANDLE outputHandle;

bool isInsideRectangle(POINT point, RECT rect){
	bool isInside = point.x >= rect.left && point.x <= rect.right && point.y >= rect.top && point.y <= rect.bottom;
	return isInside;
}

bool isMouseInsideScreen(){
	POINT cursorPos;
	RECT windowRectangle;
	
	if(!GetCursorPos(&cursorPos) || !GetWindowRect(currentWindow, &windowRectangle)){
		return true;
	}
	return isInsideRectangle(cursorPos, windowRectangle);
}

POINT getMouseClickPosition(){
	POINT cursorPos;
	RECT windowRectangle;
	while(true){
		bool mouseClicked = GetAsyncKeyState(0x01);
		if(mouseClicked){
			GetCursorPos(&cursorPos);
			GetWindowRect(currentWindow, &windowRectangle);
			
			if(!isInsideRectangle(cursorPos, windowRectangle)){
				continue;
			}
			
			cursorPos.x -= windowRectangle.left;
			cursorPos.y -= windowRectangle.top;
			return cursorPos;
		}
	}
}

void loading(){
	for(int i = 1; i<=100; i++){
		cls();
		printf("Loading %d%%\n", i);
		Sleep(50);
	}
	
//	cls();
//	printf("Click to continue\n");
//	printf("(Note : kalo click to continue gabisa, comment loading function di main)\n");
//	
//	getMouseClickPosition();
	cls();
}

int main(){
    hideCursor();
    srand(time(NULL));
    
    loading();
    
    int inp = -1;   
    do{
    	printf("1. Sorting Visualization\n2. Maze Visualization\n3. Sandbox\n");
        scanf("%d", &inp);
        getchar();
        switch(inp){
            case 1:
                sorting_vis();
                break;
            case 2:
            	maze_vis();
                break;
            case 3:
            	maze_vis();
                break;
        }
    }while(true);
    
    return 0;
}
