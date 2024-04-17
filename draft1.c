#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#define NUM_AREAS 17
// enum ar{
//     Balewadi,Baner,Sppu,Bavdhan,Pashan,Kothrud,Dhayari,Rajaram_Bridge,PMC,Dandekar_Bridge,Katraj,Bibwewadi,Swargate,Kondhwa,Koregaon_Park,Camp,COEP
// };
char* areas[17]={"Balewadi","Baner","Sppu","Bavdhan","Pashan","Kothrud","Dhayari","Rajaram_Bridge","PMC","Dandekar_Bridge","Katraj","Bibwewadi","Swargate","Kondhwa","Koregaon_Park","Camp","COEP"};

typedef struct node{
    int distance;
    int intensity;
    int factor;
}node;

typedef struct graph{
    node** matrix;
}graph;

typedef struct arr{
    int A[100];
    int length;
}arr;

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
            g->matrix[i][j].factor=g->matrix[i][j].distance+2*g->matrix[i][j].intensity;
            g->matrix[j][i].factor=g->matrix[j][i].distance+2*g->matrix[j][i].intensity;
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
        f=d/25.0;
    }
    else
        f=d/15.0;

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
void printPath(int parent[], int j, int dist, node** graph, int d,int dist1[], int* sol, int index) {
    if (parent[j] == -1) {
        // Print the vertex for the initial node
        printf(" %s ", areas[j]);
        sol[index++] = j;
        return;
    }

    printPath(parent, parent[j], graph[parent[j]][j].factor, graph, d,dist1,sol,index+1);

    // Determine the color based on distance between adjacent vertices
    if (parent[j] != d) { // Avoid printing "->" after reaching the destination
        int parentDist = graph[parent[j]][j].factor;
        if (graph[parent[j]][j].intensity <= 2){
            printf("\x1b[1;34m--%d-->\x1b[0m",dist1[j]-dist1[parent[j]]);
            // sleep(1);
        }
        else if (graph[parent[j]][j].intensity>=3 && graph[parent[j]][j].intensity <= 4){
            printf("\x1b[1;33m--%d-->\x1b[0m",dist1[j]-dist1[parent[j]]);
            // sleep(2);
        }
        else{
            printf("\x1b[1;31m--%d-->\x1b[0m",dist1[j]-dist1[parent[j]]);
            // sleep(3);
        }
            // Reset color
    }

    // Print the vertex
      //  printf("%d ",dist1[j]-dist1[parent[j]]);
        
        printf(" %s ", areas[j]);
        sol[index] = j;
        
}





// Function to print the distance array
void printSolution(int dist[], int parent[], int src, int dest,node** graph,int d,int path[], int* sol) {
    printf("Shortest Path from %s to %s: %d\nPath: ", areas[src], areas[dest], path[dest]);
    printPath(parent, dest, dist[dest], graph,d,path, sol, 0); // Pass the graph parameter correctly
    printf("\n");
}

// Function to implement Dijkstra's algorithm for a given graph
void dijkstra(node** graph, int src, int dest, int* sol) {
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
    printSolution(dist, parent, src,dest,graph,dest,path,sol);
}

// void journery_planner(graph* g) {
//     printf("\nHello! I am here to assist you with your journey!\n"); 
//     printf("Enter the starting location: ");
//     char s[50];
//     scanf("%s",s);
//     printf("\nEnter your destination: ");
//     char d[50];
//     scanf("%s,d");
//     printf("\nDo you wish to add any more stops in between? Type 1 for Yes, 0 for No\n");
//     int choice;
//     scanf("%d",&choice);
//     if(choice==0) {
//         int srcindex = find_area_index(s);
//         int destindex = find_area_index(d);
//         dijkstra(g->matrix,srcindex, destindex);
//     }
// }

void rev(int* arr,int ct){
    int i=0;
    int j=ct-1;
    while(i<j){
        int t=arr[i];
        arr[i]=arr[j];
        arr[j]=t;
        i++;
        j--;
    }
    return;
}

void findroute(graph g,int s,int d,arr* tpath);
int find_area_index(char s[]);

void adjacent(graph g,int index,int d,arr* tpath){
    printf("PROBABLY YOU ARE IN THESE AREAS\n");
    for(int i=0;i<17;i++){
        if(g.matrix[i][index].distance){
            printf("%s\t\t",areas[i]);
        }
    }
    char s[100];
    printf("\nWhere are you\n");
    scanf("%s",s);
    tpath->A[tpath->length++]=find_area_index(s);
    findroute(g,find_area_index(s),d,tpath);

}

