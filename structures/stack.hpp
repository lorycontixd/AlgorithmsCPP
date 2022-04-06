#ifndef __STACK__HPP
#define __STACK__HPP
#include <iostream>
#include <string>     // std::string, std::to_string
#include <cstdlib>
#include <exception>
using namespace std;

#define STACK_MAX_SIZE 1000 // Maximum size of stack

namespace Stack{
    class StaticStack{
    private:
        int top;
        int a[STACK_MAX_SIZE];

    public:
        StaticStack(){
            top = -1;
        }

        void Push(int x){
            if (top >= STACK_MAX_SIZE-1){
                cerr << "Stack overflow!" << endl;
                exit(EXIT_FAILURE);
            }else{
                a[++top] = x;
            }
        }

        int Pop(){
            if (IsEmpty()){
                cerr << "Cannot perform pop on empty stack" << endl;
                exit(EXIT_FAILURE);
            }
            int x = a[top--];
            return x;
        }

        int Peek()
        {
            if (IsEmpty()) {
                cout << "Cannot perform peek on empty stack" << endl;
                return 0;
            }
            else {
                int x = a[top];
                return x;
            }
        }

        bool IsEmpty()
        {
            return (top < 0);
        }

        int Search(int x){
            if (IsEmpty()){
                cerr << "Cannot perform search on empty stack." << endl;
                exit(EXIT_FAILURE);
            }else{
                for (int i=top; i>=0; i--){
                    if (a[i] == x){
                        return top-i;
                    }
                }
                cerr << "Didn't find the searched key: " << x << endl;
                exit(EXIT_FAILURE);
            }
        }

        void Scan(){
            if (IsEmpty()){
                cerr << "Cannot perform search on empty stack." << endl;
            }else{
                string s = "StaticStack: ";
                for (int i=top; i>=0; i--){
                    s = s + to_string(a[i]) + ", ";
                }
                s.erase(s.length()-2);
                cout << s << endl;
            }
        }

        void Sort(){
            if (IsEmpty()){
                cerr << "Cannot perform sort on empty stack." << endl;
            }else{
                
            }
        }
    };



    class Node {
        public:
            int data;
            Node* next;
    };
    
    Node* newNode(int data)
    {
        Node* stackNode = new Node();
        stackNode->data = data;
        stackNode->next = NULL;
        return stackNode;
    }
    
    int isEmpty(Node* root)
    {
        return !root;
    }
    
    void push(Node** root, int data)
    {
        Node* stackNode = newNode(data);
        stackNode->next = *root;
        *root = stackNode;
    }
    
    int pop(Node** root)
    {
        if (isEmpty(*root))
            cerr << "Cannot pop element from empty stack" << endl;
            exit(EXIT_FAILURE);
        Node* temp = *root;
        *root = (*root)->next;
        int popped = temp->data;
        free(temp);
    
        return popped;
    }
    
    int peek(Node* root)
    {
        if (isEmpty(root))
            cerr << "Cannot pop element from empty stack" << endl;
            exit(EXIT_FAILURE);
        return root->data;
    }

    void Scan(Node* root){
        while(!isEmpty(root))
        {
            // print top element in stack
            cout<<Stack::peek(root)<<" ";
            // remove top element in stack
            Stack::pop(&root);
        }
    }
}




#endif