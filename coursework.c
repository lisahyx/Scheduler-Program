#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<stdbool.h>
#include <ctype.h>

//Global variables
float Avg_Wait_SJN;
float Avg_Turn_SJN;
float Avg_Wait_RR;
float Avg_Turn_RR;
float Avg_Wait_RRO;
float Avg_Turn_RRO;

//Linked List For Shortest Job Next
struct node {
   int processID;
   int Bursttime;
   int waitingTime;
   int turnaroundTime;
   struct node *next;
   struct node *prev;
};
struct node *head = NULL;
struct node *tail = NULL;

//Function For Shortest Job Next
void insertFirst(int,int,struct node*);
void swap(struct node*,struct node*);
void sort(struct node*);
void IDsort(struct node*);
void printList (int n, struct node *);
void waitingTime(struct node *);
void TurnaroundTime(struct node *);
float avgWaitTime(int n,struct node *start);
float avgTurnTime(int n, struct node *start);
int InputValidation();



//function for Round Robin With overhead
void waitingtime_rro(int process[], int n, int burst[], int wait[], int tq,int overhead, int complete[]);
void avgtime_rro(int process[],int n, int burst[], int tq, int overhead);

//function for Round Robin without overhead
void avgtime(int process[],int n, int burst[], int tq);
void waitingtime (int process[], int n, int burst[], int wait[], int tq, int complete[]);

//function for comparison
void comparison();



//main method
int main(void) {
  struct node *link = (struct node*) malloc(sizeof(struct node));
  int process_sjn,Bursttime,counter;int autoID=0;

  int n;
  printf("How many process you want to run: ");
  n=InputValidation();
  int process[n-1];
  int burst[n-1];
  int tq;
  int overhead;
  int id_overhead=0;
  for (int i =0; i<n;i++){
  process[i]=++id_overhead;
  printf("Enter the burst time for process %i : ",process[i]);
  burst[i]=InputValidation();
  insertFirst(process[i], burst[i],link);

  }
  sort(link);
  waitingTime(head);
  TurnaroundTime(head);
  IDsort(link);
  printf("Enter the Quantum Time for Round Robin: ");
  tq = InputValidation();
  printf("Enter the Overhead for Round Robin: ");
  overhead= InputValidation();
  system("clear");
  printf("This is the details of Process that run in Shortest Job Next\n");
  printList(n,head);
  printf("\n");
  printf("This is the details of Process that run in Round Robin Without Overhead\n");
  avgtime(process, n, burst, tq);
  printf("\n");
  printf("This is the details of Process that run in Round Robin With Overhead\n");
  avgtime_rro(process, n, burst,  tq, overhead);
  comparison();
  return 0;
}

//insert the process id and burst time into the linked list for SJN
void insertFirst(int processID, int Bursttime,struct node*link) {
	 link = (struct node*) malloc(sizeof(struct node));
   link->processID = processID;
   link->Bursttime = Bursttime;
   if(head == NULL){
   head =link;
   tail = link;
   link->next=NULL;
   link->prev =NULL;
   }
   else {
    link->next= head;
    head->prev= link;
    head=link;
   }
}

//sort the linked list to make all the burst time arranged in ascending order for SJN
void sort(struct node*link ){
    int swapped, i;
    struct node *Forsort ;
    /* Checking for empty list */
    if (head == NULL)
        return;
    do
    {
        swapped = 0;
        Forsort  = head;
        while (Forsort ->next != NULL)
        {
            if (Forsort ->Bursttime > Forsort ->next->Bursttime)
            {
                swap(Forsort , Forsort ->next);
                swapped = 1;
            }
            Forsort  = Forsort ->next;
        }
    }
    while (swapped);
    link=Forsort ;
}

//Swap the linked is data when arranging for SJN
void swap(struct node *a, struct node *b)
{
    int temp = a->Bursttime;
    a->Bursttime = b->Bursttime;
    b->Bursttime = temp;

    int temp2 = a->processID;
    a->processID = b->processID;
    b->processID = temp2;
}

//Calculate the average waiting time for all the process for SJN
float avgWaitTime(int n,struct node *start){
  struct node *temp = start;
  float total=0;
  float avgWait=0;
  while (temp!=NULL){
    total+=temp->waitingTime;
    temp=temp->next;
  }
  avgWait = total/n;
  return avgWait;
}

