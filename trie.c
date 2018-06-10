#include "trie.h"
#include "timer.h"
#include <string.h>
#include <fstream>

void runTests(trie& t);
void runExperiment(string ip, double arr[], const int hintLength,
                        trie& t, const int numSearches);

trie::trie()
{
	head=NULL;
    for(int i=0;i<32;i++){
      // lentable[i] = new unordered_map<int, node*>(1 << std::max(0, i-2));
	  lentable[i].max_load_factor(0.2);    
 }
 
	
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

    // cout << prefix << " " << value << endl;

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

    // cout << "Trie building time: " << duration/std::pow(10,6) <<" ms"<< endl;
    prefix_file.close();

    runTests(t);

    // ifstream ip_file("MillionIPAddrOutput.txt");
    // int total_ips = 0;
    // while (getline(ip_file, entry))
    // {
    //     // With hints
    //     int num_experiments = 100000;
    //     int repeat_experiments = 20;
    //     double total_time = 0;
    //     int i, j;

    //     timer = Timer();
    //     j = repeat_experiments;
    //     while (j-- > 0) {
    //         i = num_experiments;
    //         while (i-- > 0) {
    //             t.search(entry, j);
    //         }
    //     }
    //     total_time = timer.elapsed();
    //     cout << " OUR TIME: " << total_time/(num_experiments * repeat_experiments) << " ns"<<endl;

    //     // Without hint
    //     timer = Timer();
    //     j = repeat_experiments;
    //     while (j-- > 0) {
    //         i = num_experiments;
    //         while (i-- > 0) {
    //             t.search(entry, 0);
    //         }
    //     }
    //     total_time = timer.elapsed();
    //     cout << "BASE TIME: " << total_time/(num_experiments * repeat_experiments) << " ns"<<endl;
    // }



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


void runTests(trie& t) {

    // read in some addresses from this file
    ifstream ip_file("MillionIPAddrOutput.txt");
    string ip;

    // goal is to get the time it takes to find an ip address:
    //      1) successfully with hints
    //      2) successfully without hints
    //      3) unsuccessfully with hints
    //      4) unsuccessfully without hints

    // write everything to a csv for processing in python
    ofstream successResults;
    successResults.open("successResults_point2.csv");

    //ofstream failResults;
    //failResults.open("failResults.csv");

    successResults << "0_hint,1_hint,2_hint,3_hint,4_hint,5_hint,6_hint," <<
        "7_hint,8_hint,9_hint,10_hint,11_hint,12_hint,13_hint,14_hint,15_hint," <<
        "16_hint,17_hint,18_hint,19_hint,20_hint,21_hint,22_hint,23_hint,24_hint\n";

//    failResults << "0_hint,1_hint,2_hint,3_hint,4_hint,5_hint,6_hint," <<
  //      "7_hint,8_hint,9_hint,10_hint,11_hint,12_hint,13_hint,14_hint,15_hint," <<
    //    "16_hint,17_hint,18_hint,19_hint,20_hint,21_hint,22_hint,23_hint,24_hint\n";

    const int numSearches = 100000;

    int numSucceed = 500;
    int numFailed = 1000;

    while (getline(ip_file, ip)) {

        if (numSucceed == 0 && numFailed == 0) {
            break;
        }

        // element 0 of this array is the time it took
        // to search for an ip address in the trie 20,000,000
        // times with a hint length of 0 .... and so on for
        // all hint lengths up to 24

        // search isn't successful
        if (t.search(ip, 24) == nullptr && numFailed > 0) {
            numFailed--;
            //cout << "skipping fail num " << numFailed << endl;
            // double fail[25];

            // // run the experiment 20,000,000 times
            // // for each length hint (0 to 24)
            // for (int i = 0; i <= 24; ++i) {
            //     runExperiment(ip, fail, i,
            //         t, numSearches);
            // }

            // cout << "writing to " << numFailed << " failResults\n";
            // // write results to file
            // for (int i = 0; i <= 24; ++i) {
            //     failResults << fail[i];
            //     if (i != 24) {
            //         failResults << ',';
            //     }
            // }
            // failResults << '\n';
        }

        // search is successful
        else if (numSucceed > 0) {
            numSucceed--;
            double succeed[25];

            // run the experiment 20,000,000 times
            // for each length hint (0 to 24)
            for (int i = 0; i <= 24; ++i) {
                runExperiment(ip, succeed, i,
                    t, numSearches);
            }

            cout << "writing to " << numSucceed <<" successResults\n";
            // write results to file
            for (int i = 0; i <= 24; ++i) {
                successResults << succeed[i];
                if (i != 24) {
                    successResults << ',';
                }
            }
            successResults << '\n';
        }
    }

    ip_file.close();
    successResults.close();
   // failResults.close();
}

void runExperiment(string ip, double arr[], const int hintLength,
                        trie& t, const int numSearches) {
    // cout << "\trunning experiment on " << hintLength << endl;
    // start a timer, run the experiment 100,000 times
    // and set the value of the array corresponding to that
    // hint to the value you get
    Timer timer;
    for (int i = 0; i < numSearches; ++i) {
        t.search(ip, hintLength);
    }
    arr[hintLength] = timer.elapsed() / numSearches;
}
