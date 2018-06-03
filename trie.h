# include <iostream>
# include <unordered_map>
# include <algorithm>
# include <string>
using namespace std;


struct prefix_data
    {
        string nexthop;
        string prefix;
    };
class trie
{

private:
	
   	struct node
	{
		prefix_data* data;
		node* child[2];

		node()
		{
			data = nullptr;
			child[0] = nullptr;
			child[1] = nullptr; 
		}
	};

	node* head;
	unordered_map<int, node*> lentable[32];
public:
	trie();
	~trie();
	void insert(string prefix, int mask,string nexthop);
	prefix_data* search(string prefix, int hint);
    void print_trie();
    void print_node(prefix_data* pd);

private: // helpers

	void printer(node* n,string sofar);
	unsigned int addr_to_int(string prefix);
	void pointer_deletion(node *n);

    
};