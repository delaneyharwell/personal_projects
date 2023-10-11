#include "hashtable.h"
#include "hasher.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
using namespace std;

ListNode * ListNode::find(string key, ListNode * L){
    ListNode * temp = L;
    while (temp != nullptr){
        if (temp->data == key)
            return temp;
        temp = temp->next;
    }
    return nullptr;
}

ListNode * ListNode::insert(string key, ListNode * L){
    ListNode * newp = new ListNode(key, L);
    return newp;
}

ListNode * ListNode::remove(string key, ListNode * L){
    ListNode * current = L;
    ListNode * previous =  nullptr;
    while (current != nullptr){
        if (current->data == key){
            if (previous == nullptr)
                L = current->next;
            else
                previous->next = current->next;
            delete current;
            return L;
        }
        previous = current;
        current = current->next;
    }
    return L;
}

void ListNode::print(ostream & out, ListNode * L){
    for(ListNode * temp = L; temp != nullptr; temp = temp->next){
        out<<temp->data<<" ";
    }
}

int ListNode::length(ListNode * L){
    int count = 0;
    ListNode * current = L;
    while (current != nullptr){
        count++;
        current = current->next;
    }
   
    return count;
}

void ListNode::delete_list(ListNode * L){
    while ( L != nullptr){
        ListNode * temp = L;
        L = L->next;
        delete temp;
    }
}

//HashTable
void HashTable::insert(const string & word){
    int h = hasher.hash(word, capacity);
    buf[h] = ListNode::insert(word, buf[h]);
}

bool HashTable::find(const string & word){
    int index = hasher.hash(word, capacity);
    ListNode * node = ListNode::find(word, buf[index]);
    return node != nullptr;
}

void HashTable::remove(const string & word){
    int index = hasher.hash(word, capacity);
    buf[index] = ListNode::remove(word, buf[index]);
}

bool HashTable::is_empty(){
    for (size_t i = 0; i < capacity; ++i){
        if (buf[i] != nullptr)
            return false;
    }
    return true;
}

bool HashTable::is_full(){
    return false;
}

void HashTable::print(ostream & out){
    for (size_t i = 0; i < capacity; ++i){
        ListNode::print(out, buf[i]);
    }
}

HashTable::~HashTable(){
    for (size_t i = 0; i < capacity; ++i){
        if (buf[i] != nullptr)
            ListNode::delete_list(buf[i]);
    }
}
// HashTable & HashTable::operator = (const HashTable & L){
//     return buf == L->buf;
// }
size_t HashTable::number_of_entries(){
    size_t count = 0;
    for (size_t i = 0; i < capacity; ++i){
        ListNode * node = buf[i];
        while ( node != nullptr){
            ++count;
            node = node->next;
        }
    }
    return count;
}

size_t HashTable::number_of_chains(){
    return capacity;
}

void HashTable::get_chain_lengths(vector<int> & v){
    v.clear();
    for( size_t i = 0; i < capacity; ++i)
        v.push_back(ListNode::length(buf[i]));
}

//testing functions
void error(string word, string msg){
    cout<<word<<" "<<msg<<endl;
}

void insert_all_words(string file_name, HashTable & L){
    Timer t;
    double eTime;
    ifstream in(file_name);
    int limit = NWORDS / 45;
    t.start();
    for ( string word; (in >> word) ; --limit)
        L.insert(word);
    t.elapsedUserTime(eTime);
    in.close();
    cout<<"\t\tR = "<<eTime<<endl;
}



void find_all_words(string file_name, HashTable & L){
    Timer t;
    double eTime;
    ifstream in(file_name);
    int limit = NWORDS / 45;
    t.start();
    for ( string word; (in >> word) && limit >0; --limit)
        L.find(word);
    t.elapsedUserTime(eTime);
    in.close();
    cout<<"\t\tR = "<<eTime<<endl;
}

void remove_all_words(string file_name, HashTable & L){
    Timer t;
    double eTime;
    ifstream in(file_name);
    int limit = NWORDS / 45;
    t.start();
    for ( string word; (in >> word); --limit)
        L.remove(word);
    t.elapsedUserTime(eTime);
    in.close();
    cout<<"\t\tR = "<<eTime<<endl;
}

void measure_hashtable(string file_name, HashTable & L){
    //cout<<L.name<<endl;
    cout << L.get_name() << endl;
    insert_all_words(file_name, L);
    vector<int> chain_lengths;
    L.get_chain_lengths(chain_lengths);
    Stats stats(L.get_name(), chain_lengths);
    stats.print(cout);
    find_all_words(file_name, L);
    remove_all_words(file_name, L);
    if(!L.is_empty())
        error(L.get_name(), "is not empty");
   
}

void measure_hashtables(string input_file){
    Hasher * H[] = {
        new ChatGPTHasher{},
        new GoodrichHasher{},
        new MultHasher{},
        new PreissHasher{},
        new PJWHasher{},
        new STLHasher{},
        new SumHasher{},
        new Weiss1Hasher{},
        new Weiss2Hasher{},
        new WeissHasher{},
        new MyHasher{}
    };
    int S[] = {
        //10000,
        //1000,
        100,
        //10,
        //1
    };
    for (auto size : S)
        for (auto h : H){
            HashTable ht(*h, size);
            measure_hashtable(input_file, ht);
        }

}

