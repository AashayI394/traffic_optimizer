#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

// Define the maximum number of areas
#define NUM_AREAS 17

char* areas[NUM_AREAS] = {"Balewadi", "Baner", "Sppu", "Bavdhan", "Pashan", "Kothrud", "Dhayari", "Rajaram Bridge", "PMC", "Dandekar Bridge", "Katraj", "Bibwewadi", "Swargate", "Kondhwa", "Koregaon Park", "Camp", "COEP"};

// Define the node and graph structures
typedef struct node {
    int distance;
    int intensity;
    int factor;
} node;

typedef struct graph {
    node** matrix;
} graph;

// Function prototypes
void initgraph(graph* g);
void time1(int hr, int min, graph* g);
void disp(graph* g);
int minDistance(int dist[], int sptSet[]);
void printPath(int parent[], int j, int dist, node** graph, int d, int dist1[]);
void printSolution(int dist[], int parent[], int src, int dest, node** graph, int d, int path[]);
void dijkstra(node** graph, int src, int dest);
void journeyPlanning(graph* g);

// Initialize the graph
void initgraph(graph* g) {
    g->matrix = (node**)malloc(sizeof(node*) * NUM_AREAS);
    for (int i = 0; i < NUM_AREAS; i++) {
        g->matrix[i] = (node*)malloc(sizeof(node) * NUM_AREAS);
    }
    for (int i = 0; i < NUM_AREAS; i++) {
        for (int j = 0; j < NUM_AREAS; j++) {
            g->matrix[i][j].distance = 0;
        }
    }
    FILE* fp = fopen("matrix.txt", "r");
    for (int i = 0; i < 29; i++) {
        int s, d, dist;
        fscanf(fp, "%d%d%d", &s, &d, &dist);
        g->matrix[s][d].distance = dist;
        g->matrix[d][s].distance = dist;
    }
}

// Update time matrix based on current hour and minute
void time1(int hr, int min, graph* g) {
    if (min > 30) {
        hr++;
    }
    int timematrix[NUM_AREAS][NUM_AREAS][24];
    FILE* fp = fopen("time.txt", "r");
    int s, d;
    for (int i = 0; i < 29; i++) {
        fscanf(fp, "%d%d", &s, &d);
        for (int j = 0; j < 24; j++) {
            if (j < 8 || j > 20) {
                timematrix[s][d][j] = 0;
                timematrix[d][s][j] = 0;
            } else {
                int x;
                fscanf(fp, "%d", &x);
                timematrix[s][d][j] = x;
                timematrix[d][s][j] = x;
            }
        }
    }
    for (int i = 0; i < NUM_AREAS; i++) {
        for (int j = 0; j < NUM_AREAS; j++) {
            if (i != j) {
                g->matrix[i][j].intensity = timematrix[i][j][hr];
                g->matrix[j][i].intensity = timematrix[j][i][hr];
            }
        }
    }
    for (int i = 0; i < NUM_AREAS; i++) {
        for (int j = 0; j < NUM_AREAS; j++) {
            g->matrix[i][j].factor = g->matrix[i][j].distance + g->matrix[i][j].intensity;
            g->matrix[j][i].factor = g->matrix[j][i].distance + g->matrix[j][i].intensity;
        }
    }
}

// Display the graph
void disp(graph* g) {
    for (int i = 0; i < NUM_AREAS; i++) {
        for (int j = 0; j < NUM_AREAS; j++) {
            if (i != j && g->matrix[i][j].distance != 0)
                printf("%d ", g->matrix[i][j].intensity);
        }
        printf("\n");
    }
}

