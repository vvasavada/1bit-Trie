#include "trie.h"
#include "timer.h"
#include <string.h>
#include <fstream>

trie::trie()
{
	head=NULL;
}

trie::~trie()
{
    pointer_deletion(head);  
}

void trie::pointer_deletion(node *n){
    if(n->child[0]!= NULL){
        pointer_deletion(n->child[0]);
    }
    if(n->child[1]!= NULL){
        pointer_deletion(n->child[1]);
    }
    delete n;
   
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
    int len = 1;

    if(head==NULL)		//------------First insert-----------------
        head = new node;

    unsigned int value = addr_to_int(prefix);
    int sofar = 0;
    node* current=head;
    for(int i=0;i<mask;i++)
    {
        unsigned int tmp = value;
        bool bit = (tmp & ( 1 << 31-i )) >> 31-i;
        sofar = (sofar << 1) + bit;
        if(bit){
            if(current->child[1]==NULL){
                current->child[1] = new node;
                lentable[len-1].insert(pair<int, node*>(sofar, current->child[1]));
            }
            current = current->child[1];
        }
        else{
            if(current->child[0]==NULL){
                current->child[0] = new node;
                lentable[len-1].insert(pair<int, node*>(sofar, current->child[0]));
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

prefix_data* trie::search(string prefix, int hint){
    int value = addr_to_int(prefix);
    unsigned int tmp = value;
    node* current = NULL;

    if (hint != 0){
        // auto timer = Timer();
        int sofar = tmp >> 32 - hint;  
        unordered_map<int, node*>* prefix_table = &lentable[hint-1];
        unordered_map<int, node*>::iterator it;
        it = prefix_table->find(sofar);
        // cout << "debug: " << timer.elapsed() << "\t";
        if (it == prefix_table->end()){
            return NULL;
        }
        current = it->second;

    } else {
        current = head;
    }

    tmp = value;
    node* last_match = NULL;

    for (int i = hint;;i++){
        if (current->data != NULL){
            last_match = current;
        }

        bool bit = (tmp & ( 1 << 31-i )) >> 31-i;
        if (bit){
            if(current->child[1] == NULL){
                return last_match==NULL? NULL:last_match->data;
            }
            current = current->child[1];
        } else {
            if(current->child[0] == NULL){
                return last_match==NULL? NULL:last_match->data;
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

void trie::print_node(prefix_data* pd)
{
    if (pd != NULL) {
        cout << pd->prefix << "\n";
        cout << pd->nexthop << "\n";
    }
    else{
        cout<<"Not found\n";
    }
}

int main()
{
    trie t;
    ifstream prefix_file("prefix_rand_nexthop.txt");
    string entry; 
    srand ( time(NULL) );
    auto timer = Timer();
    while (getline(prefix_file, entry))
    {
        char* trie_entry[3];
        int i = 0;
        char * pch = strtok (strdup(entry.c_str()),"/ ");
        while (pch != NULL){
            trie_entry[i++] = pch;
            pch = strtok(NULL, "/ ");
        }

        t.insert(trie_entry[0], atoi(trie_entry[1]), trie_entry[2]);
    }
    auto duration = timer.elapsed();

    cout << "Trie building time: " << duration << endl;
    prefix_file.close();

    ifstream ip_file("MillionIPAddrOutput.txt");
    int total_ips = 0;
    while (getline(ip_file, entry))
    {
        // With hints
        int num_experiments = 5;
        int i = num_experiments;
        double total_time = 0;
        while (i > 0){
            int hint = rand() % 32 + 1; // hint in range 1 to 32
            timer = Timer();      
            prefix_data* pd = t.search(entry, hint);
            duration = timer.elapsed();
            total_time += duration;
            i -= 1;
            // t.print_node(pd);
        }

        cout << total_time/num_experiments;

        // Without hint
        i = num_experiments;
        total_time = 0;
        while (i > 0){
            timer = Timer();  
            t.search(entry, 0);
            duration = timer.elapsed();
            //cout << "duration:" << duration << endl;
            total_time += duration;
            i -= 1;
        }

        cout << "," << total_time/num_experiments << endl;
    }



 /*

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

 */
}
