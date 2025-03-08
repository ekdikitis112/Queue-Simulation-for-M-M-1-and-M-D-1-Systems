/*************************************
*                                    *
*  EFTYCHIA MARIA KOUMENTAKOU        *
*                                    *
*  STYLIANOS PAPADOPOULOS            *
*                                    *
*************************************/


#include <stdio.h> //input and output
#include <stdlib.h> //rand gia genard kai genarr
#include <math.h> //gia to exp
#include <time.h> //gia na leitourgei swsta h rand 

typedef struct queue{
    struct queue *next; //enas deikths pou koitaei sto epomeno keli ths  ouras
    double ftime,artime,stime;  //finish arrival service
}node;

node *head=NULL,*tail=NULL,*list=NULL; //kefalh telos link  global metablhtes 

void menu(void){
    printf("\n\t MENU");
    printf("\n    1.Add item to the list");
    printf("\n    2.Remove item from the list");
    printf("\n    3.Show the list");
    printf("\n    4.Exit\n");
}



double put(double servtime,double arivtime,char* queue_type);
void get(void);
void printlist(void);
void freelist(void);
double genrand(double rate);
double genarr(double rate);

int main(int argc,char *argv[]){ //counter of arguments , array of args 

    srand(time(0)); // seed of rand 

    int choise=0,num,i,clients=0;
    double servtime=0,arivtime=0;
    
    /* ****************************************************
     * to run code enter arguments in the following order *
     *  1. the name of the program                        *
     *  2. number of clients (integer)                    *
     *  3. the rate of arrival (decimal)                  *
     *  4. rate of service(decimal)                       *
     *  5. M FOR M/M/1  OR D FOR M/D/1                    *
     **************************************************** */
     
    if ( argc != 5 ) //periptwseis me error
    {    
      printf("\n\n!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!\n");
      printf( "\n\t* PLEASE ENTER ARGUMENTS WITH THE FIRST BEING %s\n\t* THE SECOND BEING THE  NUMBER  OF CLIENTS \n\t* THE THIRD BEING THE RATE OF ARIVAL ", argv[0] );
      printf("\n\t* THE FORTH BEING THE RATE OF SERVICE \n\t* THE FIFTH BEING 'D' OR 'M' FOR M/D/1 OR M/M/1 QUEUE RESPECTIVELY\n\t!WARNING! ARGUMENTS ARE SEPARATED BY SPACE \n\n");
      printf("!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!!ATTENTION!\n\n");
      exit(0);
    }
    else if((strcmp(argv[4],"d")!=0) && (strcmp(argv[4],"D")!=0) && (strcmp(argv[4],"M")!=0) && (strcmp(argv[4],"m")!=0)){ // An den eisaxthei to swsto mhnyma kai terrmatismos
      printf("\n\t PLEASE ENTER 'D' OR 'M' FOR M/D/1 OR M/M/1 QUEUE\n");
      exit(0);
    }
    else if( (atof(argv[2]) <=0) || (atof(argv[2])>= atof(argv[3])))
    {
      printf("\n THE RATE OF ARIVAL  SHOULD BE BETWEEN 0 AND 1  \n"); //l
      exit(0); 
    }
    else if (atof(argv[3])<=0)
    {
      printf( "\n PLEASE ENTER SERVICE TIME\n" );//m
      exit(0);   
    }
    else if (atoi(argv[1])<0){
      printf("\n PLEASE ENTER AT LEAST ZERO CLIENTS \n");
      exit(0);
    }
    else //arxikopoihsh service time ,arrival time kai clients
    {
      servtime=1;     //for m/d/1
      if((strcmp(argv[4],"m")==0) || (strcmp(argv[4],"M")==0)){ // for m/m/1
        servtime = atof(argv[3]); 
      }
      arivtime = atof(argv[2]);
      clients = atoi(argv[1]);
    }

    FILE *f=fopen("TORTURE.csv","w");
    if(f==NULL){ //gia logous asfaleias se periptvsh pou den katafere na ftiaksei h na brei to arxeio
      exit(1);
    }
    
    printf("\n\n");
    printf("*********************************************************************\n");
    printf("* CLIENT | ARRIVAL TIME | SERVICE TIME | FINISH TIME | WAITING TIME *\n");
    printf("*********************************************************************\n");
    fprintf(f,"CLIENT,ARRIVAL TIME,SERVICE TIME,FINISH TIME,WAITING TIME\n"); //ektypwsh se csv
    
    double wtime=0;//waiting time
    for(i=1;i<=clients;i++)
    {
      wtime=put(servtime,arivtime,argv[4]); //adding to the list and returning waiting time simultaniously
      
      //for printing on screen
      printf("*    %3d |     %.4lf |       %.4lf |       %.4lf |    %.4lf \n",i,tail->artime,tail->stime,tail->ftime,wtime); //clients ,arrival time , service time , finish time, waiting time 
      printf("*-------------------------------------------------------------------*\n");

      //saving on file
      fprintf(f,"%d,%lf,%lf,%lf,%lf\n",i,tail->artime,tail->stime,tail->ftime,wtime); 

      get();  //deleting head from the  list
    }
    printf("*******************************************************************\n");
    fclose(f);
}


    /* *****************************************************
     *  Ftiaxnei prvsorino deikth tmp typou node           *
     *  Desmevei mnhmh gia thn dhmiourgeia pediou ths ouras*   
     *  Dinei times sta pedia tou struct ths ouras         *                     
     *  Prosthetei to prwsorino struct sthn lista          *           
     *******************************************************/

