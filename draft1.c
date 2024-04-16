#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
// enum ar{
//     Balewadi,Baner,Sppu,Bavdhan,Pashan,Kothrud,Dhayari,Rajaram_Bridge,PMC,Dandekar_Bridge,Katraj,Bibwewadi,Swargate,Kondhwa,Koregaon_Park,Camp,COEP
// };
char* areas[17]={"Balewadi","Baner","Sppu","Bavdhan","Pashan","Kothrud","Dhayari","Rajaram Bridge","PMC","Dandekar Bridge","Katraj","Bibwewadi","Swargate","Kondhwa","Koregaon Park","Camp","COEP"};

typedef struct node{
    int distance;
    int intensity;
    int factor;
}node;

typedef struct graph{
    node** matrix;
}graph;

void initgraph(graph* g){
    g->matrix=(node**)malloc(sizeof(node*)*17);
    for(int i=0;i<17;i++){
        g->matrix[i]=(node*)malloc(sizeof(node)*17);
    }
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            g->matrix[i][j].distance=0;
        }
    }
    FILE* fp=fopen("matrix.txt","r");
    for(int i=0;i<29;i++){
        int s,d,dist;
        fscanf(fp,"%d%d%d",&s,&d,&dist);
        g->matrix[s][d].distance=dist;
        g->matrix[d][s].distance=dist;
    }
}

void time1(int hr,int min,graph* g){
    if(min>30){
        hr++;
    }
    int timematrix[17][17][24];
    FILE* fp=fopen("time.txt","r");
    int s,d;
    for(int i=0;i<29;i++){
        fscanf(fp,"%d%d",&s,&d);
        // printf("%d %d\n",s,d);
        for(int j=0;j<24;j++){
            if(j<8 || j>20){
                timematrix[s][d][j]=0;
                timematrix[d][s][j]=0;
            }
            else{
                int x;
                fscanf(fp,"%d",&x);
                // printf("%d ",x);
                timematrix[s][d][j]=x;
                timematrix[d][s][j]=x;
            }
        }
        //printf("\n");
    }
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            if(i!=j){
                g->matrix[i][j].intensity=timematrix[i][j][hr];
                g->matrix[j][i].intensity=timematrix[j][i][hr];
            }
        }
    }
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            g->matrix[i][j].factor=g->matrix[i][j].distance+g->matrix[i][j].intensity;
            g->matrix[j][i].factor=g->matrix[j][i].distance+g->matrix[j][i].intensity;
        }
    }
}



void disp(graph* g){
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            if(i!=j && g->matrix[i][j].distance!=0)
                printf("%d ",g->matrix[i][j].intensity);
        }
        printf("\n");
    }
}

#define V 17 

int minDistance(int dist[], int sptSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Function to print the path from source to j
// Function to print the path from source to j
// Function to print the path from source to j
// Function to print the path from source to j
// Function to print the path from source to j
void printPath(int parent[], int j, int dist, node** graph, int d,int dist1[]) {
    if (parent[j] == -1) {
        // Print the vertex for the initial node
        printf(" %s ", areas[j]);
        return;
    }

    printPath(parent, parent[j], graph[parent[j]][j].factor, graph, d,dist1);

    // Determine the color based on distance between adjacent vertices
    if (parent[j] != d) { // Avoid printing "->" after reaching the destination
        int parentDist = graph[parent[j]][j].factor;
        if (graph[parent[j]][j].intensity < 2){
            printf("\x1b[1;34m--%d-->\x1b[0m",dist1[j]-dist1[parent[j]]);
            //sleep(1);
        }
        else if (graph[parent[j]][j].intensity>=3 && graph[parent[j]][j].intensity <= 4){
            printf("\x1b[1;33m--%d-->\x1b[0m",dist1[j]-dist1[parent[j]]);
            //sleep(2);
        }
        else{
            printf("\x1b[1;31m--%d-->\x1b[0m",dist1[j]-dist1[parent[j]]);
            //sleep(3);
        }
            // Reset color
    }

    // Print the vertex
      //  printf("%d ",dist1[j]-dist1[parent[j]]);
        
        printf(" %s ", areas[j]);
        
}





// Function to print the distance array
void printSolution(int dist[], int parent[], int src, int dest,node** graph,int d,int path[]) {
    printf("Shortest Path from %s to %s: %d\nPath: ", areas[src], areas[dest], path[dest]);
    printPath(parent, dest, dist[dest], graph,d,path); // Pass the graph parameter correctly
    printf("\n");
}

// Function to implement Dijkstra's algorithm for a given graph
void dijkstra(node** graph, int src, int dest) {
    int dist[V]; // The output array dist[i] holds the shortest distance from src to i
    int parent[V]; // Array to store shortest path tree
    int sptSet[V]; // sptSet[i] will be true if vertex i is included in the shortest path tree or the shortest distance from src to i is finalized
    int path[V];
    // Initialize all distances as INFINITE and sptSet[] as false
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        path[i] = INT_MAX;
        sptSet[i] = 0;
        parent[i] = -1;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;
    path[src]=0;
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v].distance != 0 && dist[u] + graph[u][v].factor < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v].factor;
                path[v] = path[u] + graph[u][v].distance;
            }
        }
    }

    // Print the distance array and the shortest path
    printSolution(dist, parent, src,dest,graph,dest,path);
}

