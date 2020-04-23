
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>

struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

typedef hufftreenode* hufftreeptr;

//struct for sorting priority queue by ascending order of frequency
struct compare
 {
   bool operator()(const hufftreeptr& h1, const hufftreeptr& h2)
   {
       return h1->frequency > h2->frequency;
   }
 };



/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////

//utility function
bool valid_hufftree(const hufftreeptr hufftree);
 // returns true if the input hufftree is a valid Huffman tree

std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree);
// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding


// CODE FOR ADDIIONAL FUNCTIONS DECLARATION

/**
    Summary: updates table of character frequencies with message
    @param message The message to be encoded , freq_table The table of frequencies for each character in message
*/
void update_freq_table(const std::string& message, std::map<char,int>& freq_table);

/**
    Summary: populates priority_queue with all unique elements in the frequency table
    @param freq_table The table of frequencies for each character in message , pq The priority queue to be filled
*/
void populate_priority_queue(std::map<char,int>& freq_table,std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, compare>& pq);

/**
    Summary: Builds huffman tree by adjoining top two items of priority queue and thus emptying pq.
    @param pq The priority queue holding sorted hufftree node pointers, root The root node holding the root of newly built huffmantree
*/
void build_huffman_tree(std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, compare>& pq, hufftreeptr& root);

/**
    Summary: Checks if the priority queue has one node left
    @param pq The priority queue of hufftree node pointers
    @return Whether it is the last_node
*/
bool last_node(std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, compare>& pq);

/**
    Summary: Checks if node_ptr points to a leaf node
    @param node_ptr Node pointer in question
    @return Whether node_ptr points to a leaf node
*/
bool is_leaf(hufftreeptr node_ptr);

/*
    Summary: Updates hufftable, adding characters of freqtable as keys, and hufftree path ('code') as value
    @param hufftable Huffman table to be updated. freq_table Table of frequencies for each character in message.
*/
void update_hufftable(std::map<char,std::string>& hufftable, std::map<char,int>& freq_table, hufftreeptr root_ptr);

/*
    Summary: Recursively traverse all nodes of huffman tree with path. When leaf reached, path ("code") added to hufftable Value
    @param root_ptr Pointer to root node. str Holds tree path. hufftable Huffman table to be updated
*/
void find_code(hufftreeptr root_ptr, std::string str, std::map<char,std::string>& hufftable);

/*
    Summary: Uses hufftable to translate message into huffman code.
    @param message Message to be encoded. hufftable Huffman table.
    @return encoded message.
*/
std::string encode_message(std::string message, std::map<char,std::string>& hufftable);

/*
    Summary: Recursively checking a tree in order, by first checking left subtree, current node, then right subtree.
    @param t node in question. valid Boolean variable stating whether valid huffman tree.
*/
void checktree(hufftreeptr t, bool& valid);

/*
    Summary: Checks that the root of the subtree is valid
    @param t node in question. valid Boolean variable stating whether valid huffman tree.
*/
void checkroot(hufftreeptr t, bool& valid);

/////////////////////////////
// main function //
/////////////////////////////

int main() {
 std::string message;
	std::map<char, int> freqtable;
	std::map<char, std::string> hufftable;
	hufftreeptr hufftree = new hufftreenode;

	message = "BBB";
	std::string encoded_message = huffencode(message,freqtable,hufftree,hufftable);
	std::cout<<"Encoded message:"<<encoded_message<<std::endl;

	std::string answer = huffdecode(encoded_message, hufftree);
	std::cout<<"Decoded message:"<<answer<<std::endl;

	bool is_valid =  valid_hufftree(hufftree);

	if(is_valid){
		std::cout<<"valid hufftree."<<std::endl;
	}
	else{
		std::cout<<"invalid hufftree."<<std::endl;
	}
}


/////////////////////////////////////////////
// Functions definitions (implementations) //
/////////////////////////////////////////////

// Returns true if the input hufftree is a valid Huffman tree
bool valid_hufftree(const hufftreeptr hufftree){
	bool valid_check = true;
	checktree(hufftree, valid_check);
	return valid_check;
}

// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))
std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable){
	update_freq_table(message,freqtable);
	std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, compare> pq;
	populate_priority_queue(freqtable,pq);
	build_huffman_tree(pq,hufftree);
	update_hufftable(hufftable,freqtable,hufftree);
	std::string code = encode_message(message, hufftable);
	return code;
}

// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding
std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree){

	std::string decodedmessage = "";
	hufftreeptr node_temp = hufftree;

	if(encodedmessage== ""){
		for(int i = 0; i<(node_temp->frequency); i++){
			decodedmessage = decodedmessage + node_temp->character;
		}
	}
	else{
		for(int i = 0; i<encodedmessage.size(); i++){

				if(encodedmessage[i]=='0'){
					node_temp = node_temp->left;
				}
				if(encodedmessage[i]=='1'){
					node_temp = node_temp->right;
				}

				if(is_leaf(node_temp)){
					decodedmessage = decodedmessage + node_temp->character;
					node_temp = hufftree;
				}
		}
	}
	return decodedmessage;
}


//  updates table of character frequencies with message
void update_freq_table(const std::string& message, std::map<char,int>& freq_table){
  char ascii_character;
  for(int i=0; i<message.size();i++){
    ascii_character = message[i];
    if(freq_table.count(ascii_character)>0){
      freq_table.at(ascii_character)++;
    }
    else{ //if not unique,
      freq_table.emplace(ascii_character,1);
    }
  }
}

//populates priority_queue with all unique elements in the frequency table
void populate_priority_queue(std::map<char,int>& freq_table, std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, compare>& pq){
	for(auto elem : freq_table){
		hufftreeptr queue_nodeptr = new hufftreenode;
		queue_nodeptr->character = elem.first;
		queue_nodeptr->frequency = elem.second;
		queue_nodeptr->left = NULL;
		queue_nodeptr->right = NULL;
		pq.push(queue_nodeptr);
	}
}

// Builds huffman tree by adjoining top two items of priority queue and thus emptying pq.
void build_huffman_tree(std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, compare>& pq, hufftreeptr& root){
	hufftreeptr leftPtr = NULL, rightPtr = NULL;

	while(!last_node(pq)){
		hufftreeptr topPtr = new hufftreenode;
		leftPtr = pq.top();
		pq.pop();

		rightPtr = pq.top();
		pq.pop();

		topPtr->character = '\0';
		topPtr->frequency = (leftPtr->frequency)+(rightPtr->frequency);
		topPtr->left = leftPtr;
		topPtr->right = rightPtr;

		pq.push(topPtr);
	}
	root = pq.top();

	pq.pop();
}

// Checks if the priority queue has one node left
bool last_node(std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, compare>& pq){
	if(pq.size()==1){
		return true;
	}
	return false;
}

// Checks if node_ptr is leaf node
bool is_leaf(hufftreeptr node_ptr){
	if((node_ptr->left == NULL)&&(node_ptr->right==NULL)){
		return true;
	}
	return false;
}

//Updates hufftable, adding characters of freqtable as keys, and hufftree path ('code') as value
void update_hufftable(std::map<char,std::string>& hufftable, std::map<char,int>& freqtable, hufftreeptr root_ptr){
	std::string tree_path;
	std::string temp_path = "";
	char input;
	for(auto elem : freqtable){
		input = elem.first;
		hufftable.emplace(elem.first, tree_path);
	}
	find_code(root_ptr, tree_path, hufftable);
}

//Recursively traverse all nodes of huffman tree with path. When reach leaf, add code to table. Called within update_hufftable.
void find_code(hufftreeptr root_ptr, std::string str, std::map<char,std::string>& hufftable){
	if(root_ptr == NULL){
		return;
	}
	if(is_leaf(root_ptr)){
		hufftable[root_ptr->character] = str;
	}

	find_code(root_ptr->left, str + "0", hufftable);
	find_code(root_ptr->right, str + "1", hufftable);
}

//Uses hufftable to translate message into huffman code.
std::string encode_message(std::string message, std::map<char,std::string>& hufftable){
	std::string code = "";
	for(int i = 0;i<message.size();i++){
		code = code + hufftable[message[i]];
	}
	return code;
}

// checks that the root of the subtree is valid
void checkroot(hufftreeptr t, bool& valid){
	if(is_leaf(t)){
		if(t->character=='\0'){
			valid = false;
		}
	}
	else{
		if(t->left==NULL || t->right==NULL){
			valid = false;
		}
		if(t->frequency!=(((t->left)->frequency)+((t->right)->frequency))){
			valid = false;
		}
	}
}

// checking a tree in order, by first checking left subtree, current node, then right subtree.
void checktree(hufftreeptr t, bool& valid){
  if(t!=NULL){
    checktree(t->left, valid);
    checkroot(t,valid);
    checktree(t->right,valid);
  }
}
