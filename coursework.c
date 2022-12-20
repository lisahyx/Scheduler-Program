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
float total_wt;
float total_tat;
float avg_turn_FCFS;
float avg_wait_RR;
float avg_turn_RR;

// functions for input validation
int checkInput();
bool checkTimeQuantum(int input);

// functions for FCFS
void findWT(int processes[], int n, int bt[], int wt[]);
void findTAT(int processes[], int n, int bt[], int wt[], int tat[]);
void printFCFS(int processes[], int n, int bt[]);
float avgWaitTime(int processes[], int n, int bt[], int wt[]);
float avgTurnTime(int processes[], int n, int bt[], int wt[], int tat[]);

// functions for RR
void avgtime_RR(int process[], int n, int burst[], int timequantum);
void algorithm_RR(int process[], int n, int burst[], int wait[], int turnaround[], int timequantum);

// function for comparison
void comparison();

// main method
int main(void)
{
  int n;

  // PROMPT USER INPUT
  printf("Enter total number of processes: ");
  n = checkInput();

  int process[n - 1];
  int burst[n - 1];
  int timequantum;
  int overhead;
  int id_overhead = 0;

  // PROMPT USER INPUT
  for (int i = 0; i < n; i++)
  {
    printf("Enter burst time for process[%d] : ", i + 1);
    burst[i] = checkInput();
  }

  printf("Enter Time Quantum for Round Robin: ");
  do
  {
    timequantum = checkInput();
  } while (!checkTimeQuantum(timequantum));

  system("cls");

  printf("Algorithm for First Come First Serve (FCFS):\n\n");
  printFCFS(process, n, burst);
  printf("\n");

  printf("Algorithm for Round Robin (RR):\n\n");
  avgtime_RR(process, n, burst, timequantum);
  printf("\n");

  comparison();
  return 0;
}

/* ****************************************** First Come First Serve (FCFS) ****************************************** */

// n = size of the array, bt = Burst Time, wt = Waiting Time
// calculate waiting time for each process in FCFS
void findWT(int processes[], int n, int bt[], int wt[])
{
  // set the first process's wt to 0
  wt[0] = 0;

  // calculation for wt
  for (int i = 1; i < n; i++)
  {
    wt[i] = bt[i - 1] + wt[i - 1];
    total_wt = total_wt + wt[i];
  }
}

// formula TAT = WT + BT
// calculate turnaround time for each process in FCFS
void findTAT(int processes[], int n, int bt[], int wt[], int tat[])
{

  // calculating turnaround time by adding wt and bt
  for (int i = 0; i < n; i++)
  {
    tat[i] = bt[i] + wt[i];
    total_tat = total_tat + tat[i];
  }
}

// print FCFS
void printFCFS(int processes[], int n, int bt[])
{
  int wt[n], tat[n], total_tat = 0, total_wt = 0;

  findWT(processes, n, bt, wt);
  // function to find turnaround time for all processes
  findTAT(processes, n, bt, wt, tat);

  // display processes along with all details
  printf("Process\t\tBurst Time\t\tTurnaround Time\t\tWaiting Time\n");

  // print all bt, wt, tat out
  for (int i = 0; i < n; i++)
  {
    total_tat = total_tat + tat[i];
    total_wt = total_wt + wt[i];
    printf("%i\t\t%i\t\t\t%i\t\t\t%i\n", (i + 1), bt[i], wt[i], tat[i]);
  }

  float t = (float)total_tat / n;
  float w = (float)total_wt / n;
  avg_turn_FCFS = t;
  avg_wait_FCFS = w;
  printf("\nAverage turnaround time: %.2f", t);
  printf("\nAverage waiting time: %.2f\n", w);
  printf("------------------------------------------------------------------------------------\n");
}

/* *********************************************** Round Robin (RR) *********************************************** */