// Find the vertex with minimum distance value
int minDistance(int dist[], int sptSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < NUM_AREAS; v++) {
        if (sptSet[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Print the path from source to destination
void printPath(int parent[], int j, int dist, node** graph, int d, int dist1[]) {
    if (parent[j] == -1) {
        printf(" %s ", areas[j]);
        return;
    }

    printPath(parent, parent[j], graph[parent[j]][j].factor, graph, d, dist1);

    if (parent[j] != d) {
        int parentDist = graph[parent[j]][j].factor;
        if (graph[parent[j]][j].intensity < 2) {
            printf("\x1b[1;34m--%d-->\x1b[0m", dist1[j] - dist1[parent[j]]);
            sleep(1);
        } else if (graph[parent[j]][j].intensity >= 3 && graph[parent[j]][j].intensity <= 4) {
            printf("\x1b[1;33m--%d-->\x1b[0m", dist1[j] - dist1[parent[j]]);
            sleep(2);
        } else {
            printf("\x1b[1;31m--%d-->\x1b[0m", dist1[j] - dist1[parent[j]]);
            sleep(3);
        }
    }

    printf(" %s ", areas[j]);
}

// Print the shortest path
// void printSolution(int dist[], int parent[], int src, int dest, node** graph, int d, int path[]) {
//     printf("Shortest Path from %s to %s: %d\nPath: ", areas[src], areas[dest], path[dest]);
//     printPath(parent, dest, dist[dest], graph, d, path);
//     printf("\n");
// }

// Implement Dijkstra's algorithm
// void dijkstra(node** graph, int src, int dest) {
//     int dist[NUM_AREAS];
//     int parent[NUM_AREAS];
//     int sptSet[NUM_AREAS];
//     int path[NUM_AREAS];
//     for (int i = 0; i < NUM_AREAS; i++) {
//         dist[i] = INT_MAX;
//         path[i] = INT_MAX;
//         sptSet[i] = 0;
//         parent[i] = -1;
//     }

//     dist[src] = 0;
//     path[src] = 0;
//     for (int count = 0; count < NUM_AREAS - 1; count++) {
//         int u = minDistance(dist, sptSet);
//         sptSet[u] = 1;
//         for (int v = 0; v < NUM_AREAS; v++) {
//             if (!sptSet[v] && graph[u][v].distance != 0 && dist[u] + graph[u][v].factor < dist[v]) {
//                 parent[v] = u;
//                 dist[v] = dist[u] + graph[u][v].factor;
//                 path[v] = path[u] + graph[u][v].distance;
//             }
//         }
//     }

//     printSolution(dist, parent, src, dest, graph, dest, path);
// }

// // Function to plan a journey with stops
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
// // Function to plan a journey with stops and re-routing
void journeyPlanning(graph* g) {
    int source, destination, numStops;
    printf("Enter source area:\n");
    for (int i = 0; i < NUM_AREAS; i++) {
        printf("%d. %s\n", i, areas[i]);
    }
    scanf("%d", &source);

    printf("Enter destination area:\n");
    for (int i = 0; i < NUM_AREAS; i++) {
        printf("%d. %s\n", i, areas[i]);
    }
    scanf("%d", &destination);

    printf("Enter number of stops between source and destination:\n");
    scanf("%d", &numStops);

    if (source < 0 || source >= NUM_AREAS || destination < 0 || destination >= NUM_AREAS) {
        printf("Invalid source or destination area.\n");
        return;
    }

    if (numStops == 0) {
        // If no stops, calculate path directly from source to destination
        dijkstra(g->matrix, source, destination);

        // Check if user has reached the final destination
        bool reachedDestination = false;
        while (!reachedDestination) {
            printf("Have you reached the final destination at %s? (Y/N)\n", areas[destination]);
            char choice;
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y') {
                reachedDestination = true;
            } else if (choice == 'N' || choice == 'n') {
                printf("Where are you now? (Enter area number)\n");
                for (int j = 0; j < NUM_AREAS; j++) {
                    printf("%d. %s\n", j, areas[j]);
                }
                scanf("%d", &source);

                // Re-route from the current location to the destination
                dijkstra(g->matrix, source, destination);
            } else {
                printf("Invalid choice. Please enter Y or N.\n");
            }
        }
        return;
    }

    int stops[numStops];
    printf("Enter the areas for stops:\n");
    for (int i = 0; i < numStops; i++) {
        scanf("%d", &stops[i]);
    }

    int currentLocation = source;
    for (int i = 0; i < numStops; i++) {
        dijkstra(g->matrix, currentLocation, stops[i]);

        // Check if user has reached the stop
        bool reachedStop = false;
        while (!reachedStop) {
            printf("Have you reached the stop at %s? (Y/N)\n", areas[stops[i]]);
            char choice;
            scanf(" %c", &choice);
            if (choice == 'Y' || choice == 'y') {
                reachedStop = true;
            } else if (choice == 'N' || choice == 'n') {
                printf("Where are you now? (Enter area number)\n");
                for (int j = 0; j < NUM_AREAS; j++) {
                    printf("%d. %s\n", j, areas[j]);
                }
                scanf("%d", &currentLocation);

                // Re-route from the current location to the same stop
                dijkstra(g->matrix, currentLocation, stops[i]);
            } else {
                printf("Invalid choice. Please enter Y or N.\n");
            }
        }

        // Update current location after reaching the stop
        currentLocation = stops[i];
    }

    // Check if user has reached the final destination
    bool reachedDestination = false;
    while (!reachedDestination) {
        printf("Have you reached the final destination at %s? (Y/N)\n", areas[destination]);
        char choice;
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            reachedDestination = true;
        } else if (choice == 'N' || choice == 'n') {
            printf("Where are you now? (Enter area number)\n");
            for (int j = 0; j < NUM_AREAS; j++) {
                printf("%d. %s\n", j, areas[j]);
            }
            scanf("%d", &source);

            // Re-route from the current location to the destination
            dijkstra(g->matrix, source, destination);
        } else {
            printf("Invalid choice. Please enter Y or N.\n");
        }
    }
}


void eta(int* h,int* m,int d,int factor)
{
    float f;
    if(factor<=2)
    {
        f=d/40.0;
        
    }
    else if(factor<=4)
    {
        f=d/5.0;
    }
    else
        f=d/10.0;

    (*h)+=f;
    int t=f;
    (*m)+=(f-t)*60;
    if((*m)>=60)
        {
            (*h)+=1;
            (*m)-=60;
        }
    if((*h)>23)
        (*h)-=24;
}


int main() {
    graph g;
    initgraph(&g);
    time1(1, 0, &g);
    journeyPlanning(&g);
    int t=3;
    int m=20;
    eta(&t,&m,30,1);
    printf("%d:%d",t,m);

    return 0;
}
