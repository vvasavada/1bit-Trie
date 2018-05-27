# include <iostream>
# include <unordered_map>
# include <algorithm>
# include <string>
using namespace std;



class trie
{

private:
	struct prefix_data
    {
        string nexthop;
        string prefix;
    };

   	struct node
	{
		prefix_data* data;
		node* child[1];

		node()
		{
			data = NULL;
			child[0]=NULL;
			child[1]=NULL; 
		}
	};

	node* head;
	unordered_map<int, node*> lentable[32]; 

public:
	trie();
	~trie();
	void insert(string prefix, int mask,string nexthop);
	node* search(string prefix, int hint);
    void print_trie();
    void print_node(node* n);

private: // helpers

	void printer(node* n,string sofar);
	unsigned int addr_to_int(string prefix);
	void pointer_deletion(node *n);

    
};