//Calculate the average turnaround time for all the process for SJN
float avgTurnTime(int n,struct node *start){
  struct node *temp = start;
  float total=0;
  float avgTurn=0;
  while (temp!=NULL){
    total+=temp->turnaroundTime;
    temp=temp->next;
  }
  avgTurn = total/n;
  return avgTurn;
}

//Print the element our from the linked list for SJN
void printList(int n,struct node *start)
{
    struct node *temp = start;
    float avg_wait =(float)avgWaitTime(n,temp);
    float avg_turn = (float)avgTurnTime(n,temp);
    printf("Process\t\tBurst Time\t\tWaiting Time\t\tTurn Around Time\n");
    while (temp!=NULL)
    {
        printf("%i\t\t\t\t%i\t\t\t\t%i\t\t\t\t\t\t%i\n",temp->processID, temp->Bursttime,temp->waitingTime,temp->turnaroundTime);
        temp = temp->next;
    }
    Avg_Wait_SJN=avg_wait;
    Avg_Turn_SJN=avg_turn;
    printf("\n\nThe average waiting time is %.2f\n",avg_wait);
    printf("The aerage turn around time is %.2f\n",avg_turn);
    printf("-----------------------------------------------------\n");
}

//Calculate the waiting time of each process for SJN
void waitingTime (struct node* start)
{
    int sum =0 ;
    start-> waitingTime=0;
    start= start->next;
    while(start!=NULL){
      sum+=start->prev->Bursttime ;
      start->waitingTime=sum;
      start= start->next;
    }
}

//Calculate the turn around time for each process for SJN
void TurnaroundTime(struct node *start){
  int sum=0;
  while(start!=NULL){
    sum+=start->Bursttime;
    start->turnaroundTime=sum;
    start= start->next;
  }
}

//Sort the process ID in ascending order to be print accordingly for SJN
void IDsort(struct node*link ){
    int swapped, i;
    struct node *Forsort ;
    if (head == NULL)
        return;
    do
    {
        swapped = 0;
        Forsort  = head;
        while (Forsort ->next != NULL)
        {
            if (Forsort ->processID > Forsort ->next->processID)
            {
                int temp1=Forsort->Bursttime;
                Forsort->Bursttime= Forsort->next->Bursttime;
                Forsort->next->Bursttime=temp1;
                int temp2= Forsort->waitingTime;
                Forsort->waitingTime= Forsort->next->waitingTime;
                Forsort->next->waitingTime=temp2;
                int temp3=Forsort->turnaroundTime;
                Forsort->turnaroundTime= Forsort->next->turnaroundTime;
                Forsort->next->turnaroundTime=temp3;
                int temp4=Forsort->processID;
                Forsort->processID=Forsort->next->processID;
                Forsort->next->processID=temp4;
                swapped = 1;
            }
            Forsort  = Forsort ->next;
        }
    }
    while (swapped);
    link=Forsort ;
}

//calculate the waiting time and completion time for each process in Round Robin without overhead algorithm
void waitingtime (int process[], int n, int burst[], int wait[], int tq, int complete[]){
  int rem_burst[n];
  int done;
  for (int i =0; i<n;i++){
    rem_burst[i] = burst[i];
  }
  int current_time =0;
  do{
    done = 1;
    for (int i=0; i<n; i++){
      if (rem_burst[i] > 0){
          done = 0;
          if (rem_burst[i] > tq)
          {
            current_time = current_time +tq ;
            rem_burst[i] -= tq;
          }
          else{
            current_time = current_time + rem_burst[i];
            wait[i] = i*(tq);
            complete[i] = current_time;

            rem_burst[i] = 0;
          }
      }
    }
    if(done==1){
      break;
    }
  }while(true);
}

