/*
Evan Hughes [eshughes@ucsc.edu]
CMPS109 Assignment 1
*/
// $Id: inode.cpp,v 1.4 2014-04-09 17:04:58-07 - - $

#include <cassert>
#include <iostream>

using namespace std;

#include "debug.h"
#include "inode.h"

int inode::next_inode_nr {1};

inode::inode(inode_t init_type):
   inode_nr (next_inode_nr++), type (init_type)
{
   switch (type) {
      case DIR_INODE:
           contents.dirents = new directory();
           break;
      case FILE_INODE:
           contents.data = new wordvec();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

void inode_state::init(inode* root_node){
   root = root_node;
   root->contents.dirents->emplace(".", root);
   root->contents.dirents->emplace("..", root);
   cwd = root;
}

void inode_state::change_prompt(const wordvec &words){
   string new_prompt;
   for(unsigned i = 1; i < words.size(); i++){
      new_prompt += words[i];
   }
   prompt = new_prompt;
}
string inode_state::get_prompt(){
   return prompt;
}
void inode_state::cat(const wordvec &words){
   inode *tmp = cwd;
   wordvec pathname = split(words[1], "/");
   string filename;
   if(pathname.size() == 1){
      filename = pathname[0];
      auto it = cwd->contents.dirents->find(filename);
      if(it == cwd->contents.dirents->end()){
         throw yshell_exn("The file " + filename + " does not exist.");
      } else if (it->second->type == FILE_INODE){
         cout << filename << ": " << endl;
         for(unsigned i=1; i<it->second->contents.data->size(); i++){
            //cout << it->second->contents.data->size() << endl;
            cout << it->second->contents.data[i] << " ";
         }
         cout << endl;
      } else {
         throw yshell_exn("\"" + filename + "\" is a directory.");
      }
   } else {
      filename = pathname.back();
      pathname.pop_back();
      this->cd_dir(pathname);
      auto it = cwd->contents.dirents->find(filename);
      if(it == cwd->contents.dirents->end()){
         throw yshell_exn("The file " + filename + " does not exist.");
      } else if (it->second->type == FILE_INODE){
         cout << filename << ": " << endl;
         for(unsigned i=1; i<it->second->contents.data->size(); i++){
            cout << it->second->contents.data[i] << " ";
         }
         cout << endl;
      } else {
         throw yshell_exn("\"" + filename + "\" is a directory.");
      }
   }
   cwd = tmp;
}
void inode_state::pwd(){
   wordvec pathname;
   inode *tmp = cwd;
   int prev_inode;
   string prev_dir;
   while(cwd != root){
      auto it = cwd->contents.dirents->find("..");
      prev_inode = cwd->get_inode_nr();
      cwd = it->second;
      auto itor = cwd->contents.dirents->begin();
      while(itor != cwd->contents.dirents->end()){
         if(prev_inode == itor->second->get_inode_nr()
               && itor->first != ".."){
            prev_dir = itor->first;
         }
         itor++;
      }
      pathname.push_back("/");
      pathname.push_back(prev_dir);

   }
   pathname.push_back("/");
   while(!pathname.empty()){
      cout << pathname.back();
      pathname.pop_back();
   }
   cout << endl;
   cwd = tmp;
}


void inode_state::cd(){
   cwd = root;
}
void inode_state::cd_dir(const wordvec &words){
   if(words.size() != 0){
      wordvec path;
      auto it = cwd->contents.dirents->find(words[0]);
      if(it == cwd->contents.dirents->end()){
         throw yshell_exn("No directory with name: " + words[0]);
      } else if( it->second->type != DIR_INODE){
         throw yshell_exn(words[0] + " is not a directory.");
      } else {
         cwd = it->second;
         for(unsigned i = 1; i < words.size(); i++){
            path.push_back(words[i]);
         }
         this->cd_dir(path);
      } 
   }
}
void inode_state::ls(const wordvec &words){
   if(words.size() == 1){
      auto it = cwd->contents.dirents->begin();
      while(it != cwd->contents.dirents->end()){
         cout << "  " << it->second->get_inode_nr() 
         << "  " << it->second->size();
         if(it->second->type == DIR_INODE){
            cout << "  " << it->first << "/" << endl;
         } else {
            cout << "  " << it->first << endl;
         }
         it++;
      }
   } else if(words.size() > 1){
      wordvec path = split(words[1], "/");
      for(unsigned i = 0; i < path.size(); i++){
         auto it = cwd->contents.dirents->find(path[i]);
         if(it == cwd->contents.dirents->end()){
            throw yshell_exn("No file or directory \"" 
                              + path[i] + "\"");
         } else {
            if(it->second->type == DIR_INODE){
               auto itor = it->second->contents.dirents->begin();
               while(itor != it->second->contents.dirents->end()){
                  cout << "  " << itor->second->get_inode_nr()
                  << "  " << itor->second->size();
                  if(itor->second->type == DIR_INODE){
                     cout << "  " << itor->first << "/" << endl;
                  } else {
                     cout << "  " << itor->first << endl;
                  }
                  itor++;
               }
            } else {
               cout << it->first << endl;
            }
         }
      }
   }
}

//
// copy ctor -
//    Make a copy of a given inode.  This should not be used in
//    your program if you can avoid it, since it is expensive.
//    Here, we can leverage operator=.
//
inode::inode (const inode& that) {
   *this = that;
}

//
// operator= -
//    Assignment operator.  Copy an inode.  Make a copy of a
//    given inode.  This should not be used in your program if
//    you can avoid it, since it is expensive.
//
inode& inode::operator= (const inode& that) {
   if (this != &that) {
      inode_nr = that.inode_nr;
      type = that.type;
      contents = that.contents;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
   return *this;
}


int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

int inode::size() const {
   int size {0};
   int tmp = 0;
   DEBUGF ('i', "size = " << size);
   if(type == DIR_INODE){
      size = contents.dirents->size();
   } else if (type == FILE_INODE){
      if(contents.data->size() == 0){
         size = 0;
      } else {
         tmp = contents.data->size() - 1;
         for(int i = 0; i <= tmp; i++){
            size+= contents.data->at(i).size();
         }
         size += tmp;
      }
   }
   return size;
}

const wordvec& inode::readfile() const {
   DEBUGF ('i', *contents.data);
   assert (type == FILE_INODE);
   return *contents.data;
}

void inode::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   assert (type == FILE_INODE);
   //writing over the file, hence deleting current contents
   contents.data->clear(); 
   for(unsigned i = 2; i < words.size(); i++){
      contents.data->push_back(words[i]); 
   }
}
void inode::ls_r(inode_state& state, const wordvec &words){
   auto it = contents.dirents->begin();
   for(;it != contents.dirents->end(); it++){
      cout << "  " << it->second->get_inode_nr()
      << "  " << it->second->size();
      if(it->second->type == DIR_INODE){
         cout << "  " << it->first << "/" << endl;
      } else {
         cout << "  " << it->first << endl;
      }
      if(it->second->type == DIR_INODE && 
         it->first != "." && it->first != ".."){
         it->second->ls_r(state, words);
      }
   }
}
void inode_state::lsr(inode_state& state, const wordvec &words){
   wordvec path;
   if(words.size() > 1){
      path = split(words[1], "/");
   }
   if(words.size() == 1){
      cwd->ls_r(state, words);
   } else {
      for(unsigned i = 0; i < path.size(); i++){
         auto it = cwd->contents.dirents->find(path[i]);
         if(it == cwd->contents.dirents->end()){
            throw yshell_exn("No file or directory \"" 
                              + path[i] + "\"");
         } else {
            if(it->second->type == DIR_INODE){
               it->second->ls_r(state, words);
            } else {
               cout << it->first << endl;
            }
         }
      }
   }
   
}
void inode::rm_r(inode_state& state, const wordvec& words, 
                                    const string& filename){
   auto it = contents.dirents->begin();
   for(; it != contents.dirents->end(); it++){
      if(it->second->type == DIR_INODE && 
         it->first != ".." && it->first != "."){
         it->second->rm_r(state, words, filename);
      }
      contents.dirents->erase(it);
      
   }
}
void inode_state::rmr(inode_state& state, const wordvec& words){
   inode *tmp = cwd;
   string filename;
   if(words.size() == 1){
      throw yshell_exn("Must specify a pathname.");
   } else if (words[1] == "/"){
      throw yshell_exn("Cannot rm root directory.");
   } else {
      wordvec path = split(words[1], "/");
      if(path.size() == 1){
         filename = path[0];
         auto it = cwd->contents.dirents->find(filename);
         if( it == cwd->contents.dirents->end()){
            throw yshell_exn("Invalid pathname, the specified filename\
                               or directory does not exist.");
         } else {
            if(it->second->type == DIR_INODE){
               cwd = it->second;
               cwd->rm_r(state, words, filename);
            } else {
               cwd->remove(filename);
            }
         }
      } else {
         filename = path.back();
         path.pop_back();
         this->cd_dir(path);
         auto it = cwd->contents.dirents->find(filename);
         if( it == cwd->contents.dirents->end()){
            throw yshell_exn("Invalid pathname, the specified filename\
                               or directory does not exist.");
         } else {
            if(it->second->type == DIR_INODE){
               cwd = it->second;
               cwd->rm_r(state, words, filename);
            } else {
               cwd->remove(filename);
            }
         }
      }
   }
   cwd = tmp;
}
void inode::remove (const string& filename) {
   DEBUGF ('i', filename);
   assert (type == DIR_INODE);
   auto it = contents.dirents->find(filename);
   if(it->second->type == DIR_INODE && it->second->size() > 2){
      throw yshell_exn("Cannot remove a non-empty directory.");
   } else {
      contents.dirents->erase(it);
   }
}

void inode_state::rm(const wordvec& words){
   inode *tmp = cwd;
   string filename;
   if(words.size() == 1){
      throw yshell_exn("Must specify a pathname.");
   } else if (words[1] == "/"){
      throw yshell_exn("Cannot rm root directory.");
   } else {
      wordvec path = split(words[1], "/");
      if(path.size() == 1){
         filename = path[0];
         auto it = cwd->contents.dirents->find(filename);
         if( it == cwd->contents.dirents->end()){
            throw yshell_exn("Invalid pathname, the specified filename\
                               or directory does not exist.");
         } else {
            cwd->remove(filename);
         }
      } else {
         filename = path.back();
         path.pop_back();
         this->cd_dir(path);
         auto it = cwd->contents.dirents->find(filename);
         if( it == cwd->contents.dirents->end()){
            throw yshell_exn("Invalid pathname, the specified filename\
                               or directory does not exist.");
         } else {
            cwd->remove(filename);
         }
      }
   }
   cwd = tmp;
}
void inode_state::mkdir(const wordvec& words){
   inode *tmp = cwd;
   string dirname;
   wordvec path = split(words[1], "/");
   if(path.size() == 1){
      dirname = path[0];
      cwd->mkdir(dirname);
   } else {
      dirname = path.back();
      path.pop_back();
      this->cd_dir(path);
      cwd->mkdir(dirname);
      cwd = tmp;
   }
}
inode& inode::mkdir (const string& dirname){
   inode *dir = new inode(DIR_INODE);
   auto dn = contents.dirents->find(dirname);
   if( dn != contents.dirents->end()){
      throw yshell_exn("Directory " + dirname + " already exists.");
   } else {
      contents.dirents->emplace(dirname,dir);
      dir->contents.dirents->emplace("..", this);
      dir->contents.dirents->emplace(".", dir);
   }
   //return dir;
}
void inode::write_file(const wordvec& words){
   assert(type == FILE_INODE);
   for(unsigned i = 2; i < words.size(); i++){
      contents.data->push_back(words[i]);
   }
}

void inode_state::mk(const wordvec& words){
   inode *file = new inode(FILE_INODE);
   inode *tmp = cwd;
   string filename;
   wordvec path = split(words[1], "/");
   if(path.size() == 1){
      filename = path[0];
      auto it = cwd->contents.dirents->find(filename);
      if(it != cwd->contents.dirents->end()){
         if(it->second->type == DIR_INODE){
            throw yshell_exn("A directory with the name "
                         + filename + " already exists.");
         }
      } else {
         cwd->contents.dirents->emplace(filename, file);
      }
   } else {
      filename = path.back();
      path.pop_back();
      this->cd_dir(path);
      auto it = cwd->contents.dirents->find(filename);
      if(it != cwd->contents.dirents->find(filename)){
         if(it->second->type == DIR_INODE){
            throw yshell_exn("A directory with the name " 
                           + filename + " already exists.");
         }
      } else {
         cwd->contents.dirents->emplace(filename, file);
      }
      cwd = tmp;
   }
   if(words.size() > 2){
      file->write_file(words);
   }
}

inode& inode::mkfile (const string& filename){
   auto it = contents.dirents->find(filename);
   if(it != contents.dirents->end()){
      //throw yshell_exn("Directory or file with name " 
      //                  + filename + " already exists.");
   } else {
      inode *file = new inode(FILE_INODE);
      contents.dirents->emplace(filename, file);
      return *file;
   }
}



inode_state::inode_state() {
   DEBUGF ('i', "root = " << (void*) root << ", cwd = " << (void*) cwd
          << ", prompt = " << prompt);
}

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

