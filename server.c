#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
/* portul folosit */

#define PORT 3061

int clients[20], logat[30], ready=-1;
int pip[100][3], pipp[100][3];
extern int errno;		/* eroarea returnata de unele apeluri */

/* functie de convertire a adresei IP a clientului in sir de caractere */
char * conv_addr (struct sockaddr_in address)
{
  static char str[25];
  char port[7];

  /* adresa IP a clientului */
  strcpy (str, inet_ntoa (address.sin_addr));	
  /* portul utilizat de client */
  bzero (port, 7);
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}

int pass(char password[], int account)
{//printf("[server]Username corect\n");
  int ok=0, nr=0,i,j,k;
char retine[100];
    FILE *fd; fd=fopen("pass.txt", "r+");
    while(nr<account)
    {
        fgets(retine,100,fd); nr++;
    }
    for(i=0;i<strlen(retine);i++)
    {
        if(retine[i]!='\n'&&retine[i]!=' ')
        {retine[ok]=retine[i]; ok++;}
    }
    retine[ok]='\0'; 
    if(strstr(password,retine)!=0)
    return 1;
    else
    {//printf("[server]Parola incorecta\n");
    return 0;}
}

int login(char username[], char password[], int logat[])
{//printf("[server] Verificam datele introduse de client\n");
  int account=0, ok=0, nr=0,i,j,k;
char retine[100];
    FILE *fd; fd=fopen("file.txt","r+");
    while(fgets(retine,100,fd)!=NULL)
    { account++; nr=0;
        for(i=0;i<strlen(retine);i++)
        {
          if(retine[i]!='\n' && retine[i]!=' ')
          {retine[nr]=retine[i];
          nr++;}
        }
     retine[nr]='\0';
     if(strstr(username,retine)!=0 )//username-ul exista
      {if (pass(password, account))
        { 
        if(logat[account]==0)
         { logat[account]=1; ok=1;
         printf("[server] Contul cu numarul %d s-a logat\n", account);
         return account;
         }
         else if(logat[account]==1)
         printf("[server] Acest cont s-a logat deja");
        }
      }
    }
   
   if(ok==0) return 0;
}

