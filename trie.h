# include <iostream>
# include <vector>
# include <algorithm>
# include <string>
# include <cmath>
using namespace std;



class trie
{
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
	void printer(node* n,string sofar);
	public:
		trie();
		~trie();
		void insert(string prefix, int mask,string nexthop);
        void print_trie();
};