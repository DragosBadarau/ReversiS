/* cliTCP.c - Exemplu de client TCP 
   Trimite un nume la server; primeste de la server "Hello nume".
         
   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009

*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char msg[100], buff[1024];		// mesajul trimis
  int log=0; //status logare client 
  int tura=1;
  int ret;
  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] Eroare la socket().\n");
      return errno;
    }
  

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if ((ret=connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr))) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
   printf("[client] Serverul are descriptorul %d \n",sd);
    while(log==0)//cat timp clientul nu e logat
    { printf("[client] Introduceti nume:parola pentru a va loga\n");
      /* citirea mesajului */
      bzero (msg, 100);
      fflush (stdout);
      read (0, msg, 100);//se citeste de la tastatura nume:parola
      /* trimiterea mesajului la server */
     if (write (sd, msg, 100) <= 0)
     {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
     }
  /* citirea raspunsului dat de server 
     (apel blocant pina cind serverul raspunde) */
  if (read (sd, &msg, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
     /* afisam mesajul primit */
    printf ("[client] Mesajul de logare primit este: %s\n", msg);
    if(msg[0]=='I')//daca clientul s-a logat
    log=1;
    }
    //dau comanda de start
    
     bzero (msg, 100);
     fflush (stdout);
     read (0, msg, 100);
    if (write (sd, &msg, 100) <= 0)
     {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
     }
     if(msg[0]=='q')//clientul trimite comanda quit, se deconecteaza
     //quit /* inchidem conexiunea, am terminat */
      {close(sd); return 0;}
    printf ("[client] Mesajul trimis este: %s\n", msg);
     //bzero (msg, 100);
     if (read (sd, &msg, 100) <= 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
    printf ("[client] Mesajul primit 1 este: %s\n", msg);
     //Clientul asteapta un adversar 
     while(tura) //incep jocul 
     {if(tura>1)
       printf("[client] Sunt in meci!\n");
       tura++;
      if (read (sd, &msg, 1000) <= 0)//unde pui piesa?/ iesi din meci
      {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
      }
      printf ("[client] Mesajul pt joc este: %s\n", msg);
     bzero (msg, 1000);
     fflush (stdout);
     read (0, msg, 1000);
     if (write (sd, &msg, 1000) <= 0)
     {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
     }
     }
    //quit /* inchidem conexiunea, am terminat */
  close(sd);
  //printf ("[client]Mesajul FINAL este: %s\n", msg);
}