//calculate the average waiting time and average turn around time  in Round Robin without overhead algorithm and produce the output sreen
void avgtime(int process[],int n, int burst[], int tq){
  int wait[n];
  int turn[n];
  int complete[n];
  int total_wait = 0;
  int total_turn = 0;
waitingtime(process, n, burst, wait, tq,complete);
printf("Process\t\tBurst Time\t\tWaiting Time\t\tTurn Around Time\n");
for (int j=0;j<n;j++){
  total_wait = total_wait + wait[j];
  total_turn = total_turn + complete[j];
  printf("%i\t\t\t\t%i\t\t\t\t%i\t\t\t\t\t\t%i\n",j+1,burst[j],wait[j],complete[j]);
}
float avg_wait = (float)total_wait/n;
float avg_turn = (float)total_turn/n;
Avg_Wait_RR=avg_wait;
Avg_Turn_RR=avg_turn;
printf("\n\nThe average waiting time is %.2f\n",avg_wait);
printf("The average turn around time is %.2f\n",avg_turn);
printf("-----------------------------------------------------\n");
}

//calculate the waiting time and completion time for each process in Round Robin with overhead algorithm
void waitingtime_rro(int process[], int n, int burst[], int wait[], int tq,int overhead, int complete[]){
  int rem_burst_overhead[n];
  int done_overhead;
  for (int i =0; i<n;i++){
    rem_burst_overhead[i] = burst[i];
  }
  int current_time_overhead =-(overhead);
  do{
    done_overhead = 1;
    for (int i=0; i<n; i++){
      if (rem_burst_overhead[i] > 0){
          done_overhead = 0;
          if (rem_burst_overhead[i] > tq)
          {
            current_time_overhead = current_time_overhead +tq + overhead;
            rem_burst_overhead[i] -= tq;
          }
          else{
            current_time_overhead = current_time_overhead + rem_burst_overhead[i]+overhead;
            wait[i] = i*(tq+overhead);
            complete[i] = current_time_overhead;
            rem_burst_overhead[i] = 0;
          }
      }
    }
    if(done_overhead==1){
      break;
    }
  }while(true);
}

//calculate the average waiting time and average turn around time  in Round Robin with overhead algorithm and produce the output sreen
void avgtime_rro(int process[],int n, int burst[], int tq, int overhead){
  int wait_overhead[n];
  int turn_overhead[n];
  int complete_overhead[n];
  int total_wait_overhead = 0;
  int total_turn_overhead = 0;
waitingtime_rro(process, n, burst, wait_overhead, tq, overhead,complete_overhead);
printf("Process\t\tBurst Time\t\tWaiting Time\t\tTurn Around Time\n");
for (int j=0;j<n;j++){
  total_wait_overhead = total_wait_overhead + wait_overhead[j];
  total_turn_overhead = total_turn_overhead + complete_overhead[j];
  printf("%i\t\t\t\t%i\t\t\t\t%i\t\t\t\t\t\t%i\n",j+1,burst[j],wait_overhead[j],complete_overhead[j]);
}
float avg_wait_overhead = (float)total_wait_overhead/n;
float avg_turn_overhead = (float)total_turn_overhead/n;
Avg_Wait_RRO=avg_wait_overhead;
Avg_Turn_RRO=avg_turn_overhead;
printf("\n\nThe average waiting time is %.2f\n",avg_wait_overhead);
printf("The average turn around time is %.2f\n",avg_turn_overhead);
printf("-----------------------------------------------------\n");
}

