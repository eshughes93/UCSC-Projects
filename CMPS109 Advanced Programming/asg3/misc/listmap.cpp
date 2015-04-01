#include "listmap.h"
#include "xless.h"
#include "xpair.h"

listmap<Key,Value,Less>::listmap() {
   head = tail = NULL;
}

iterator listmap<Key,Value,Less>::find (const key_type&) const {
   iterator itor;
   itor.map = this;
   itor.node = this->head;
   node cur = head;
   while(cur != NULL) {
      
      cur = cur->next;
   }
   return itor;
}


void listmap::insert (const value_type&) {

   node tmp;
   tmp->pair = value_type;

   node cur = head; 

   if(empty()) {
      head = tail = tmp;
      tmp->next = NULL;
      tmp->prev = NULL;
   }
 
   while(cur!=NULL) {

      if(less(value_type.first, cur.value_type.first)){
       
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
      cur->next = tmp;
      tmp->prev = cur;
      tail = tmp;
      tmp->next = NULL;   
   } 
   
}



iterator listmap::begin() {
   iterator begin(this, this->head);
   return begin;
}

iterator listmap::end() {
   iterator end(this, this->tail);
   return end;
}