double put (double servtime,double arivtime,char *queue_type){   
    node* tmp; 
    double wtime=0;
    tmp=(node*)malloc(sizeof(node)); 
    if(tmp==NULL){ 
      printf("ERROR"); //not available memmory
        return;
    }

    if(head==NULL){
    
        head = tail = list = tmp;//ola pairnoyn thn timh tou tmp ,tou struct pou molis eftiaksa
        head->artime = genarr(arivtime);
        head->stime = servtime;    //for m/d/1
        
        if((strcmp(queue_type,"m")==0) || (strcmp(queue_type,"M")==0)) //for m/m/1
          head->stime = genrand(servtime);
          
        head->ftime = head->artime + head->stime;
        wtime=0; //einai o prwtos
    }
    else{  
        
        tmp->artime = genarr(arivtime) + tail->artime;
        tmp->stime = servtime;   //for m/d/1
        
        if((strcmp(queue_type,"m")==0) || (strcmp(queue_type,"M")==0)) //for m/m/1
          tmp->stime=genrand(servtime);
          
        tmp->ftime = tmp->stime + tail->ftime;
        wtime = tmp->ftime - tmp->artime + tmp->stime;//toy telaiftaiou to ftime-arrtime+stime
        list = tail = list->next = tmp;
    }
    return wtime;
}

//afairesh olwn twn pwlatwn pou exoun ftime < arrtime tou teleutaiou pelath
void get(void){
    
  if( head->ftime<tail->artime){
    node* tmp=head->next;
    free(head);
    head=tmp;    
  }
        
}

//for testing
void printlist(void){
    node *tmp;
    tmp=head;
    printf("\n [");
    while(tmp!=NULL){
        printf(" %lf,",tmp->artime);
        tmp=tmp->next;
    }
    printf(" ]\n");
}


void freelist(void){
    free(list);
}


double genrand(double rate){ //rate =l,m
int y;
double p;
while ((y = rand()) == RAND_MAX);
p = (double) y / RAND_MAX;
return -log(1-p) / rate;
}


double genarr(double rate){//rate =l,m
  double t; 
  while ((t = rand()) == RAND_MAX);
  t = (double) t / RAND_MAX;
  return rate*(exp(-rate*t));
}