//Fucntion used for comparison to find the best one accoding to the choosen comaprison algorithm
void comparison(){
  float min_wait;
  float min_turn;
  float min_wait_logic;
  float min_turn_logic;
  char best_wait[50];
  char best_turn[50];
  char best_logic[50];
  char best[50];
  float SJN = Avg_Wait_SJN + Avg_Turn_SJN;
  float RR = Avg_Wait_RR + Avg_Turn_RR;
  float RRO = Avg_Wait_RRO + Avg_Turn_RRO;
  float min;
  int selection;
  printf("Please choose the algorithm that you want to choose the best algorithm that suit your process:\n1.Average Waiting Time + Average Turn Around Time\n2.Best Average Waiting Time\n3.Best Average Turn Around Time\n");
  printf("What is your choice: ");
  scanf("%i",&selection);
switch(selection){
  case 1:{
      if(SJN<RR && SJN < RRO){
          min = SJN;
          strcpy(best,"Shortest Job Next");
          printf("\n\nThe algorithm that have the shortest total average waiting time and turn around time is %s with time %.2f microseconds\n\n",best,min);
      }
      else if (RR<SJN && RR<RRO){
        min = RR;
        strcpy(best,"Round Robin Without Overhead");
        printf("\n\nThe algorithm that have the shortest total average waiting time and turn around time is %s with time %.2f microseconds\n\n",best,min);
      }
      else if (RRO<SJN && RRO<RR){
        min = RRO;
        strcpy(best,"Round Robin With Overhead");
        printf("\n\nThe algorithm that have the shortest total average waiting time and turn around time is %s with time %.2f microseconds\n\n",best,min);
      }
      break;
  }
  case 2:{
  if(Avg_Wait_SJN < Avg_Wait_RR  && Avg_Wait_SJN < Avg_Wait_RR){
    min_wait = Avg_Wait_SJN;
    strcpy(best_wait,"Shortest Job Next");
    printf("\n\nThe algorithm that have the shortest average waiting time is %s with time %.2f microseconds\n\n",best_wait,min_wait);
  }
    else if(Avg_Wait_RR < Avg_Wait_SJN  && Avg_Wait_RR < Avg_Wait_RRO){
    min_wait = Avg_Wait_RR;
    strcpy(best_wait,"Round Robin Without Overhead");
    printf("\n\nThe algorithm that have the shortest average waiting time is %s with time %.2f microseconds\n\n",best_wait,min_wait);
    if(min_wait == Avg_Wait_RR){
    min_wait_logic = Avg_Wait_RRO;
    strcpy(best_logic,"Round Robin With Overhead");
    if(Avg_Turn_RRO > Avg_Turn_SJN)
    {
      min_wait_logic = Avg_Wait_SJN;
      strcpy(best_logic,"Shortest Job Next");

    }
    printf("In real life condition, the Round Robin algorithm is coming with Overhead.\nTherefore we should take the %s (The Smallest Among them) as the best algorithm in real life.",best_logic);

  }
  }
      else if(Avg_Wait_RRO < Avg_Wait_SJN  && Avg_Wait_RRO < Avg_Wait_RR){
      min_wait = Avg_Wait_RRO;
      strcpy(best_wait,"Round Robin With Overhead");
      printf("\n\nThe algorithm that have the shortest average waiting time is %s with time %.2f microseconds\n\n",best_wait,min_wait);
  }
  break;
  }
  case 3:{
  if(Avg_Turn_SJN < Avg_Turn_RR  && Avg_Turn_SJN < Avg_Turn_RR){
    min_turn = Avg_Turn_SJN;
    strcpy(best_turn,"Shortest Job Next");
    printf("\n\nThe algorithm that have the shortest average turn around time is %s with time %.2f microseconds\n\n",best_turn,min_turn);
  }
   else if(Avg_Turn_RR < Avg_Turn_SJN  && Avg_Turn_RR < Avg_Turn_RRO){
    min_turn = Avg_Turn_RR;
    strcpy(best_turn,"Round Robin Without Overhead");
    printf("\n\nThe algorithm that have the shortest average turn around time is %s with time %.2f microseconds\n\n",best_turn,min_turn);
    if(min_turn == Avg_Turn_RR){
    min_turn_logic = Avg_Turn_RRO;
    strcpy(best_logic,"Round Robin With Overhead");
    if(Avg_Turn_RRO > Avg_Turn_SJN)
    {
      min_turn_logic = Avg_Turn_SJN;
      strcpy(best_logic,"Shortest Job Next");
    }
    printf("In real life condition, the Round Robin algorithm is coming with Overhead.\nTherefore we should take the %s as the best algorithm in real life.",best_logic);
  }
  }
      else if(Avg_Turn_RRO < Avg_Turn_SJN  && Avg_Turn_RRO < Avg_Turn_RR){
      min_turn = Avg_Turn_RRO;
      strcpy(best_turn,"Round Robin With Overhead");
      printf("\n\nThe algorithm that have the shortest average turn around time is %s with time %.2f microseconds\n\n",best_turn,min_turn);
      }
  break;
  }
  default:{
    printf("Invalid");
  }
}
}

//Function to check whether the input is number
int  InputValidation()
{
	int temp = 0;
	while(scanf("%d" ,&temp)!= 1)
	{
		printf("please enter valid input:");
		scanf("%*s");
	}
	return temp;
}


