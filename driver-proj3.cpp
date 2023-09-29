/**
* file: driver-proj3.cpp
* author: Michael Mathews
* course: CSI 3334
* assignment: project 3
* due date: March 4, 2022
* version: 11.7
*
* This file uses user commands to fill and use an encryptable and decryptable BST codebook
*/

#include <iostream>
#include <sstream>
#include "bst-student-proj3.h"

/* main
*  parameters:
*      argc -- the number of arguments from the command line
*      argv -- the command line argument values
*  return value: 0 (indicating a successful run)
*
* This function uses user commands to fill and use an encryptable and
* decryptable BST codebook
*/
int main(int argc, const char * argv[]) {
    char command;
    string line, word;
    bool done = false;
    stringstream ss;
    EncryptionTree<string> tree;
    const int WS_QUOTE = 2;

    while(!done){
        cin >> command;
        //i x — insert cleartext word x into the codebook
        if(command == 'i'){
            cin >> word;
            tree.insert(word);
        }
        //r x — remove cleartext word x from the codebook.
        else if(command == 'r'){
            cin >> word;
            tree.remove(word);
        }
        //e ’cleartext message’ — encrypt the given cleartext message.
        else if(command == 'e'){
            //ignore first space
            cin.ignore(WS_QUOTE);
            getline(cin, line, '\'');
            //use stringstream to process message
            //ignore first quote
            ss.clear();
            ss.str(line);
            
            //do first encryption
            ss >> word;
            cout << tree.encrypt(word);
            //keep encrypting until you reach the end of the line
            while(ss.good()){
                ss >> word;
                cout << " " << tree.encrypt(word);
            }
            cout << endl;
        }
        //d ’encrypted message’ — decrypt the given encrypted message.
        else if(command == 'd'){
            //ignore first space
            cin.ignore(WS_QUOTE);
            getline(cin, line, '\'');
            //use stringstream to process message
            ss.clear();
            ss.str(line);
            //keep decrypting until you reach the end of the line
            
            //do first decryption
            ss >> word;
            if(tree.decrypt(word)){
                cout << *tree.decrypt(word);
            }
            else {
                cout << "?";
            }
            //finish encryption
            while(ss.good()){
                ss >> word;
                //if word exists in codebook
                if(tree.decrypt(word)){
                    cout << " " << *tree.decrypt(word);
                }
                else {
                    cout << " ?";
                }
            }
            cout << endl;
        }
        //p — print the codebook in preorder format
        else if(command == 'p'){
            tree.printPreorder();
        }
        //q — quit the program (stop processing input)
        else if(command == 'q'){
            done = true;
        }
    }

    return 0;
}
