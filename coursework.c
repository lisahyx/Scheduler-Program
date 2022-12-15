/*
G52OSC Coursework 

Group 3:
Chai Qin Hui 20307184
Kelly Tan Kai Ling 20310184
Lisa Ho Yen Xin 20297507
Tan En Xuan 20297487
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// global variables
float avg_wait_FCFS;
float avg_turn_FCFS;
float avg_wait_RR;
float avg_turn_RR;

// linked List For Shortest Job Next
struct node
{
  int processID;
  int Bursttime;
  int waitingTime;
  int turnaroundTime;
  struct node *next;
  struct node *prev;
};
struct node *head = NULL;
struct node *tail = NULL;

// functions for input validation
int checkInput();
bool checkTimeQuantum(int input);

// functions for FCFS
void insertFirst(int, int, struct node *);
void swap(struct node *, struct node *);
void sort(struct node *);
void IDsort(struct node *);
void printList(int n, struct node *);
void waitingTime(struct node *);
void TurnaroundTime(struct node *);
float avgWaitTime(int n, struct node *start);
float avgTurnTime(int n, struct node *start);

// functions for RR
void avgtime_RR(int process[], int n, int burst[], int timequantum);
void algorithm_RR(int process[], int n, int burst[], int wait[], int turnaround[], int timequantum);

// function for comparison
void comparison();

// main method
int main(void)
{
  struct node *link = (struct node *)malloc(sizeof(struct node));
  int process_sjn, Bursttime, counter;
  int autoID = 0;

  int n;
  int temp;
  printf("Enter total number of processes: ");
  n = checkInput();

  int process[n - 1];
  int burst[n - 1];
  int timequantum;
  int overhead;
  int id_overhead = 0;
  for (int i = 0; i < n; i++)
  {
    process[i] = ++id_overhead;
    printf("Enter burst time for process[%i] : ", process[i]);
    burst[i] = checkInput();
    insertFirst(process[i], burst[i], link);
  }
  sort(link);
  waitingTime(head);
  TurnaroundTime(head);
  IDsort(link);
  printf("Enter Time Quantum for Round Robin: ");
  do
  {
    timequantum = checkInput();
  } while (!checkTimeQuantum(timequantum));

  system("cls");

  printf("Algorithm for First Come First Serve (FCFS):\n\n");
  printList(n, head);
  printf("\n");

  printf("Algorithm for Round Robin (RR):\n\n");
  avgtime_RR(process, n, burst, timequantum);
  printf("\n");

  comparison();
  return 0;
}

/* ****************************************** First Come First Serve (FCFS) ****************************************** */

// insert the process id and burst time into the linked list for SJN
void insertFirst(int processID, int Bursttime, struct node *link)
{
  link = (struct node *)malloc(sizeof(struct node));
  link->processID = processID;
  link->Bursttime = Bursttime;
  if (head == NULL)
  {
    head = link;
    tail = link;
    link->next = NULL;
    link->prev = NULL;
  }
  else
  {
    link->next = head;
    head->prev = link;
    head = link;
  }
}

// sort the linked list to make all the burst time arranged in ascending order for SJN
void sort(struct node *link)
{
  int swapped, i;
  struct node *Forsort;
  //check for empty list
  if (head == NULL)
    return;
  do
  {
    swapped = 0;
    Forsort = head;
    while (Forsort->next != NULL)
    {
      if (Forsort->Bursttime > Forsort->next->Bursttime)
      {
        swap(Forsort, Forsort->next);
        swapped = 1;
      }
      Forsort = Forsort->next;
    }
  } while (swapped);
  link = Forsort;
}

// swap the linked is data when arranging for SJN
void swap(struct node *a, struct node *b)
{
  int temp = a->Bursttime;
  a->Bursttime = b->Bursttime;
  b->Bursttime = temp;

  int temp2 = a->processID;
  a->processID = b->processID;
  b->processID = temp2;
}

// calculate the average waiting time for all the process for SJN
float avgWaitTime(int n, struct node *start)
{
  struct node *temp = start;
  float total = 0;
  float avgWait = 0;
  while (temp != NULL)
  {
    total += temp->waitingTime;
    temp = temp->next;
  }
  avgWait = total / n;
  return avgWait;
}

