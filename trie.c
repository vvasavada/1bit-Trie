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

unsigned int trie::addr_to_int(string prefix)
{
    unsigned int value =0;
    char * pch = strtok (strdup(prefix.c_str()),".");
    while (pch != NULL)
    {
        value = value <<8;
        value = value + atoi(pch);
        pch = strtok (NULL, ".");
    }

    return value;
}

void trie::insert(string prefix, int mask, string nexthop)
{
    int len = 0;
	if(head==NULL){			//------------First insert-----------------
		head = new node;
        len += 1;
    }

    
    unsigned int value = addr_to_int(prefix);
    int sofar = 0;
    node* current=head;
    cout << "mask " << mask << "\n";
    for(int i=0;i<mask;i++)
    {
        unsigned int tmp = value;
        bool bit = (tmp & ( 1 << 31-i )) >> 31-i;
        sofar = (sofar << 1) + bit;
        if(bit){
            if(current->child[1]==NULL){
                current->child[1] = new node;
                cout << "sofar " << sofar << "\n";
                cout << "len " << len << "\n";
                lentable[len].insert(pair<int, node*>(sofar, current->child[1]));
            }
            current = current->child[1];
        }
        else{
            if(current->child[0]==NULL){
                current->child[0] = new node;
                cout << "sofar " << sofar << "\n";
                cout << "len " << len << "\n";
                lentable[len].insert(pair<int, node*>(sofar, current->child[0]));
            }
            current = current->child[0];
        }

        len += 1;
    }
    if(current->data !=NULL){
        cout<<"Collisions!!!!\n";
    }
    current->data = new prefix_data;
    current->data->nexthop.assign(nexthop);
    current->data->prefix.assign(prefix);
}

trie::node* trie::search(string prefix, int hint){
    int value = addr_to_int(prefix);
    unsigned int tmp = value;

    int sofar = tmp >> 32 - hint;  
    map<int, node*> prefix_table = lentable[hint];
    map<int, node*>::iterator it;

    it = prefix_table.find(sofar);
    if (it == prefix_table.end()){
        cout << "match not found!" << "\n";
        return NULL;
    }

    tmp = value;
    node* current = it->second;
    node* last_match = NULL;

    for (int i = hint;;i++){
        if (current->data != NULL){
            last_match = current;
        }

        bool bit = (tmp & ( 1 << 31-i )) >> 31-i;
        if (bit){
            if(current->child[1] == NULL){
                return last_match;
            }
            current = current->child[1];
        } else {
            if(current->child[0] == NULL){
                return last_match;
            }
            current = current->child[0];
        }
    }
}

void trie::printer(node* n, string sofar)
{
    if(n==NULL){
        //cout<<sofar<<"\n";
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

void trie::print_node(node* n)
{
    if (n != NULL) {
        cout << n->data->prefix << "\n";
        cout << n->data->nexthop << "\n";
    }
}

int main()
{
 string p1 = string("150.170.180.190");
 string p2 = string("225.192.193.192");
 string p3 = string("204.205.206.207");
 string p4 = string("225.192.193.194");
 string p5 = string("12.13.14.15");
 string p6 = string("128.129.130.136");
 string p7 = string("128.129.130.137");
 string p8 = string("128.129.130.138");

 string prefixes[8] = {p1, p2, p3, p4, p5, p6, p7, p8};
 int masks[8] = {2, 3, 5, 1, 1, 4, 6, 7};

 trie t;
 for (int i = 0; i < 8; i++){
    t.insert(prefixes[i], masks[i], string("1.2.3.4"));
 }
 t.print_trie();

 cout << "searching for 194.170.51.170...\n";
 t.print_node(t.search(string("194.170.51.170"), 1));

 cout << "searching for 130.170.51.170...\n";
 t.print_node(t.search(string("130.170.51.170"), 4));
}