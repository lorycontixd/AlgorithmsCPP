/**
 * @file btree.hpp
 * @author Lorenzo Conti (lorenzo.conti3@studenti.unimi.it)
 * @brief Implementation of the B-Tree data structure.
 * @version 0.1
 * @date 2022-03-25
 * 
 * @AlgorithmsCPP Copyright (c) 2022 Lorenzo Conti
 * 
 */


#ifndef __BTREE_HPP__
#define __BTREE_HPP__
#include <vector>
#include <iostream>
using namespace std;

namespace BTree{
    template <class T> class Node{
        private:
            unsigned rank;
            unsigned n;
            T* keys;
            Node** pointers;

            bool leaf;
        public:
            Node(int _rank, bool isleaf = false){
                rank = _rank;
                keys = new T[2 * rank - 1];
                pointers = new Node *[2 * rank];
                leaf = isleaf;
                n = 0;
            }
            

            /**
             * @brief Insert new data value in the Node when it is not full.
             * 
             * @param node Starting node where the value should be inserted to.
             * @param value Value to be inserted in the node
             */
            void InsertNonFull(T k){
                unsigned i = n-1;
                if (leaf){
                    while (i>=1 && k<keys[i]){
                        keys[i+1] = keys[i];
                        i--;
                    }
                    keys[i+1] = k;
                    n++;
                    // Disk_write
                }else{
                    while (i>=1 && k<keys[i]){
                        i--;
                    }
                    i++;
                    // Disk_read
                    if (pointers[i]->n == 2*rank - 1) // Full child node
                    {
                        SplitChild(i, pointers[i]);
                        if (k>keys[i])
                        {
                            i++;
                        }
                    }
                    pointers[i]->InsertNonFull(k);
                }
            }

            /**
             * @brief Split a child node of the current node at index i into 2 children nodes,
             * that still belong to the parent node.
             * 
             * @param i Position index where the new
             * @param y 
             */
            void SplitChild(int i, Node *y){
                Node<T> *z = new Node(y->rank, y->leaf);
                z->n = rank - 1;
                for (unsigned j=0; j<rank-1; j++) // O(rank)
                {
                    z->keys[j] = y->keys[rank+j];
                }
                if (!y->leaf){
                    for (unsigned j=1; j<rank; j++) //O(rank)
                    {
                        z->pointers[j] = y->pointers[rank+j];
                    }
                }
                y->n = rank - 1;
                for (unsigned j=n+1; j>i+1; j--){
                    pointers[j+1] = pointers[j];
                }
                pointers[i+1] = z;
                for (unsigned j=n; j>i; j--){
                    keys[j+1] = keys[j];
                }
                keys[i] = keys[rank];
                n++;
            }

            void Scan(){
                int i;
                for (i = 0; i < n; i++) {
                    if (leaf == false)
                    pointers[i]->Scan();
                    cout << " " << keys[i];
                }

                if (!leaf)
                    pointers[i]->Scan();
            }

            Node<T>* Search(T k){
                unsigned i = 0;
                while (i < n && k > keys[i])
                    i++;

                if (keys[i] == k)
                    return this;

                if (leaf == true)
                    return nullptr;

                return pointers[i]->Search(k);
            }

            T FindKey(T k){
                int idx = 0;
                while (idx < n && keys[idx] < k)
                    ++idx;
                return idx;
            }

            void Delete(T k){
                T idx = FindKey(k);
                if (idx < n && keys[idx] == k) {
                    if (leaf)
                        RemoveFromLeaf(idx);
                    else
                        RemoveFromNonLeaf(idx);
                } else {
                    if (leaf) {
                        cerr << "The key " << k << " does not exist in the tree\n";
                        return;
                    }
                    bool flag = ((idx == n) ? true : false);
                    if (pointers[idx]->n < rank)
                    Fill(idx);

                    if (flag && idx > n)
                        pointers[idx - 1]->Delete(k);
                    else
                        pointers[idx]->Delete(k);
                }
                return ;
            }

            // Remove from the leaf
            void RemoveFromLeaf(int idx) {
                for (int i = idx + 1; i < n; ++i)
                    keys[i - 1] = keys[i];

                n--;
                return;
            }

            // Delete from non leaf node
            void RemoveFromNonLeaf(int idx) {
                T k = keys[idx];

                if (pointers[idx]->n >= rank) {
                    int pred = GetPredecessor(idx);
                    keys[idx] = pred;
                    pointers[idx]->Delete(pred);
                }

                else if (pointers[idx + 1]->n >= rank) {
                    int succ = GetSuccessor(idx);
                    keys[idx] = succ;
                    pointers[idx + 1]->Delete(succ);
                }

                else {
                    Merge(idx);
                    pointers[idx]->Delete(k);
                }
                return;
            }

            T GetPredecessor(int idx) {
                Node *cur = pointers[idx];
                while (!cur->leaf)
                    cur = cur->pointers[cur->n];

                return cur->keys[cur->n - 1];
            }

            T GetSuccessor(int idx) {
                Node *cur = pointers[idx + 1];
                while (!cur->leaf)
                    cur = cur->pointers[0];

                return cur->keys[0];
            }

            // Borrow from previous
            void BorrowFromPrev(int idx) {
                Node<T> *child = pointers[idx];
                Node<T> *sibling = pointers[idx - 1];

                for (int i = child->n - 1; i >= 0; --i)
                    child->keys[i + 1] = child->keys[i];

                if (!child->leaf) {
                    for (int i = child->n; i >= 0; --i)
                    child->pointers[i + 1] = child->pointers[i];
                }

                child->keys[0] = keys[idx - 1];

                if (!child->leaf)
                    child->pointers[0] = sibling->pointers[sibling->n];

                keys[idx - 1] = sibling->keys[sibling->n - 1];

                child->n += 1;
                sibling->n -= 1;

                return;
            }

            // Borrow from the next
            void BorrowFromNext(int idx) {
                Node<T> *child = pointers[idx];
                Node<T> *sibling = pointers[idx + 1];

                child->keys[(child->n)] = keys[idx];

                if (!(child->leaf))
                    child->pointers[(child->n) + 1] = sibling->pointers[0];

                keys[idx] = sibling->keys[0];

                for (int i = 1; i < sibling->n; ++i)
                    sibling->keys[i - 1] = sibling->keys[i];

                if (!sibling->leaf) {
                    for (int i = 1; i <= sibling->n; ++i)
                    sibling->pointers[i - 1] = sibling->pointers[i];
                }

                child->n += 1;
                sibling->n -= 1;

                return;
            }

            void Fill(int idx) {
                if (idx != 0 && pointers[idx - 1]->n >= rank)
                    BorrowFromPrev(idx);

                else if (idx != n && pointers[idx + 1]->n >= rank)
                    BorrowFromNext(idx);

                else {
                    if (idx != n)
                    Merge(idx);
                    else
                    Merge(idx - 1);
                }
                return;
            }

            // Merge
            void Merge(int idx) {
                Node<T> *child = pointers[idx];
                Node<T> *sibling = pointers[idx + 1];

                child->keys[rank - 1] = keys[idx];

                for (int i = 0; i < sibling->n; ++i)
                    child->keys[i + rank] = sibling->keys[i];

                if (!child->leaf) {
                    for (int i = 0; i <= sibling->n; ++i)
                    child->pointers[i + rank] = sibling->pointers[i];
                }

                for (int i = idx + 1; i < n; ++i)
                    keys[i - 1] = keys[i];

                for (int i = idx + 2; i <= n; ++i)
                    pointers[i - 1] = pointers[i];

                child->n += sibling->n + 1;
                n--;

                delete (sibling);
                return;
            }

            template<class> friend class Tree;
    };


