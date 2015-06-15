#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <set>

using namespace std;

struct gnode {
    int data;
    gnode *next;
};

class Graph {
    gnode **adjList;
    int vcount;

public:
  Graph(int count) {
      vcount = count;
      adjList = new gnode* [vcount];
      for (int i=0; i<vcount; i++) {
          adjList[i] = NULL;
      }
  }

 void Clear() {
    for (int i=0; i<vcount; i++) {
      if (adjList[i]) {
          gnode* node = adjList[i];

          while (node) {
              gnode* tmp = node->next;
              delete node;
              node = tmp;
          }
      }
    }
    delete [] adjList;
  }

  void AddEdge(int start, int end) {

      if (start >= vcount) {
          cerr << "Can't add vertex " << start << endl;
          cerr << "Exceeds graph vertex count" << endl;
      }
      if (end >= vcount) {
          cerr << "Can't add vertex " << end << endl;
          cerr << "Exceeds graph vertex count" << endl;
      }
      gnode* node = adjList[start];

      if (!node) {
          node = new gnode;
          node->data = start;
          node->next = NULL;
          adjList[start] = node;
      }
      else {
          while(node->next != NULL) {
              node = node->next;
          }
      }

      gnode *nextNode = new gnode;
      nextNode->data = end;
      nextNode->next = NULL;

      node->next = nextNode;

      if (!adjList[end]) {
          adjList[end] = new gnode;
          adjList[end]->data = end;
          adjList[end]->next = NULL;
      }

  }

  void Print() {
    for (int i=0; i<vcount; i++) {
      if (adjList[i]) {
          gnode *node = adjList[i];
          while (node) {
              cout << node->data << " ";
              node = node->next;
          }
          cout << endl;
      }
    }
  }

  void BFSCycleCheck() {

    bool* visited = new bool[vcount];
    memset(visited, 0, vcount);

    list<gnode*> queue;

    set<pair<int,int> > edgeList;

    queue.push_back(adjList[0]);
    visited[0] = true;


    while (!queue.empty()) {

      gnode* v = *(queue.begin());
      queue.pop_front();

      cout << "Data: " << v->data << endl;

      gnode *adj = adjList[v->data]->next;

      while (adj) {

        pair <int, int> edge = make_pair(v->data, adj->data);
        cout << "Inserting edge : " << v->data << " " << adj->data << endl;
        edgeList.insert(edge);

        if (!visited[adj->data]) {
          visited[adj->data] = true;
          queue.push_back(adj);

        }
        else {
          // Cycle check
          gnode* edges = adjList[adj->data]->next;
          while (edges) {
            cout << "Searching edge : " << adj->data << " " << edges->data << endl;
            if (edgeList.find(make_pair(adj->data, edges->data)) != edgeList.end()) {
              cout << "Loop found on edge " << adj->data << " " << edges->data << endl;
              break;
            }
            edges = edges->next;
          }
        }
        adj = adj->next;
      }
    }

  }


};



int main() {
   Graph *g = new Graph(5);

   g->AddEdge(0,1);
   g->AddEdge(1,2);
   g->AddEdge(2,0);
   //g->AddEdge(0,1);
   g->Print();

   g->BFSCycleCheck();

  return 0;
}
