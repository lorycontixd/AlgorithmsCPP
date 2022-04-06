#include "../structures/stack.hpp"
#include <iostream>
using namespace std;

int main(){
    // Static stack
    Stack::StaticStack s;
    s.Push(10);
    s.Push(20);
    s.Push(15);
    s.Push(30);

    s.Scan();
    cout << "Search using static stack: " << s.Search(10) << endl;

    // Dynamic stack
    Stack::Node* root = NULL;
 
    Stack::push(&root, 10);
    Stack::push(&root, 20);
    Stack::push(&root, 30);
 
    cout << Stack::pop(&root) << " popped from stack\n";
 
    cout << "Top element is " << Stack::peek(root) << endl;
     
    cout<<"Elements present in dynamic stack : ";
    Stack::Scan(root);
    return 0;

}