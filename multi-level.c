#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// functions for input validation
int checkInput();
bool checkTimeQuantum(int input);

struct process
{
    char name;
    int AT, BT, WT, TAT, RT, CT;
} Q1[10], Q2[10];/*Two queues*/

int n;

void sortByArrival()
{
    struct process temp;
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (Q1[i].AT > Q1[j].AT)
            {
                temp = Q1[i];
                Q1[i] = Q1[j];
                Q1[j] = temp;
            }
        }
    }
}


int main()
{
    int i ,j, k=0, r=0, time=0, flag=0;
    char c;

    // PROMPT USER INPUT
    printf("Enter total number of processes: ");
    n = checkInput();

    int process[n - 1];
    int burst[n - 1];
    int timequantum;
    int overhead;
    int id_overhead = 0;

    // PROMPT USER INPUT
    for (int i = 0, c = 'A'; i < n; i++, c++)
    {
        Q1[i].name = c;
        printf("Enter the arrival time: ");
        scanf("%d", &Q1[i].AT);
        printf("Enter burst time for process[%d] : ", i + 1);
        burst[i] = checkInput();
        Q1[i].BT = burst[i];
        Q1[i].RT = Q1[i].BT; /*save burst time in remaining time for each process*/
    }

    printf("Enter Time Quantum for Round Robin: ");
    do
    {
        timequantum = checkInput();
    } while (!checkTimeQuantum(timequantum));

    sortByArrival();
    time = Q1[0].AT;

    printf("Process in first queue following RR");
    printf("\nProcess\t\tRT\t\tWT\t\tTAT\t\t");

    for(i=0; i<n; i++)
    {
        if(Q1[i].RT <= timequantum)
        {
            time += Q1[i].RT;/*from arrival time of first process to completion of this process*/
            Q1[i].RT=0;
            Q1[i].WT=time-Q1[i].AT-Q1[i].BT;/*amount of time process has been waiting in the first queue*/
            Q1[i].TAT=time-Q1[i].AT;/*amount of time to execute the process*/
            printf("\n%c\t\t%d\t\t%d\t\t%d",Q1[i].name,Q1[i].BT,Q1[i].WT,Q1[i].TAT);
        }
        else/*process moves to queue 2*/
        {
            Q2[k].WT=time;
            time += timequantum;
            Q1[i].RT -= timequantum;
            Q2[k].BT=Q1[i].RT;
            Q2[k].RT=Q2[k].BT;
            Q2[k].name=Q1[i].name;
            k=k+1;
            flag=1;
        }
    }

    if(flag==1)
    {
        printf("\nProcess in second queue following FCFS");
        printf("\nProcess\t\tRT\t\tWT\t\tTAT\t\t");
    }

    for(i=0;i<k;i++)
    {
        /*process moves to queue 2 with FCFS*/
        Q2[r].AT=time;
        time += timequantum;
        Q2[i].RT -= timequantum;
        Q2[r].BT = Q2[i].RT;
        Q2[r].RT = Q2[r].BT;
        Q2[r].name=Q2[i].name;
        r=r+1;
        flag=2;
    }

    for(i=0;i<r;i++)
    {
        if(i==0)
                Q2[i].CT = Q2[i].BT+time - timequantum;
            else
                Q2[i].CT = Q2[i-1].CT+Q2[i].BT;
    }

    for(i=0;i<r;i++)
    {
        Q2[i].TAT=Q2[i].CT;
        Q2[i].WT=Q2[i].TAT-Q2[i].BT;
        printf("\n%c\t\t%d\t\t%d\t\t%d\t\t",Q2[i].name,Q2[i].BT,Q2[i].WT,Q2[i].TAT);
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