// calculate the average turnaround time for all the process for SJN
float avgTurnTime(int n, struct node *start)
{
  struct node *temp = start;
  float total = 0;
  float avgTurn = 0;
  while (temp != NULL)
  {
    total += temp->turnaroundTime;
    temp = temp->next;
  }
  avgTurn = total / n;
  return avgTurn;
}

// print the element our from the linked list for SJN
void printList(int n, struct node *start)
{
  struct node *temp = start;
  float avg_wait = (float)avgWaitTime(n, temp);
  float avg_turn = (float)avgTurnTime(n, temp);
  printf("Process\t\tBurst Time\t\tTurnaround Time\t\tWaiting Time\n");
  while (temp != NULL)
  {
    printf("%i\t\t%i\t\t\t%i\t\t\t%i\n", temp->processID, temp->Bursttime, temp->turnaroundTime, temp->waitingTime);
    temp = temp->next;
  }
  avg_wait_FCFS = avg_wait;
  avg_turn_FCFS = avg_turn;
  printf("\nAverage turnaround time: %.2f", avg_turn);
  printf("\nAverage waiting time: %.2f\n", avg_wait);
  printf("------------------------------------------------------------------------------------\n");
}

// calculate the waiting time of each process for SJN
void waitingTime(struct node *start)
{
  int sum = 0;
  start->waitingTime = 0;
  start = start->next;
  while (start != NULL)
  {
    sum += start->prev->Bursttime;
    start->waitingTime = sum;
    start = start->next;
  }
}

// calculate the turn around time for each process for SJN
void TurnaroundTime(struct node *start)
{
  int sum = 0;
  while (start != NULL)
  {
    sum += start->Bursttime;
    start->turnaroundTime = sum;
    start = start->next;
  }
}

// sort the process ID in ascending order to be print accordingly for SJN
void IDsort(struct node *link)
{
  int swapped, i;
  struct node *Forsort;
  if (head == NULL)
    return;
  do
  {
    swapped = 0;
    Forsort = head;
    while (Forsort->next != NULL)
    {
      if (Forsort->processID > Forsort->next->processID)
      {
        int temp1 = Forsort->Bursttime;
        Forsort->Bursttime = Forsort->next->Bursttime;
        Forsort->next->Bursttime = temp1;
        int temp2 = Forsort->waitingTime;
        Forsort->waitingTime = Forsort->next->waitingTime;
        Forsort->next->waitingTime = temp2;
        int temp3 = Forsort->turnaroundTime;
        Forsort->turnaroundTime = Forsort->next->turnaroundTime;
        Forsort->next->turnaroundTime = temp3;
        int temp4 = Forsort->processID;
        Forsort->processID = Forsort->next->processID;
        Forsort->next->processID = temp4;
        swapped = 1;
      }
      Forsort = Forsort->next;
    }
  } while (swapped);
  link = Forsort;
}

/* *********************************************** Round Robin (RR) *********************************************** */

// calculate waiting time and turnaround time for each process in RR
void algorithm_RR(int process[], int n, int burst[], int wait[], int turnaround[], int timequantum)
{
  int current_burst[n]; //remainder burst time
  int done; //flag to check if process is complete or not

  //copy the initial burst time for every process
  for (int i = 0; i < n; i++)
  {
    current_burst[i] = burst[i];
  }

  //initialize current time (arrival time) to 0
  int current_time = 0;

  do
  {
    done = 1;
    //for every process (starting from process 1)
    for (int i = 0; i < n; i++)
    {
      //if still have remainder burst time (process not completed)
      if (current_burst[i] > 0)
      {
        done = 0;
        //if current proccess' burst time > time quantum
        if (current_burst[i] > timequantum)
        {
          current_time = current_time + timequantum;
          current_burst[i] -= timequantum;
        }
        //if current proccess' burst time < time quantum
        else
        {
          current_time = current_time + current_burst[i];
          turnaround[i] = current_time;
          wait[i] = turnaround[i] - burst[i];
          current_burst[i] = 0;
        }
      }
    }
    //if every process have done, break the loop
    if (done == 1)
    {
      break;
    }
  } while (true);
}

