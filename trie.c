#include "trie.h"
#include <string.h>
trie::trie()
{
	head=NULL;
}

trie::~trie()
{
	delete head;
}

void trie::insert(string prefix, int mask, string nexthop)
{
	if(head==NULL)				//------------First insert-----------------
		head = new node;
    int value =0;
    char * pch = strtok (strdup(prefix.c_str()),".");
    while (pch != NULL)
    {
        value = value <<8;
        value = value + atoi(pch);
        pch = strtok (NULL, ".");
    }
    node* current=head;
    for(int i=0;i<mask;i++)
    {
        int tmp = value;
        bool bit = (tmp & ( 1 << 31-i )) >> 31-i;
        if(bit){
            if(current->child[1]==NULL){
                current->child[1] = new node;
            }
            current = current->child[1];
        }
        else{
            if(current->child[0]==NULL){
                current->child[0] = new node;
            }
            current = current->child[0];
        }

    }
    if(current->data !=NULL){
        cout<<"Collisions!!!!\n";
    }
    current->data = new prefix_data;
    current->data->nexthop.assign(nexthop);
    current->data->prefix.assign(prefix);
}
void trie::printer(node* n, string sofar){
   
    
    if(n==NULL){
        cout<<sofar<<"\n";
        return;
    }
    if(n->data!=NULL){
        cout<<"\t"<<sofar<<"\n";
        cout<<"\t\t"<<n->data->prefix<<"\n\t\t";
        cout<<n->data->nexthop<<"\n";
    }
    
    printer(n->child[0],sofar+"0");
    printer(n->child[1],sofar+"1");
}

void trie::print_trie()
{
    printer(head,"");
}

int main()
{
 trie t;
 t.insert(string("12.13.14.15"),2,"15.16.17.18");
 t.insert(string("12.13.14.15"),3,"15.16.17.18");
 t.print_trie();
}