void prepare_table(int table[][9])
{
  int i=0,j=0;
  for(i=0;i<8;i++)
  {
    for(j=0;j<8;j++)
    table[i][j]=0;
  }
  table[3][3]=1;
  table[4][4]=1;
  table[3][4]=2;
  table[4][3]=2;
}
void Identificare(int identifica[],int descriptor)
{int account=0, ok=0, nr=0,i,j,k;
char retine[100];
int nr_cont;
nr_cont=identifica[descriptor];
    FILE *fd; fd=fopen("file.txt","r+");
    while(nr<nr_cont)
    {
        fgets(retine,100,fd); nr++;
    }
    for(i=0;i<strlen(retine);i++)
    {
        if(retine[i]!='\n'&&retine[i]!=' ')
        {retine[ok]=retine[i]; ok++;}
    }
    printf("%s", retine);
}
int pozitie(int linie, int coloana,int jucator, int table[][9])
{int i,j;
printf("[server] 2 Verific mutarea \n");
 if(jucator==1)//8 cazuri (pt fiecare vecin posibil)
  {
    if(table[linie+1][coloana+1]==2){
      i=linie+2; j=coloana+2;
      while(i<8 && j<8)
        {if(table[i][j]==1)
         return 1;
         if(table[i][j]==0)
         break;
         else {i++; j++;}
        }
    } 
    if(table[linie-1][coloana-1]==2){
     i=linie-2; j=coloana-2;
     while(i>=0 && j>=0)
      {if(table[i][j]==1)
       return 1;
       if(table[i][j]==0) 
       break;
       else {i--; j--;}
      }
    }
    if(table[linie-1][coloana]==2){
     i=linie-2; j=coloana;
     while(i>=0)
      {if(table[i][j]==1)
       return 1;
       if(table[i][j]==0) 
       break;
       else i--; 
      }
    }
    if(table[linie][coloana-1]==2){
     i=linie; j=coloana-2;
     while(j>=0)
      {if(table[i][j]==1)
       return 1;
       if(table[i][j]==0) 
       break;
       else j--; 
    }}
    if(table[linie+1][coloana]==2){
     i=linie+2; j=coloana;
     while(i<8)
      {if(table[i][j]==1)
       return 1;
       if(table[i][j]==0) 
       break;
       else i++; 
    }}
    if(table[linie][coloana+1]==2){
     i=linie; j=coloana+2;
     while(j<8)
      {if(table[i][j]==1)
       return 1;
       if(table[i][j]==0) 
       break;
       else j++; 
    }}
    if(table[linie+1][coloana-1]==2){
     i=linie+2; j=coloana-2;
     while(i<8 && j>=0)
      {if(table[i][j]==1)
       return 1;
       if(table[i][j]==0) 
       break;
       else {i++; j--;} 
    }}
    if(table[linie-1][coloana+1]==2){
     i=linie-2; j=coloana+2;
     while(j>=0 && i<8)
      {if(table[i][j]==1)
       return 1;
       if(table[i][j]==0) 
       break;
       else {i--; j++;}
    }}
   return 0;
  }
  if(jucator==2)//8 cazuri (pt fiecare vecin posibil)
  {
    if(table[linie+1][coloana+1]==1){
      i=linie+2; j=coloana+2;
      while(i<8 && j<8)
        {if(table[i][j]==2)
         return 1;
         if(table[i][j]==0)
         break;
         else {i++; j++;}
        }
    } 
    if(table[linie-1][coloana-1]==1){
     i=linie-2; j=coloana-2;
     while(i>=0 && j>=0)
      {if(table[i][j]==2)
       return 1;
       if(table[i][j]==0) 
       break;
       else {i--; j--;}
      }
    }
    if(table[linie-1][coloana]==1){
     i=linie-2; j=coloana;
     while(i>=0)
      {if(table[i][j]==2)
       return 1;
       if(table[i][j]==0) 
       break;
       else i--; 
      }
    }
    if(table[linie][coloana-1]==1){
     i=linie; j=coloana-2;
     while(j>=0)
      {if(table[i][j]==2)
       return 1;
       if(table[i][j]==0) 
       break;
       else j--; 
    }}
    if(table[linie+1][coloana]==1){
     i=linie+2; j=coloana;
     while(i<8)
      {if(table[i][j]==2)
       return 1;
       if(table[i][j]==0) 
       break;
       else i++; 
    }}
    if(table[linie][coloana+1]==1){
     i=linie; j=coloana+2;
     while(j<8)
      {if(table[i][j]==2)
       return 1;
       if(table[i][j]==0) 
       break;
       else j++; 
    }}
    if(table[linie+1][coloana-1]==1){
     i=linie+2; j=coloana-2;
     while(i<8 && j>=0)
      {if(table[i][j]==2)
       return 1;
       if(table[i][j]==0) 
       break;
       else {i++; j--;} 
    }}
    if(table[linie-1][coloana+1]==1){
     i=linie-2; j=coloana+2;
     while(j>=0 && i<8)
      {if(table[i][j]==2)
       return 1;
       if(table[i][j]==0) 
       break;
       else {i--; j++;}
    }}
   return 0;
  }
  return 0;
}
void reversi(int linie, int coloana, int table[][9])//functie pentru a schimba piesele adversarului "imbratisate"
{int i,j,ok=0,ki,kj;
  if(table[linie][coloana]==1)//8 cazuri pentru toate cele 8 tipuri de schimbari
  {
    if(table[linie+1][coloana]==2)
    {ok=0;
      i=linie+2; j=coloana;
      while(ok==0)
       for(; i<8;i++)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(i=linie+2;i<ki;i++)
       table[i][j]=1;
       table[linie+1][coloana]=1;
      }
    }
    if(table[linie-1][coloana]==2)
    {ok=0;
      i=linie-2; j=coloana;
      while(ok==0)
       for(; i>0;i--)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(i=linie-2;i>ki;i--)
       table[i][j]=1;
       table[linie-1][coloana]=1;
      }
    }
    if(table[linie][coloana+1]==2)
    {ok=0;
      i=linie; j=coloana+2;
      while(ok==0)
       for(; j<8;j++)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana+2;j<kj;j++)
       table[i][j]=1;
       table[linie][coloana+1]=1;
      }
    }
    if(table[linie][coloana-1]==2)
    {ok=0;
      i=linie; j=coloana-2;
      while(ok==0)
       for(; j>0;j--)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana-2;j>kj;j--)
       table[i][j]=1;
       table[linie][coloana-1]=1;
      }
    }
    if(table[linie+1][coloana+1]==2)
    {ok=0;
      i=linie+2; j=coloana+2;
      while(ok==0)
       for(; j<8 && i<8;j++,i++)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana+2,i=coloana+2;j<kj && i<ki;j++,i++)
       table[i][j]=1;
       table[linie+1][coloana+1]=1;
      }
    }
    if(table[linie-1][coloana-1]==2)
    {ok=0;
      i=linie-2; j=coloana-2;
      while(ok==0)
       for(; j>0 && i>0;j--,i--)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana-2,i=coloana-2;j>kj && i>ki;j--,i--)
       table[i][j]=1;
       table[linie-1][coloana-1]=1;
      }
    }
    if(table[linie-1][coloana+1]==2)
    {ok=0;
      i=linie-2; j=coloana+2;
      while(ok==0)
       for(; j<8 && i>0;j++,i--)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana+2,i=coloana-2;j<kj && i>ki;j++,i--)
       table[i][j]=1;
       table[linie-1][coloana+1]=1;
      }
    }
    if(table[linie+1][coloana-1]==2)
    {ok=0;
      i=linie+2; j=coloana-2;
      while(ok==0)
       for(; j>0 && i<8;j--,i++)
        {if(table[i][j]==1)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana-2,i=coloana+2;j>kj && i<ki;j--,i++)
       table[i][j]=1;
       table[linie+1][coloana-1]=1;
      }
    }
  }
  if(table[linie][coloana]==2)
  {
    if(table[linie+1][coloana]==1)
    {ok=0;
      i=linie+2; j=coloana;
      while(ok==0)
       for(; i<8;i++)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(i=linie+2;i<ki;i++)
       table[i][j]=2;
       table[linie+1][coloana]=2;
      }
    }
    if(table[linie-1][coloana]==1)
    {ok=0;
      i=linie-2; j=coloana;
      while(ok==0)
       for(; i>0;i--)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(i=linie-2;i>ki;i--)
       table[i][j]=2;
       table[linie-1][coloana]=2;
      }
    }
    if(table[linie][coloana+1]==1)
    {ok=0;
      i=linie; j=coloana+2;
      while(ok==0)
       for(; j<8;j++)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana+2;j<kj;j++)
       table[i][j]=2;
       table[linie][coloana+1]=2;
      }
    }
    if(table[linie][coloana-1]==1)
    {ok=0;
      i=linie; j=coloana-2;
      while(ok==0)
       for(; j>0;j--)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana-2;j>kj;j--)
       table[i][j]=2;
       table[linie][coloana-1]=2;
      }
    }
    if(table[linie+1][coloana+1]==1)
    {ok=0;
      i=linie+2; j=coloana+2;
      while(ok==0)
       for(; j<8 && i<8;j++,i++)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana+2,i=coloana+2;j<kj && i<ki;j++,i++)
       table[i][j]=2;
       table[linie+1][coloana+1]=2;
      }
    }
    if(table[linie-1][coloana-1]==1)
    {ok=0;
      i=linie-2; j=coloana-2;
      while(ok==0)
       for(; j>0 && i>0;j--,i--)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana-2,i=coloana-2;j>kj && i>ki;j--,i--)
       table[i][j]=2;
       table[linie-1][coloana-1]=2;
      }
    }
    if(table[linie-1][coloana+1]==1)
    {ok=0;
      i=linie-2; j=coloana+2;
      while(ok==0)
       for(; j<8 && i>0;j++,i--)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana+2,i=coloana-2;j<kj && i>ki;j++,i--)
       table[i][j]=2;
       table[linie-1][coloana+1]=2;
      }
    }
    if(table[linie+1][coloana-1]==1)
    {ok=0;
      i=linie+2; j=coloana-2;
      while(ok==0)
       for(; j>0 && i<8;j--,i++)
        {if(table[i][j]==2)
         ok=1;
         if(table[i][j]==0)
         ok=2;
        }       
      if(ok==1)
      {ki=i; kj=j;
       for(j=coloana-2,i=coloana+2;j>kj && i<ki;j--,i++)
       table[i][j]=2;
       table[linie+1][coloana-1]=2;
      }
    }
  }
}
int winner(int table[][9])
{
  int c1=0,i,j;
  for(i=0;i<8;i++)
  for(j=0;j<8;j++)
  if(table[i][j]==1)
  c1++;
  if(c1>32)
  return 1;
  if(c1==32)
  return 0;
  return 2;
}
void start_match(int p1, int p2, int identifica[])
{ printf("[server] Joaca clientii cu descriptorii:%d, %d\n",p1,p2);
  //printf("[server] Joaca "); Identificare(identifica,p2); 
  //printf(" impotriva lui "); Identificare(identifica,p1); printf("\n");
  int table[9][9];
  write(p1,"match found\n",20);
  write(p2,"match found\n",20); 
  prepare_table(table);
  int pieces=60, ok1=0,ok2=0,mutare,exit,nr=0,linie,coloana,a,b,i,j,win;
  char msg1[10], msg2[10];
  for(i=0;i<8;i++)
          {for(j=0;j<8;j++)
           printf("%d ",table[i][j]);
           printf("\n");
          }
  while(pieces>0){
    ok1=0, ok2=0; 
    while(!ok1)
    {//write table to player1 
    ok1=0;
      write(p1,"unde pui piesa?\n",30);
      write(p2,"este randul adversarului\n", 40);
      read(p1,&msg1, sizeof(int));//clientul alege unde pune piesa
      //printf("Mutare primita: %s \n", msg1);
      read(p2,&msg2, sizeof(int));//verific daca celalalt client vrea sa paraseasca jocul
      //printf("P2 exit: %s \n", msg2);
      mutare=(msg1[0]-'0')*10+msg1[1]-'0';
      if(msg2[0]=='0')
      exit=0; //oponentul ramane in meci
      else
      exit=1; //oponentul renunta
      if(mutare>=0)
      printf("[server]MUTARE:%d, EXIT:%d \n",mutare,exit);
      linie=mutare/8; 
      coloana=mutare%8;
      if(table[linie-1][coloana]==2 || table[linie][coloana-1]==2 ||table[linie][coloana+1]==2 ||table[linie+1][coloana]==2  )
      {printf("[server] 1 Verific mutarea 1\n");
        if(pozitie(linie,coloana,1,table))
        {printf("[server] Mutare corecta 1\n");
          table[linie][coloana]=1;
          pieces--;
          ok1=1;
          nr=0;
          reversi(linie,coloana,table);
          for(i=0;i<8;i++)
          {for(j=0;j<8;j++)
           printf("%d ",table[i][j]);
           printf("\n");
          }
        } 
      }
    }
   while(!ok2)
   {if(nr==0)
     printf("Se inverseaza rolurile 2 \n");
     nr++;
     //write table to player2
      ok2=0;
      write(p2,"unde pui piesa?",30);
      write(p1,"este randul adversarului", 40);
      read(p2,&msg2, sizeof(int));
      read(p1,&msg1, sizeof(int));//verific daca celalalt client vrea sa paraseasca jocul
       mutare=(msg2[0]-'0')*10+msg2[1]-'0';
       if(msg1[0]=='0')
      exit=0; //oponentul ramane in meci
      else
      exit=1; //oponentul renunta
      if(mutare>=0)
      printf("[server]MUTARE:%d, EXIT:%d \n",mutare,exit);
      linie=mutare/8; 
      coloana=mutare%8;
      if(table[linie-1][coloana]==1 || table[linie][coloana-1]==1 ||table[linie][coloana+1]==1 ||table[linie+1][coloana]==1  )
      {printf("[server] 1 Verific mutarea 2\n");
        if(pozitie(linie,coloana,2,table))
        {printf("[server] Mutare corecta 2 \n");
          table[linie][coloana]=2;
          pieces--;
          ok2=1;
          reversi(linie,coloana,table);
          for(i=0;i<8;i++)
          {for(j=0;j<8;j++)
           printf("%d ",table[i][j]);
           printf("\n");
          }
        }

      }
   }
  }
  win=winner(table);
  if(win==1)
  printf("[server] A castigat "); Identificare(identifica,p1); printf("!!!\n");
  if(win==2)
  printf("[server] A castigat "); Identificare(identifica,p2); printf("!!!\n");
  if(win==0) 
  printf("[server] Egalitate!\n");
}