// calculate average waiting time and turnaround time  in RR and print to screen
void avgtime_RR(int process[], int n, int burst[], int timequantum)
{
  int wait[n], turn[n], turnaround[n];
  int total_wait = 0;
  int total_turn = 0;
  //call algorithm_RR to complete round robin algorithm
  algorithm_RR(process, n, burst, wait, turnaround, timequantum);

  printf("Process\t\tBurst Time\t\tTurnaround Time\t\tWaiting Time\n");
  //print burst time, turnaround time, waiting time for each process
  for (int j = 0; j < n; j++)
  {
    total_wait = total_wait + wait[j];
    total_turn = total_turn + turnaround[j];
    printf("%i\t\t%i\t\t\t%i\t\t\t%i\n", j + 1, burst[j], turnaround[j], wait[j]);
  }
  //calculate and print average waiting time and average turnaround time
  float avg_wait = (float)total_wait / n;
  float avg_turn = (float)total_turn / n;
  avg_wait_RR = avg_wait;
  avg_turn_RR = avg_turn;
  printf("\nAverage turnaround time: %.2f", avg_turn);
  printf("\nAverage waiting time: %.2f\n", avg_wait);
  printf("------------------------------------------------------------------------------------\n");
}

// function used for comparison to find the best one according to the chosen comparison algorithm
void comparison()
{
  float min_wait;
  float min_turn;
  float min_wait_logic;
  float min_turn_logic;
  char best_wait[50];
  char best_turn[50];
  char best_logic[50];
  char best[50];
  float SJN = avg_wait_FCFS + avg_turn_FCFS;
  float RR = avg_wait_RR + avg_turn_RR;
  float min;
  int selection;
  printf("Please choose the algorithm that you want to choose the best algorithm that suit your process:\n1.Average Waiting Time + Average Turn Around Time\n2.Best Average Waiting Time\n3.Best Average Turn Around Time\n");
  printf("What is your choice: ");
  scanf("%i", &selection);
  switch (selection)
  {
  case 1:
  {
    if (SJN < RR)
    {
      min = SJN;
      strcpy(best, "Shortest Job Next");
      printf("\n\nThe algorithm that have the shortest total average waiting time and turn around time is %s with time %.2f microseconds\n\n", best, min);
    }
    else if (RR < SJN)
    {
      min = RR;
      strcpy(best, "Round Robin Without Overhead");
      printf("\n\nThe algorithm that have the shortest total average waiting time and turn around time is %s with time %.2f microseconds\n\n", best, min);
    }
    break;
  }
  case 2:
  {
    if (avg_wait_FCFS < avg_wait_RR)
    {
      min_wait = avg_wait_FCFS;
      strcpy(best_wait, "Shortest Job Next");
      printf("\n\nThe algorithm that have the shortest average waiting time is %s with time %.2f microseconds\n\n", best_wait, min_wait);
    }
    else
    {
      min_wait = avg_wait_RR;
      strcpy(best_wait, "Round Robin");
      printf("\n\nThe algorithm that have the shortest average waiting time is %s with time %.2f microseconds\n\n", best_wait, min_wait);
    }
    break;
  }
  case 3:
  {
    if (avg_turn_FCFS < avg_turn_RR && avg_turn_FCFS < avg_turn_RR)
    {
      min_turn = avg_turn_FCFS;
      strcpy(best_turn, "Shortest Job Next");
      printf("\n\nThe algorithm that have the shortest average turn around time is %s with time %.2f microseconds\n\n", best_turn, min_turn);
    }
    else
    {
      min_turn = avg_turn_RR;
      strcpy(best_turn, "Round Robin Without Overhead");
      printf("\n\nThe algorithm that have the shortest average turn around time is %s with time %.2f microseconds\n\n", best_turn, min_turn);
    }
    break;
  }
  default:
  {
    printf("Invalid");
  }
  }
}

//check whether the input is number
int checkInput()
{
  int temp = 0;
  while (scanf("%d", &temp) != 1)
  {
    printf("Please enter valid input: ");
    scanf("%*s");
  }
  return temp;
}

//check whether the time quantum is valid or not (10-100)
bool checkTimeQuantum(int temp)
{
  if (temp < 10 || temp > 100)
  {
    printf("Time quantum should be 10-100ms.\n");
    printf("Please enter new time quantum: ");
    return false;
  }
  return true;
}
