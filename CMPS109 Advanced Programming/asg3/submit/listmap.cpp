/*
Evan Hughes [eshughes@ucsc.edu]
Hadley BLack [hablack@ucsc.edu]
Assignment 3 Listmap-Templates
Due May 12 2014

listmap.cpp
*/
// $Id: listmap.tcc,v 1.1 2014-04-24 18:02:55-07 - - $

#include "listmap.h"
#include "trace.h"

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (const value_type& pair):
            pair(pair), prev(NULL), next(NULL) {
}

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::listmap (): head(NULL), tail (NULL) {
}

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap () {
   TRACE ('l', (void*) this);
}

//call near the end of program execution to clear memory leak
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::freelist(){
   while(head != NULL){
      node *cur = head;
      head = head->next;
      delete cur;
   }
}

//inserts into list in appropriate spot
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::insert (const xpair<Key,Value>& pair) {
   TRACE ('l', &pair << "->" << pair);
   node *tmp = new node(pair);
   node *cur = head; 
   if(empty()){
      head = tmp;
      tail = tmp;
      tmp->next = NULL;
      tmp->prev = NULL;
   } else {
       while(cur!=NULL) {
         if(pair.first == cur->pair.first) {
            cur->pair.second = pair.second;
            break;
         } else if(less(pair.first, cur->pair.first)){
            if(cur == head) {
               tmp->next = cur;
               cur->prev = tmp;
               head = tmp;
               tmp->prev = NULL;
            } else {
               tmp->next = cur;
               tmp->prev = cur->prev;
               cur->prev->next = tmp;
               cur->prev = tmp; 
            }
          break;  
       }
      cur = cur->next;
    }

    if(cur == NULL) {
         tail->next = tmp;
         tmp->prev = tail;
         tail = tmp;
         tmp->next = NULL;   
      }

   }

   //delete tmp;
}

//prints contents of a list
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::print () {

   node *cur = head;
 
   if(cur == NULL) {
      cout << "List is empty. " << endl;
   } 

   while(cur != NULL) {
      cout << cur->pair << endl;
      cur = cur->next;
   }
}

//prints all keys with a given value
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::print_values 
             (const xpair<Key,Value>& pair) {

   node *cur = head;
 
   if(cur == NULL) {
      cout << "List is empty. " << endl;
   } 

   while(cur != NULL) {
     if(pair.second == cur->pair.second) {
        cout << cur->pair << endl;
     }
     cur = cur->next;
   }
}

//delete specific key
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::remove (const xpair<Key,Value>& pair) {
   iterator itor;
   node *cur = head;
   while(cur != NULL){
      if(cur->pair.first == pair.first){
         iterator itor(this,cur);
         itor.erase();
         break;
      }
      cur = cur->next;
   }
}

//finds a key in list
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   TRACE ('l', that);
   
   node *cur = head;
   while(cur != NULL){
      if(cur->pair.first == that){
         cout << cur->pair << endl;
         return iterator(this,cur);
      }
      cur = cur->next;
   }
   cout << that << ": key not found" << endl;
   return end();
}


template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::begin () {
   return iterator (this, head);
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::end () {
   return iterator (this, NULL);
}

template <typename Key, typename Value, class Less>
bool listmap<Key,Value,Less>::empty () const {
   return head == NULL;
}


template <typename Key, typename Value, class Less>
xpair<Key,Value>& listmap<Key,Value,Less>::iterator::operator* () {
   TRACE ('l', where->pair);
   return where->pair;
}

template <typename Key, typename Value, class Less>
xpair<Key,Value> *listmap<Key,Value,Less>::iterator::operator-> () {
   TRACE ('l', where->pair);
   return &(where->pair);
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++ () {
   TRACE ('l', "First: " << map << ", " << where);
   TRACE ('l', "Second: " << map->head << ", " << map->tail);
   where = where->next;
   return *this;
}

template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator-- () {
   where = where->prev;
   return *this;
}

template <typename Key, typename Value, class Less>
bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::iterator::iterator (listmap *map,
            node *where): map (map), where (where){
}

//deletes key value pair at given iterator location
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::iterator::erase () {
   TRACE ('l', "map = " << map << ", where = " << where << endl);

   if(where == map->head && where == map->tail) {
      node *tmp = where;
      map->head = map->tail = NULL;
      delete tmp;

   } else if(where == map->head) {      
      node *tmp = map->head;
      map->head->next->prev = NULL;
      map->head = map->head->next;
      delete tmp;

   } else if(where == map->tail) {
      node *tmp = map->tail;
      map->tail->prev->next = NULL;
      map->tail = map->tail->prev;
      delete tmp;

   } else {
      node *tmp = where;
      where->next->prev = where->prev;
      where->prev->next = where->next;
      delete tmp;
   }

}

#include "listmap.tcc"