// calculate waiting time and turnaround time for each process in RR
void algorithm_RR(int process[], int n, int burst[], int wait[], int turnaround[], int timequantum)
{
  int current_burst[n]; // remainder burst time
  int done;             // flag to check if process is complete or not

  // copy the initial burst time for every process
  for (int i = 0; i < n; i++)
  {
    current_burst[i] = burst[i];
  }

  // initialize current time (arrival time) to 0
  int current_time = 0;

  do
  {
    done = 1;
    // for every process (starting from process 1)
    for (int i = 0; i < n; i++)
    {
      // if still have remainder burst time (process not completed)
      if (current_burst[i] > 0)
      {
        done = 0;
        // if current proccess' burst time > time quantum
        if (current_burst[i] > timequantum)
        {
          current_time = current_time + timequantum;
          current_burst[i] -= timequantum;
        }
        // if current proccess' burst time < time quantum
        else
        {
          current_time = current_time + current_burst[i];
          turnaround[i] = current_time;
          wait[i] = turnaround[i] - burst[i];
          current_burst[i] = 0;
        }
      }
    }
    // if every process have done, break the loop
    if (done == 1)
    {
      break;
    }
  } while (true);
}

// calculate average waiting time and turnaround time in RR and print to screen
void avgtime_RR(int process[], int n, int burst[], int timequantum)
{
  int wait[n], turn[n], turnaround[n];
  int total_wait = 0;
  int total_turn = 0;
  // call algorithm_RR to complete round robin algorithm
  algorithm_RR(process, n, burst, wait, turnaround, timequantum);

  printf("Process\t\tBurst Time\t\tTurnaround Time\t\tWaiting Time\n");
  // print burst time, turnaround time, waiting time for each process
  for (int j = 0; j < n; j++)
  {
    total_wait = total_wait + wait[j];
    total_turn = total_turn + turnaround[j];
    printf("%i\t\t%i\t\t\t%i\t\t\t%i\n", j + 1, burst[j], turnaround[j], wait[j]);
  }
  // calculate and print average waiting time and average turnaround time
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
  float FCFS = avg_wait_FCFS + avg_turn_FCFS;
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
      if (FCFS < RR)
      {
        min = FCFS;
        strcpy(best, "First Come First Serve");
        printf("\n\nThe algorithm that have the shortest total average waiting time and turnaround time is %s with time %.2f microseconds.\n\n", best, min);
      }
      else if (RR < FCFS)
      {
        min = RR;
        strcpy(best, "Round Robin Without Overhead");
        printf("\n\nThe algorithm that have the shortest total average waiting time and turnaround time is %s with time %.2f microseconds.\n\n", best, min);
      }
      else 
      {
        min = FCFS;
        printf("\n\nBoth algorithms have the same total average waiting time and turnaround time, which is %.2f microseconds.\n\n", min);
      }
      break;
    }
    case 2:
    {
      if (avg_wait_FCFS < avg_wait_RR)
      {
        min_wait = avg_wait_FCFS;
        strcpy(best_wait, "First Come First Serve");
        printf("\n\nThe algorithm that have the shortest average waiting time is %s with time %.2f microseconds.\n\n", best_wait, min_wait);
      }
      else if (avg_wait_RR < avg_wait_FCFS)
      {
        min_wait = avg_wait_RR;
        strcpy(best_wait, "Round Robin");
        printf("\n\nThe algorithm that have the shortest average waiting time is %s with time %.2f microseconds.\n\n", best_wait, min_wait);
      } 
      else
      {
        min_wait = avg_wait_FCFS;
        printf("\n\nBoth algorithms have the same average waiting time, which is %.2f microseconds.\n\n", min_wait);
      }
      break;
    }
    case 3:
    {
      if (avg_turn_FCFS < avg_turn_RR)
      {
        min_turn = avg_turn_FCFS;
        strcpy(best_turn, "First Come First Serve");
        printf("\n\nThe algorithm that have the shortest average turnaround time is %s with time %.2f microseconds.\n\n", best_turn, min_turn);
      }
      else if (avg_turn_RR < avg_turn_FCFS)
      {
        min_turn = avg_turn_RR;
        strcpy(best_turn, "Round Robin Without Overhead");
        printf("\n\nThe algorithm that have the shortest average turnaround time is %s with time %.2f microseconds.\n\n", best_turn, min_turn);
      }
      else
      {
        min_turn = avg_turn_FCFS;
        printf("\n\nBoth algorithms have the same average waiting time, which is %.2f microseconds.\n\n", min_turn);
      }
      break;
    }
    default:
    {
      printf("Invalid");
    }
  }
}

// check whether the input is number
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

// check whether the time quantum is valid or not (10-100)
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