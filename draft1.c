#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

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

void time(int hr,int min,graph* g){
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
                timematrix[s][d][j]=1;
                timematrix[d][s][j]=1;
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
            g->matrix[i][j].factor=g->matrix[i][j].distance*g->matrix[i][j].intensity;
            g->matrix[j][i].factor=g->matrix[j][i].distance*g->matrix[j][i].intensity;
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
void printPath(int parent[], int j, int dist, node** graph, int d) {
    if (parent[j] == -1) {
        // Print the vertex for the initial node
        printf("%s", areas[j]);
        return;
    }

    printPath(parent, parent[j], graph[parent[j]][j].factor, graph, d);

    // Determine the color based on distance between adjacent vertices
    if (parent[j] != d) { // Avoid printing "->" after reaching the destination
        int parentDist = graph[parent[j]][j].factor;
        if (parentDist < 4)
            printf("\033[0;34m");  // Blue color
        else if (parentDist >= 4 && parentDist <= 7)
            printf("\033[0;33m");  // Yellow color
        else
            printf("\033[0;31m");  // Red color
        printf(" --> \033[0m");  // Reset color
    }

    // Print the vertex
    if (dist < 4)
        printf("%s", areas[j]);
    else if (dist >= 4 && dist <= 7)
        printf("%s", areas[j]);
    else
        printf("%s", areas[j]);
}





// Function to print the distance array
void printSolution(int dist[], int parent[], int src, int dest,node** graph,int d) {
    printf("Shortest Path from %s to %s: %d\nPath: ", areas[src], areas[dest], dist[dest]);
    printPath(parent, dest, dist[dest], graph,d); // Pass the graph parameter correctly
    printf("\n");
}

// Function to implement Dijkstra's algorithm for a given graph
void dijkstra(node** graph, int src, int dest) {
    int d=dest;
    int dist[V]; // The output array dist[i] holds the shortest distance from src to i
    int parent[V]; // Array to store shortest path tree
    int sptSet[V]; // sptSet[i] will be true if vertex i is included in the shortest path tree or the shortest distance from src to i is finalized

    // Initialize all distances as INFINITE and sptSet[] as false
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        sptSet[i] = 0;
        parent[i] = -1;
    }

    // Distance of source vertex from itself is always 0
    dist[src] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex as processed
        sptSet[u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex
        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && graph[u][v].factor != 0 && dist[u] + graph[u][v].factor < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v].factor;
            }
        }
    }

    // Print the distance array and the shortest path
    printSolution(dist, parent, src, dest,graph,d);
}

int main(){
    graph g;
    initgraph(&g);
    time(16,0,&g);
    // disp(&g);
    dijkstra(g.matrix,6,0);
    
    //disp(&g);
    return 0;
}