void findroute(graph g,int s,int d,arr* tpath){
     int sol[100];
     int diffpath[30];

    for(int i=0; i<100; i++) {
        sol[i] = -1;
    }
    dijkstra(g.matrix,s,d,sol);
    int ct=0;
    printf("\n");
    while(sol[ct]!=-1) {
        ct++;
    }
    rev(sol,ct);
    int i=0;
    int c=1;
    i=1;

    while(c==1){
        if(i==ct){
            printf("\nYOU HAVE REACHED YOUR DESTINATION\n");
            break;
        }
        printf("\nHave You reached %s?(1 for Yes/0 for No)\n",areas[sol[i++]]);
        fflush(stdin);
        scanf("%d",&c);
        if(c==0){
            adjacent(g,sol[i-2],d,tpath);
            return;
        }
        if(c==1){
            tpath->A[tpath->length++]=sol[i-1];
            for(int i=0;i<tpath->length;i++){
                //printf("HERE%d\n",tpath->length);
                if(tpath->length==1){
                    printf("%s ",areas[tpath->A[i]]);
                    break;
                }
                if(i==0){
                    printf("%s ",areas[tpath->A[i]]);
                    continue;
                }
                if (g.matrix[tpath->A[i-1]][tpath->A[i]].intensity <= 2){
                printf("\x1b[1;34m--%d-->\x1b[0m",g.matrix[tpath->A[i-1]][tpath->A[i]].distance);
                // sleep(1);
                }
                else if (g.matrix[tpath->A[i-1]][tpath->A[i]].intensity>=3 && g.matrix[tpath->A[i-1]][tpath->A[i]].intensity <= 4){
                    printf("\x1b[1;33m--%d-->\x1b[0m",g.matrix[tpath->A[i-1]][tpath->A[i]].distance);
                    // sleep(2);
                }
                else{
                    printf("\x1b[1;31m--%d-->\x1b[0m",g.matrix[tpath->A[i-1]][tpath->A[i]].distance);
                    // sleep(3);
                }
                printf("%s ",areas[tpath->A[i]]);
                
            }
        }
    }
    //disp(&g);
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

int finddistance(graph* g, int sol[], int sindex, int dindex) {
    int j=0;
    int cnt=0;
    while(sol[cnt]!=-1){
        cnt++;
    }
    for(int i=0;i<cnt/2;i++){
        int t=sol[i];
        sol[i]=sol[cnt-i-1];
        sol[cnt-i-1]=t;
    }
    int ans= g->matrix[sindex][sol[0]].distance;
    while(sol[j+1]!=-1) {
        //printf("HERE");
        //printf("ans=%d\n",ans);
        ans+= g->matrix[sol[j]][sol[j+1]].distance;
        j++;
    }
    ans+=g->matrix[sol[j]][dindex].distance;
    printf("ans=%d\n",ans);
    return ans;
}

int find_time(graph* g,int sol[],int sindex,int dindex,int *h,int* m){
    eta(h,m,g->matrix[sindex][sol[0]].distance,g->matrix[sindex][sol[0]].intensity);
    printf("t=%d %d\n",*h,*m);
    int j=0;
    while(sol[j+1]!=-1){
        eta(h,m,g->matrix[sol[j]][sol[j+1]].distance,g->matrix[sol[j]][sol[j+1]].intensity);
        printf("t=%d %d\n",*h,*m);
        j++;
    }
    eta(h,m,g->matrix[sol[j]][dindex].distance,g->matrix[sol[j]][dindex].intensity);
    printf("t=%d %d\n",*h,*m);
}

// void printentirepath(int sol[],int f,int sindex,int dindex){
//     if(f==0){
//         printf("")
//     }
// }

void journeyPlanning(graph* g) {
    int t=-1,min=-1;
    while(t<0 || t>=24 || (min<0 || min>60)){
            printf("ENTER HOUR OF THE DAY IN 24 HOUR FORMAT(0-23)\n");
            scanf("%d",&t);
            printf("ENTER MINUTE OF THE DAY(0-60)\n");
            scanf("%d",&min);
            if(t<0 || t>=24 || min<0 || min>60){
                printf("ENTER VALID TIME IN 24 HOUR FORMAT\n");
            }
    }
    int h=t,m=min;
    time1(t,min,g);
    char source[20], destination[20]; 
    int numStops;

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

    printf("\nEnter source area:\n");
    scanf("%s", source);
    printf("Enter destination area:\n");
    


    scanf("%s", destination);
    int sindex = find_area_index(source);
    int dindex = find_area_index(destination);
    printf("Enter number of stops between source and destination:\n");
    scanf("%d", &numStops);

    if (sindex < 0 || sindex >= NUM_AREAS || dindex < 0 || dindex >= NUM_AREAS) {
        printf("Invalid source or destination area.\n");
        return;
    }
    arr tpath;
    tpath.length=0;
    for(int i=0;i<100;i++){
        tpath.A[i]=-1;
    }
    if (numStops == 0) {
        findroute(*g,find_area_index(source),find_area_index(destination),&tpath);
        
        return;
    }

    int stops[numStops];
    printf("Enter the areas for stops:\n");
    for (int i = 0; i < numStops; i++) {
        char stopname[20];
        scanf("%s", stopname);
        stops[i] = find_area_index(stopname);
    }
    int sol[100];
    for(int i=0; i<100; i++) {
        sol[i]=-1;
    }

    printf("\nHERE IS YOUR ENTIRE PATH: \n");
    dijkstra(g->matrix,sindex,stops[0],sol);
    int j=0;
    int distvar=0;
    distvar+= finddistance(g,sol,sindex,stops[0]);
    find_time(g,sol,sindex,stops[0],&t,&min);
    printf("ETA=%d:%d\n",t,min);
    printf("TOTAL DISTANCE=%d\n",distvar);
    int k=0;
    while(sol[k]!=-1){
        printf("%d ",sol[k]);
        k++;
    }
    for(int i=0; i<numStops-1; i++) {
        for(int i=0; i<100; i++) {
            sol[i]=-1;
        }
        dijkstra(g->matrix,stops[i],stops[i+1],sol);
        distvar+= finddistance(g,sol,stops[i],stops[i+1]);
        find_time(g,sol,stops[i],stops[i+1],&t,&min);
        printf("ETA=%d:%d\n",t,min);
        printf("TOTAL DISTANCE=%d\n",distvar);
    }
    for(int i=0; i<100; i++) {
        sol[i]=-1;
    }


    dijkstra(g->matrix,stops[numStops-1],dindex,sol);
    distvar+= finddistance(g,sol,stops[numStops-1],dindex);
    find_time(g,sol,stops[numStops-1],dindex,&t,&min);
    printf("ETA=%d:%d\n",t,min);
    printf("TOTAL DISTANCE=%d\n",distvar);


    printf("\n");
    printf("Enter 1 to start the journey! \n");
    int choice;
    scanf("%d",&choice);
    if(choice==1) {
        
        int currentLocation = find_area_index(source);
        for (int i = 0; i < numStops; i++) {
        //dijkstra(g->matrix, currentLocation, stops[i]);
            findroute(*g,currentLocation,stops[i],&tpath);
            currentLocation = stops[i];
        }
        findroute(*g,stops[numStops-1],dindex,&tpath);
        for(int i=0;i<tpath.length;i++){
            printf("%d ",tpath.A[i]);
        }
        find_time(g,tpath.A,sindex,dindex,&h,&m);
        printf("\nETA=%d:%d\n",h,m);
        rev(tpath.A,tpath.length);
        printf("\n");
        //printf("hh-%d %d %d\n",sindex,tpath.A[0],g->matrix[sindex][tpath.A[0]].distance);
        int fans=(finddistance(g,tpath.A,sindex,dindex));
        //int y=(g->matrix[sindex][tpath.A[0]].distance);
        //printf("%d %d\n",fans,y);
        printf("TOTAL DISTANCE COVERED = %d\n",fans);
    }
    else return;
    
}


int main(){
    graph g;
    initgraph(&g);
    printf("%d",g.matrix[1][4].distance);
    for(int i=0;i<17;i++){
        if(g.matrix[i][16].distance){
            printf("%s\t\t",areas[i]);
        }
    }
    printf("\033[1m\n\n\t\t\t\t\t\tWELCOME TO TRAFFIC AND ROAD NAVIGATOR\n\n\033[0m");
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
        else if(c==2){
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
                    printf("ENTER MINUTE OF THE DAY(0-60)\n");
                    scanf("%d",&min);
                    if(t<0 || t>=24 || min<0 || min>60){
                        printf("ENTER VALID TIME IN 24 HOUR FORMAT\n");
                    }
                }
                time1(t,min,&g);
                // disp(&g);
                int sol[100];
                for(int i=0;i<100;i++){
                    sol[i]=-1;
                }
                dijkstra((g.matrix), x,y,sol);
                printf("TOTAL DISTANCE=%d\n",finddistance(&g,sol,x,y));
                find_time(&g,sol,x,y,&t,&min);
                printf("ETA=%d %d\n",t,min);
                }
        }
        else if(c==3){

            journeyPlanning(&g);
        }
        else if(c==4){
            break;
        }
        else{
            printf("ENTER A VALID CHOICE(1-4)\n");
        }
    }
    
    
    return 0;
}