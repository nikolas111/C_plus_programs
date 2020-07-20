#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
using namespace std;
#endif /* __PROGTEST__ */

struct Node{
    char data;
    struct Node * left, *right;
};
// Global variable for monitoring the bits
int i=-1;
unsigned char chunk;
int NewBit(ifstream &file) {
    int bit = 0;
// New chunk
    if (i == -1) {
        i = 7;
        chunk = file.get();
        if( file.fail() ){
	    // byte not complete
            return 404;
        }
    }
    // getting bit at i-th position
    bit = ((chunk >> i) & 1);
    --i;
    return bit;
}
void deleteTree(Node* node) {
    if (node == NULL) return;
    deleteTree(node->left);
    deleteTree(node->right);
    free(node);
}
int Error_Handling(ifstream & f1, ofstream &f2){

    if(!f1.is_open() || !f2.is_open()){
        return 0;
    }
    if(!f1.good() || !f2.good()){
        return 0;
    }
    if(f1.bad() || f2.bad()){
        return 0;
    }
    return 1;

}

int WriteIn(int bit, ifstream & file, Node * tmp, Node* tree, ofstream &output){

    while(bit){
	// one chunk has 4096 bits
        for(int r=0;r<4096;){
            bit = NewBit(file);
            if(bit == 404)return -1;
            if( bit)tmp=tmp->right;
            else tmp=tmp->left;
            if(tmp->left==NULL && tmp->right== NULL){
                output << tmp->data;
                tmp=tree;
                ++r;
            }
        }
        bit=NewBit(file);
    }
    int  NumOfChars=0;
    for (int  k=11;k>=0;--k){
        bit = NewBit(file);
        if(bit == 404)return -1;
        NumOfChars = (NumOfChars & ~(1 << k)) | (bit << k);
    }
    for(int r=0;r<NumOfChars;){
        bit = NewBit(file);
        if(bit == 404)return -1;
        if( bit)tmp=tmp->right;
        else tmp=tmp->left;
        if(tmp->left==NULL && tmp->right== NULL){
            output <<tmp->data;
            tmp=tree;
            ++r;
        }
    }
    return 0;
}

Node * CreateTree(Node * Tree,ifstream &file){
    unsigned char node_char = '\0';
    Node *node = new Node;
    int bit;
    node->left = NULL;
    node->right = NULL;
    node->data = NewBit(file);
    Tree = node;
    if (node->data == 0) {
        Tree->left = CreateTree(Tree->left, file);
        Tree->right = CreateTree(Tree->right, file);
    } else {
        for (int  k=7;k>=0;--k){
            bit = NewBit(file);
	    // setting bit at k-th position
            node_char = (node_char & ~(1 << k)) | (bit << k);
        }
        node->data=node_char;
        return Tree;
    }
    return Tree;
}


bool decompressFile ( const char * inFileName, const char * outFileName )
{
    Node * Tree=NULL;
    int bit=0;
    int error_bit=0;
    std::ofstream outWriteFile;
    std::ifstream file;

    outWriteFile.open(outFileName,std::ios::out|std::ios::binary);
    file.open(inFileName,std::ios::binary|std::ios::in);
    int true_false = Error_Handling(file,outWriteFile);

    if(!true_false){
	file.close();
        outWriteFile.close();
	i=-1;
	return false;
    }

    Tree = CreateTree (Tree, file);
    bit = NewBit(file);
    Node * tmp = Tree;
    error_bit = WriteIn(bit,file,tmp,Tree,outWriteFile);
    
    if(error_bit ==-1){
	file.close();
        outWriteFile.close();
	deleteTree(Tree);
	i=-1;
	return false;
	}
    i=-1;
    
    deleteTree(Tree);
    file.close();
    outWriteFile.close();
    // error handling if the contents was written
    if(file.fail() || outWriteFile.fail()){
        return false;
    }
    return true;
}

bool compressFile ( const char * inFileName, const char * outFileName )
{
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}
#ifndef __PROGTEST__
bool identicalFiles ( const char * fileName1, const char * fileName2 )
{
    // todo
    std::ifstream f1(fileName1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(fileName2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; //file problem
    }

    if (f1.tellg() != f2.tellg()) {
        return false; //size mismatch
    }

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
    //return false; // Given from progtest
}

int main ( void )
{
    

    return 0;
}
#endif /* __PROGTEST__ */


