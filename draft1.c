#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include<limits.h>


char* areas[17]={"Balewadi","Baner","Sppu","Bavdhan","Pashan","Kothrud","Dhayari","Rajaram Bridge","PMC","Dandekar Bridge","Katraj","Bibwewadi","Swargate","Kondhwa","Koregaon Park","Camp","COEP"};

typedef struct node{
    int distance;
    int intensity;
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
            g->matrix[i][j].intensity=-1;
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

void disp(graph* g){
    for(int i=0;i<17;i++){
        for(int j=0;j<17;j++){
            if(i!=j && g->matrix[i][j].distance!=0)
                printf("source-%s dest-%s dist-%d\n",areas[i],areas[j],g->matrix[i][j].distance);
        }
    }
}

#define V 17 
 
int minDistance(int dist[], bool sptSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min){
            min = dist[v], min_index = v;
            printf("%s--",areas[v]);
        }
    return min_index;
}
// A utility function to print the constructed distance
// array
void printSolution(int dist[])
{   
    printf("\n");
    // printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%s \t\t\t\t %d\n", areas[i], dist[i]);

}
 
// Function that implements Dijkstra's single source
// shortest path algorithm for a graph represented using
// adjacency matrix representation
void dijkstra(node** graph, int src)
{
    int dist[V]; // The output array.  dist[i] will hold the
                 // shortest
    // distance from src to i
 
    bool sptSet[V]; // sptSet[i] will be true if vertex i is
                    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized
 
    // Initialize all distances as INFINITE and stpSet[] as
    // false
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to
        // src in the first iteration.
        int u = minDistance(dist, sptSet);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++){
            if (!sptSet[v] && graph[u][v].distance
                && dist[u] != INT_MAX
                && dist[u] + graph[u][v].distance < dist[v]){
                    dist[v] = dist[u] + graph[u][v].distance;
                    
                }
                // printf("%s--\n",areas[u]);
        }
 
            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            
                
    }
 
    // print the constructed distance array
    printSolution(dist);
}

int main(){
    graph g;
    initgraph(&g);
    // disp(&g);
    dijkstra(g.matrix,0);
    return 0;
}