int find_area_index(char s[]){
    int x=-1;
    for(int i=0;i<17;i++){
        if(!strcmp(areas[i],s)){
            x=i;
        }
    }
    return x;
}

int main(){
    graph g;
    initgraph(&g);
    printf("\n\n\t\tWELCOME TO TRAFFIC AND ROAD NAVIGATOR\n\n");
    while(1){
        
        printf("1\tSHOW AREAS\n");
        printf("2\tFIND ROUTE\n");
        printf("3\tJOURNEY PLANNER\n");
        printf("4\tEXIT\n");
        int c;
        printf("ENTER CHOICE\n");
        scanf("%d",&c);
        if(c==1){
            // Print AREAS text in bold
            printf("\033[1m"); // Start bold text
            printf("\t\t\t\tA R E A S\n\n");
            printf("\033[0m"); // Reset text formatting
    // Print the table
            for (int i = 0; i < 5; i++) {
                printf("%-12s", areas[i]); // Printing elements from column 1
                printf("\033[1;33m|\033[0m\t%-12s\t\033[1;33m|\033[0m", (i + 5 < 17) ? areas[i + 5] : ""); // Printing elements from column 2 or empty string if NULL
                printf("\033[1;33m|\033[0m\t%-12s\t\033[1;33m|\033[0m", (i + 10 < 17) ? areas[i + 10] : ""); // Printing elements from column 3 or empty string if NULL
                printf("\033[1;33m|\033[0m\t%-12s\t\033[1;33m|\033[0m", (i + 15 < 17) ? areas[i + 15] : ""); // Printing elements from column 4 or empty string if NULL
                printf("\n");
            }
        }
        if(c==2){
            char source[100];
            char dest[100];
            printf("ENTER SOURCE:-");
            scanf("%s",source);
            printf("ENTER DESTINATION:-");
            scanf("%s",dest);
            int x=-1,y=-1;
            x=find_area_index(source);
            y=find_area_index(dest);
            //printf("%d %d\n",x,y);
            if(x==-1 || y==-1){
                printf("Sorry! This app does not support the area that you mentioned\n");
            }
            else{
                int t=-1;
                int min=-1;
                while(t<0 || t>=24 || (min<0 || min>60)){
                    printf("ENTER HOUR OF THE DAY IN 24 HOUR FORMAT(0-23)\n");
                    scanf("%d",&t);
                    printf("ENTER MINUTE OF THE DAY IN 24 HOUR FORMAT(0-23)\n");
                    scanf("%d",&min);
                    if(t<0 || t>=24 || min<0 || min>60){
                        printf("ENTER VALID TIME IN 24 HOUR FORMAT\n");
                    }
                }
                time1(t,min,&g);
                // disp(&g);
                dijkstra(g.matrix,x,y);
            }
        }
        if(c==4){
            break;
        }
    }
    
    
    //disp(&g);
    return 0;
}