    template <class T> class Tree{
        protected:
            Node<T>* root;
            unsigned rank;
        public:
            Tree(unsigned _rank){
                rank = _rank;
                root = nullptr;
            }

            void Insert(T k){
                if (root == nullptr){
                    root = new Node<T>(rank, true);
                    root->keys[0] = k;
                    root->n = 1;
                }else{
                    if (root->n == 2*rank+1) // Root node is full
                    {
                        Node<T> *s = new Node<T>(rank, false); // s will be the new root
                        s->pointers[0] = root; // Setting old root as first child of the new root
                        s->SplitChild(0, root);
                        
                        unsigned i = 0;
                        if (s->keys[0] < k){
                            i++;
                        }
                        s->pointers[i]->InsertNonFull(k);
                        root = s;
                    }else // Root node is not full
                    {
                        root->InsertNonFull(k);
                    }
                }
            }

            void Scan(){
                if (root != nullptr){
                    root->Scan();
                }
                cout << endl;
            }

            Node<T>* Search(T k){
                return (root == nullptr) ? nullptr : root->Search(k);
            }

            // Deleting a key from a B-tree
            void Delete(int k) {
                if (!root) {
                    cout << "The tree is empty\n";
                    return;
                }

                root->Delete(k);

                if (root->n == 0) {
                    Node<T> *tmp = root;
                    if (root->leaf)
                        root = NULL;
                    else
                        root = root->pointers[0];

                    delete tmp;
                }
                return;
            }

    };
}
    

#endif