/* programul */
int main ()
{
  struct sockaddr_in server;	/* structurile pentru server si clienti */
  struct sockaddr_in from;
  int sd, client;		/* descriptori de socket */
  int optval=1; 			/* optiune folosita pentru setsockopt()*/ 
  int nfds;			/* numarul maxim de descriptori */
  int len,i,k,j, log=0, ok=0;			/* lungimea structurii sockaddr_in */
  char username[100], password[100], buffer[1024];
  int total_clients=6, nr_clients=0, identifica[100];
  int username_len=0, password_len=0;
  int astept[30], nrastept=0;
  int adversar=0;
  int s=1;
  char adv;
  pid_t childpid;
  int newsocket;
  for(i=0;i<20;i++)
    clients[i]=0;
  for(k=0;k<29;k++)
        logat[k]=0; 
        ready=-1;

  /* creare socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server] Eroare la socket().\n");
      return errno;
    }

  /*setam pentru socket optiunea SO_REUSEADDR */ 
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(optval));

  /* pregatim structurile de date */
  bzero (&server, sizeof (server));

  /* umplem structura folosita de server */
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl (INADDR_ANY);
  server.sin_port = htons (PORT);

  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server] Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 10) == -1)
    {
      perror ("[server] Eroare la listen().\n");
      return errno;
    }
        
  /* servim in mod concurent clientii... */
  while (1)
    {
     printf ("[server] Asteptam la portul %d...\n", PORT);
     printf("[server] Astept comanda \n");
	   /* pregatirea structurii client */
	   len = sizeof (from);
     fflush(stdout);
	   bzero (&from, sizeof (from));

	   /* a venit un client, acceptam conexiunea */
	   newsocket = accept (sd, (struct sockaddr *) &from, &len);
     nr_clients++;
     clients[nr_clients]=newsocket;
     printf("[server] Sunt %d clienti\n",nr_clients);
       printf("[server] DESCRIPTORUL ESTE:%d \n",clients[nr_clients]);
	   /* eroare la acceptarea conexiunii de la un client */
	   if (newsocket< 0)
	    {
	      perror ("[server] Eroare la accept().\n");
	      exit(1);
        //continue;
	    }
      printf("[server] S-a conectat clientul de la adresa: %s%d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));        
      printf("[server] S-a conectat clientul cu descriptorul:%d\n",clients[nr_clients]);
      int nrord=0;
      if((childpid=fork())==0)
      { log=0;
        while(log==0)//cat timp clientul nu e conectat
        {nrord++; if(nrord>1) printf("[server] Clientul cu descriptorul %d nu s-a logat\n",clients[nr_clients] );
          bzero(buffer,sizeof(buffer));
          read(clients[nr_clients],&buffer, 50); //citesc numele si parola clientului conectat
           printf ("[server] Mesajul primit 1 este:%s\n", buffer);
          // printf("[server] DESCRIPTORUL ESTE:%d \n",clients[nr_clients]);
          ok=0; username_len=0; password_len=0;
          for(i=0;i<sizeof(buffer);i++)
           {
            if(buffer[i]==':')
            ok=1;
            if(ok==0)
            username[username_len++]=buffer[i];
            if(ok==1)
            password[password_len++]=buffer[i];
           }
          // printf ("[server] Username-ul primit este:%s\n Parola primita este:%s", username, password);
           if(log=login(username,password,logat))  
           {
             write(clients[nr_clients], "Introduceti start:adversar/quit",45);
             }
           else 
           {  
             write(clients[nr_clients], "Nu te-ai logat!",25);
           } 
        }
        //printf("[server] CONTUL cu numarul %d \n",log);
        identifica[clients[nr_clients]]=log; ///Retin pentru fiecare descriptor numele
        printf("[server] Numele userului logat este: " );
        Identificare(identifica, clients[nr_clients]);
        printf("\n");
        bzero(buffer, sizeof(buffer));
        printf("[server] S-a logat clientul cu descriptorul:%d\n",clients[nr_clients]);
        while(s)
        {
        if(read(clients[nr_clients],&buffer, sizeof(buffer))<0)
        {
          perror("[server]Eroare la citirea mesajului de la client");
          close(clients[nr_clients]);
          continue;//?
        }
        printf("[server] Mesajul primit 2 este:%s\n",buffer);
        //buffer[6]='\0';
        if(buffer[0]=='s')
        /////////start:descriptor_adversar
        {s=0;
          printf("[server] INTRAM IN START \n");
          //printf("[server] ADVERSAR: %c \n",buffer[6]);
          adversar=buffer[6]-'0';//retin descriptorul adversarului
          //daca descriptorul adversarului e 0, clientul care trimite acel mesaj asteapta
          //daca e diferit de 0, clientul care trimite acel mesaj va incepe un meci cu clientul
          //cu descriptorul transmis
          //printf("[server] ADVERSAR: %d \n",adversar);
          if(adversar==0)
          {printf("[server] Clientul cu descriptorul %d asteapta \n",clients[nr_clients]);
           //bzero(buffer,sizeof(buffer));
             //strcpy(buffer, "Asteptam un adversar...");
             //write(clients[nr_clients], &buffer, sizeof(buffer));
             write(clients[nr_clients], "Asteptam un adversar...", 30);
             //write(0, &buffer, sizeof(buffer));
             //send(clients[nr_clients], buffer, sizeof(buffer),0);
           //bzero(buffer,sizeof(buffer));
           //ready=clients[nr_clients];
           //start_match(clients[nr_clients],ready+1);
           }
          else if(adversar!=0)
          {printf("[server] Incepe un meci!\n");
            //start_match(clients[nr_clients],ready);
             write(clients[nr_clients], "Asteptam un adversar...", 30);
           start_match(clients[nr_clients],adversar,identifica);
           //ready=-1;
           }
        }
        else if(buffer[0]=='q')
          { 
            nr_clients=nr_clients-1;
            printf ("[server] S-a deconectat clientul cu descriptorul %d.\n",clients[nr_clients]);
            printf("[server] Au ramas %d clienti\n",nr_clients);
		        close (sd);		/* inchidem conexiunea cu clientul */
          }
        }/* while */
      }/*if fork*/
		  
		  
		}/* while */
}/* main */


         
