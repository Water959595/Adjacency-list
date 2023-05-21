#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
	int data;
	Node* next;
}Node_t;


typedef struct adjList
{
	Node* head;			//headNode of adjlist(to store the address of first node)
}adjList_t;


typedef struct Graph
{
	int verticeNum;
	adjList* adjListArr;	//array of pointer: address of each headNodes
	bool* visited;			//array of int:		visited or not
}Graph_t;


Graph_t* createGraph(int verticeNum);
Node_t* createNewNode(int data);
void addEdge(Graph_t* graph, int src, int dest, bool isDirected);
void printGraph(Graph_t* graph);

void DFS(Graph_t* graph, int start);
void BFS(Graph_t* graph, int start);


int main()
{
	int vNum = 0;			//input - the numebr of the vertices in the graph
	int selecGraph = 0;		//input - choose Directed or Undirected
	int edgeNum = 0;		//input - the number of destinations of vertex
	int neighbor = 0;		//input - the value number of neighbor vertex
	Graph_t* graph;			//create a graph


	printf("Enter the number of the vertices in the graph >> ");
	scanf("%d", &vNum);
	graph = createGraph(vNum);


	do
	{
		printf("\n** Main Table **\n");
		printf("1. Directed graph\n");
		printf("2. Undirected graph\n");

		printf("Select a function you want to test (1~2)>> ");
		scanf("%d", &selecGraph);
	} while (selecGraph != 1 && selecGraph != 2);


	for (int i = 0; i < vNum; i++)
	{
		printf("Enter the number of destinations of vertex %d >> ", i);
		scanf("%d", &edgeNum);

		for (int j = 0; j < edgeNum; j++)
		{
			printf("Vertex %d: destination %d >> ",i , j + 1);
			scanf("%d", &neighbor);


			if (selecGraph == 1)		//1. Directed graph
			{
				addEdge(graph, i, neighbor, 1);
			}
			else if (selecGraph == 2)	//2. Undirected graph
			{
				addEdge(graph, i, neighbor, 0);
			}
		}
		printf("\n");
	}

	printGraph(graph);

	printf("\n\nDFS traversal: ");
	DFS(graph, 0);
	printf("\n");

	printf("\n\nBFS traversal: ");
	BFS(graph, 0);
	printf("\n");

	printf("\n");
	system("pause");
	return 0;
}


/*=============================== Function area ===================================*/

//func(1)
Graph_t* createGraph(int verticeNum)
{
	Graph_t* graph = (Graph_t*)malloc(sizeof(Graph_t));

	graph->verticeNum = verticeNum;
	graph->adjListArr = (adjList_t*)malloc(sizeof(adjList_t) * verticeNum);
	graph->visited = (bool*)malloc(sizeof(bool) * verticeNum);

	//initialize
	for (int i = 0; i < verticeNum; i++)
	{
		graph->adjListArr[i].head = nullptr;
		graph->visited[i] = false;
	}

	return graph;
}

//func(2)
Node_t* createNewNode(int data)
{
	Node_t* newNode = (Node_t*)malloc(sizeof(Node_t));
	newNode->data = data;
	newNode->next = nullptr;
	return newNode;
}


//func(3)
void addEdge(Graph_t* graph, int src, int dest, bool isDirected)
{
	Node_t* srcNode;
	Node_t* destNode;
	Node_t* tempPtr;		//helper 


	/*Directed(from src to dest)(add the destNode into src adjlist)*/
	destNode = createNewNode(dest);				//create a dest node

	//case: headNode doesn't point to other node yet
	if (graph->adjListArr[src].head == nullptr)
	{
		graph->adjListArr[src].head = destNode;	//let head pointer to first node(dest)
	}
	else
	{
		tempPtr = graph->adjListArr[src].head;	//store address srcNode (start to find nullptr)
		while (tempPtr->next != nullptr)		//finding the tail of adjlist
		{
			tempPtr = tempPtr->next;
		}
		tempPtr->next = destNode;			//let tail's next point to new destNode
	}


	/*unDirected(from dest to src)(add the destNode(src) into dest adjlist)*/
	if (!isDirected)
	{
		destNode = createNewNode(src);

		if (graph->adjListArr[dest].head == nullptr)
		{
			graph->adjListArr[dest].head = destNode;
		}
		else
		{
			tempPtr = graph->adjListArr[dest].head;
			while (tempPtr->next != nullptr)
			{
				tempPtr = tempPtr->next;
			}
			tempPtr->next = destNode;
		}
	}
}


//func(4)
void printGraph(Graph_t* graph)
{
	for (int i = 0; i < graph->verticeNum; i++)
	{
		Node_t* triv = graph->adjListArr[i].head;

		printf("\n Adjacency list of vertex %d\n head ", i);
		while (triv)
		{
			printf("-> %d", triv->data);	//print vertex
			triv = triv->next;				//renew
		}
		printf("\n");
	}
}


//func(5)
void DFS(Graph_t* graph, int start)
{
	Node_t* triv;	//traversal pointer

	graph->visited[start] = true;		//had visited
	printf("%d ", start);				//access

	triv = graph->adjListArr[start].head;

	while (triv != nullptr)		//keeping visit until NULL
	{
		if (graph->visited[triv->data] == false)  //unvisited
		{
			DFS(graph, triv->data);		//pass that node, do DFS again(recursively)
		}

		triv = triv->next;		//renew to visit next
	}
}


//func(6)
void BFS(Graph_t* graph, int start)
{
	int* queue = (int*)malloc(sizeof(int) * graph->verticeNum);
	int front = -1;
	int rear = -1;
	Node_t* triv;		//traversal pointer


	//re-initialize the visited[] after DFS
	for (int i = 0; i < graph->verticeNum; i++)
	{
		graph->visited[i] = false;
	}


	//check each vertex is visited
	for (start; start < graph->verticeNum; start++)
	{
		if (graph->visited[start] == false)	//unvisited
		{
			//enqueue: store the starting vertex
			queue[++rear] = start;
			graph->visited[start] = true;	//mark visited


			while (front < rear) //data exists in queue, (front == rear => No data)
			{
				printf("%d ", queue[++front]);	//access the first data (dequeue)

				triv = graph->adjListArr[start].head;

				//check neighbor exist or not
				while (triv != nullptr)
				{
					//check "neighbor" is visited or not
					if (graph->visited[triv->data] == false)	
					{
						queue[++rear] = triv->data;			//enqueue
						graph->visited[triv->data] = true;	//mark visited
						printf("%d ", queue[++front]);		//access that vertex(dequeue)
					}

					triv = triv->next;		//renew the traversal vertex
				}
			}
		}

	}

}