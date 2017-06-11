#include<iostream>
#include<vector>

int getMinVertex(int dist[],bool visit[],int v) 
{
	int index=0;
	int min = INT_MAX;
	for (int i = 0; i < v; i++) {
		if (visit[i] == false && dist[i] < min) {
			min = dist[i];
			index = i;
		}
	}
	return index;
}

void print(int dist[], int v) {
	for (int i = 0; i < v; i++) {
		std::cout << "从源点到点" << i << "距离为" << dist[i] << std::endl;
	}

}

void Dijkstra(std::vector<std::vector<int>> graph, int src) 
{
	int v = graph.size();
	bool visit[20] ;
	int dist[20] ;

	for (int i = 0; i < v; i++) {
		visit[i] = false;
		dist[i] = INT_MAX ;
	}
	dist[0] = 0;

	

	//std::cout << dist << std::endl;
	for (int i = 0; i < v; i++) {
		int index = getMinVertex(dist, visit, v);
		visit[index] = true;
		for (int j = 0; j < v; j++) {
			if (graph.at(index).at(j)+dist[index]< dist[j]&&!visit[j]&&(graph.at(index).at(j)!=0)) {
				dist[j] = graph.at(index).at(j)+dist[index];
			}
		}
	}
	

	print(dist, v);

}



int main() 
{
	std::vector<std::vector<int>> graph =
	{
		{ 0, 4, 0, 0, 0, 0, 0, 8, 0 },
		{ 4, 0, 8, 0, 0, 0, 0, 11, 0 },
		{ 0, 8, 0, 7, 0, 4, 0, 0, 2 },
		{ 0, 0, 7, 0, 9, 14, 0, 0, 0 },
		{ 0, 0, 0, 9, 0, 10, 0, 0, 0 },
		{ 0, 0, 4, 0, 10, 0, 2, 0, 0 },
		{ 0, 0, 0, 14, 0, 2, 0, 1, 6 },
		{ 8, 11, 0, 0, 0, 0, 1, 0, 7 },
		{ 0, 0, 2, 0, 0, 0, 6, 7, 0 }
	};
	// 单源最短路径
	Dijkstra(graph, 0);
	system("pause");
	return 0;
}