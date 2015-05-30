#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define ALLOC 11
#define PAD -1
#define START -2
#define END -3


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
          cerr << "Exceeds graph vertext count" << endl;
      }
      if (end >= vcount) {
          cerr << "Can't add vertex " << end << endl;
          cerr << "Exceeds graph vertext count" << endl;
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
  
  void* MemDump(int& count, int& size) {
    int MAX = ALLOC;
    
    if (MAX%2 == 0)
      MAX++;
      
    int *buffer = (int*)malloc(MAX);
    int *tmpbuff = buffer;
    
    //cout << "INT Size " << sizeof(int) << endl;
   
    for (int i=0; i<vcount; i++) {
      if (adjList[i]) {
          gnode *node = adjList[i];
          while (node) {
            if (tmpbuff+sizeof(int) > buffer+MAX) {
                //cout << "1.Tmp buff:" << tmpbuff << endl;
                //cout << "2.Buff:" << buffer << endl;
                MAX += ALLOC;
                
                if (MAX%2 == 0)
                MAX++;
                
                buffer = (int*)realloc(buffer, MAX);
                tmpbuff = buffer + count*sizeof(int);
                //cout << "After" << endl;
                //cout << "1.Tmp buff:" << tmpbuff << endl;
                //cout << "2.Buff:" << buffer << endl;
            }
            *tmpbuff = node->data;
            //cout << "Data: " << *tmpbuff << " Buff: " << tmpbuff << endl;
            count++;
            tmpbuff += sizeof(int);
            node = node->next;
          }
          
          if (tmpbuff+sizeof(int) > buffer+MAX) {
            //cout << "3.Tmp buff:" << tmpbuff << endl;
            //cout << "4.Buff:" << buffer << endl;
            MAX += ALLOC;
            
            if (MAX%2 == 0)
            MAX++;
            
            buffer = (int*)realloc(buffer, MAX);
            tmpbuff = buffer + count*sizeof(int);
            //cout << "After" << endl;
            //cout << "3.Tmp buff:" << tmpbuff << endl;
            //cout << "4.Buff:" << buffer << endl;
          }
          
          *tmpbuff = PAD;
          count++;
          //cout << "Pad Data: " << *tmpbuff << " Buff: " << tmpbuff << endl;
          tmpbuff += sizeof(int);
      }
    }  
    if (tmpbuff+sizeof(int) > buffer+MAX) {
      MAX += sizeof(int);
      buffer = (int*)realloc(buffer, MAX);
      tmpbuff = buffer + count*sizeof(int);
    }
    *tmpbuff = END;
    count++;
    size = MAX;
    return buffer;
  }
  
  
  void PrintMemDump(void* buff, int count) {
      int *intbuff = (int*)buff;
      while (count--) {
          cout << "Location: " << intbuff << " Data: " << (*intbuff) << " ";
          intbuff += sizeof(int);
      }
      cout << endl;
  }
  
  void DiskDump(void *buff, int size, FILE* fp) {
      //fwrite(buff, sizeof(int), count, fp);
      fwrite(buff, 1, size, fp);
  }
  
  void DiskLoad(void *newbuff, int size, FILE* fp) {
      //fread(newbuff, sizeof(int), count, fp);
      fread(newbuff, 1, size, fp);
  }
  
  void Restore(void* newbuff, int count) {
      int* ibuff = (int*)newbuff;
      adjList = new gnode* [vcount];
      for (int i=0; i<vcount; i++) {
          adjList[i] = NULL;
      }
      cout << "Graph node count: " << vcount << endl;
      gnode* curr = NULL;
      
      while (count--) {
        int data = *ibuff;
        //cout << "Read back: " << data << endl;
        ibuff += sizeof(int);
        if (data == PAD) {
            curr = NULL;
            continue;
        }
        else if (data == END) {
            break;
        }
        if (!curr) {
          if (!adjList[data]) {
            //cout << "Creating new list " << data << endl;
            adjList[data] = new gnode;
            adjList[data]->data = data;
            adjList[data]->next = NULL;
            curr = adjList[data];
          }
        }
        else {
            gnode* nextNode = new gnode;
            nextNode->data = data;
            nextNode->next = NULL;
            curr->next = nextNode;
            //cout << "adding " << data << "next to " << curr->data << endl;
            curr = nextNode;
        }
      }
  } 
};

int main()
{
   FILE* fp = fopen("diskdump", "wb");
   
   if (!fp) {
       cout << "Couldn't open file for writing." << endl;
   }
   
   Graph *g = new Graph(5);
   g->AddEdge(0,1);
   g->AddEdge(0,2);
   g->AddEdge(0,3);
   g->AddEdge(1,2);
   g->AddEdge(1,3);
   g->AddEdge(2,3);
   g->AddEdge(3,4);
   
   g->Print();
   
   int count = 0, size = 0;
   void* buff = g->MemDump(count, size);
   
   //g->DiskDump(buff, count, fp);
   g->DiskDump(buff, size, fp);
   
   g->PrintMemDump(buff, count);
   
   fclose(fp);
   free(buff);
   
   g->Clear();
   
   fp = fopen("diskdump", "rb");
   
   if (!fp) {
       cout << "Couldn't open file for reading." << endl;
   }
   //void* newbuff = malloc(sizeof(int)*count);
   void* newbuff = malloc(size);
   
   //g->DiskLoad(newbuff, count, fp);
   g->DiskLoad(newbuff, size, fp);
   
   g->PrintMemDump(newbuff, count);
   
   g->Restore(newbuff, count);
   
   g->Print();
   return 0;
}
