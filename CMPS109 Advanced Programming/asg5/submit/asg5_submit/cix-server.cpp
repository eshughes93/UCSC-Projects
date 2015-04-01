/*
Hadley Black [hablack@ucsc.edu]
Evan HUghes [eshughes@ucsc.edu]
CMPS 109 Assignment 5 Client-Server SFTP
Due June 6th 2014
*/
// $Id: cix-server.cpp,v 1.2 2014-05-27 17:16:42-07 - - $

#include <fstream>
#include <iostream>
using namespace std;

#include <libgen.h>

#include "cix_protocol.h"
#include "logstream.h"
#include "signal_action.h"
#include "sockets.h"

logstream log (cout);

//server's reply to client put command
void reply_put (accepted_socket& client_sock, cix_header& header) {

   header.cix_command = CIX_ACK;
   memset (header.cix_filename, 0, CIX_FILENAME_SIZE);
   send_packet(client_sock, &header, sizeof header);

   char buffer[header.cix_nbytes];
   recv_packet(client_sock,buffer,header.cix_nbytes);
   log << "received payload: " << buffer << endl;
   ofstream file(header.cix_filename);
   file.write(buffer, header.cix_nbytes);
   
   
}

//server's reply to client rm command
void reply_rm(accepted_socket& client_sock, cix_header& header){
   int check = unlink(header.cix_filename);
   if(check != 0){
      header.cix_command = CIX_NAK;
      header.cix_nbytes = errno;
      send_packet (client_sock, &header, sizeof header);
   } else {
      header.cix_command = CIX_ACK;
      memset (header.cix_filename, 0, CIX_FILENAME_SIZE);
      send_packet (client_sock, &header, sizeof header);
   }
}
//server's reply to client get command
void reply_get(accepted_socket& client_sock, cix_header& header){

   ifstream file(header.cix_filename);
   if(file == NULL){
      header.cix_command = CIX_NAK;
      header.cix_nbytes = errno;
      send_packet (client_sock, &header, sizeof header);
   } else {
      header.cix_command = CIX_FILE;
      memset (header.cix_filename, 0, CIX_FILENAME_SIZE);
      file.seekg(0, file.end);
      header.cix_nbytes = file.tellg();
      file.seekg(0, file.beg); 

      log << "sending header " << header << endl;
      send_packet (client_sock, &header, sizeof header);

      char buffer[header.cix_nbytes];
      file.read(buffer,header.cix_nbytes);
      //buffer[header.cix_nbytes] = '\0';
      cout << buffer << endl;
      send_packet(client_sock, buffer, header.cix_nbytes);
   }
}

//server's reply to client ls command
void reply_ls (accepted_socket& client_sock, cix_header& header) {
   FILE* ls_pipe = popen ("ls -l", "r");
   if (ls_pipe == NULL) {
      log << "ls -l: popen failed: " << strerror (errno) << endl;
      header.cix_command = CIX_NAK;
      header.cix_nbytes = errno;
      send_packet (client_sock, &header, sizeof header);
   }
   string ls_output;
   char buffer[0x1000];
   for (;;) {
      char* rc = fgets (buffer, sizeof buffer, ls_pipe);
      if (rc == nullptr) break;
      ls_output.append (buffer);
   }
   header.cix_command = CIX_LSOUT;
   header.cix_nbytes = ls_output.size();
   memset (header.cix_filename, 0, CIX_FILENAME_SIZE);
   log << "sending header " << header << endl;
   send_packet (client_sock, &header, sizeof header);
   send_packet (client_sock, ls_output.c_str(), ls_output.size());
   log << "sent " << ls_output.size() << " bytes" << endl;
}


void signal_handler (int signal) {
   log << "signal_handler: caught " << strsignal (signal) << endl;
   switch (signal) {
      case SIGINT: case SIGTERM: throw cix_exit();
      default: break;
   }
}

int main (int argc, char**argv) {
   log.set_execname (basename (argv[0]));
   log << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   signal_action (SIGINT, signal_handler);
   signal_action (SIGTERM, signal_handler);
   int client_fd = stoi (args[0]);
   log << "starting client_fd " << client_fd << endl;
   try {
      accepted_socket client_sock (client_fd);
      log << "connected to " << to_string (client_sock) << endl;
      for (;;) {
         cix_header header;
         recv_packet (client_sock, &header, sizeof header);
         log << "received header " << header << endl;
         switch (header.cix_command) {
            case CIX_LS:
               reply_ls (client_sock, header);
               break;
            case CIX_PUT:
               reply_put (client_sock, header);
               break;
            case CIX_GET:
               reply_get(client_sock, header);
               break;
            case CIX_RM:
               reply_rm(client_sock, header);
               break;
            default:
               log << "invalid header from client" << endl;
               log << "cix_nbytes = " << header.cix_nbytes << endl;
               log << "cix_command = " << header.cix_command << endl;
               log << "cix_filename = " << header.cix_filename << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      log << error.what() << endl;
   }catch (cix_exit& error) {
      log << "caught cix_exit" << endl;
   }
   log << "finishing" << endl;
   return 0;
}

