#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100


struct process {
    int quantum_time;
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int TAT;
    int waiting_time;
    int is_completed;
    int completion_time;
};
int comparefcfs(const void *a, const void *b)
{
return ((struct process *)a)->arrival_time - ((struct process *)b)->arrival_time;
}
void FCFS()
{
FILE *input_file = fopen("FCFS.txt", "r");
FILE *output_file = fopen("FCFS_OUTPUT.txt", "w");
int num_processes;
fscanf(input_file, "%d", &num_processes);
struct process *processes = (struct Process *)malloc(num_processes *
sizeof(struct process));
for (int i = 0; i < num_processes; i++)
{
fscanf(input_file, "%d %d %d", &processes[i].pid,
&processes[i].arrival_time, &processes[i].burst_time);
}
fclose(input_file);
qsort(processes, num_processes, sizeof(struct process), comparefcfs);
int current_time = 0;
for (int i = 0; i < num_processes; i++)
{
if (processes[i].arrival_time > current_time)
{
current_time = processes[i].arrival_time;
}
processes[i].completion_time = current_time + processes[i].burst_time;
processes[i].waiting_time = current_time - processes[i].arrival_time;
if (processes[i].waiting_time < 0)
{
processes[i].waiting_time = 0;
}
current_time = processes[i].completion_time;
processes[i].TAT = processes[i].waiting_time +
processes[i].burst_time;
}
float avg_waiting_time = 0, avg_turnaround_time = 0;
for (int i = 0; i < num_processes; i++)
{
avg_waiting_time += processes[i].waiting_time;
avg_turnaround_time += processes[i].TAT;
}
avg_waiting_time /= num_processes;
avg_turnaround_time /= num_processes;
fprintf(output_file, "Average turnaround time: %.2f\n",
avg_turnaround_time);
fprintf(output_file, "Average waiting time: %.2f\n", avg_waiting_time);
fprintf(output_file, "Gantt Chart:\n");
for (int i = 0; i < num_processes; i++)
{
for (int j = 0; j < processes[i].burst_time; j++)
{
fprintf(output_file, "P%d |", processes[i].pid);
}
}
fprintf(output_file, "\n");
fclose(output_file);
free(processes);
return;
}
int compareps(const void *p1, const void *p2){
    struct process *a = (struct process *)p1;
    struct process *b = (struct process *)p2;
if (a->priority < b->priority)
{
return -1;
}
else if (a->priority == b->priority)
{
return 0;
}
else
{
return 1;
}
}
void priority()
{
  FILE *input_file, *output_file;
input_file = fopen("Priority.txt", "r");
output_file = fopen("PRIORITY_OUTPUT.txt", "w");
// Read input from the file
int num_processes;
fscanf(input_file, "%d", &num_processes);
// Create an array of processes and initialize them
struct process processes[num_processes];
for (int i = 0; i < num_processes; i++)
{
fscanf(input_file, "%d %d %d %d", &processes[i].pid,
&processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority);
processes[i].remaining_time = processes[i].burst_time;
}
// Sort the processes based on their priority
qsort(processes, num_processes, sizeof(struct process), compareps);
int current_time = 0;
int completed_processes = 0;
// Create an array to store the order of execution
int execution_order[1000], num_executions = 0;
// Run the Priority Scheduling algorithm
while (completed_processes < num_processes)
{
int highest_priority_index = -1;
int highest_priority = -1;
for (int i = 0; i < num_processes; i++)
{
if (processes[i].arrival_time <= current_time &&
processes[i].remaining_time > 0)
{
if (processes[i].priority > highest_priority)
{
highest_priority_index = i;
highest_priority = processes[i].priority;
}
}
}
if (highest_priority_index == -1)
{
current_time++;
execution_order[num_executions++] = -1;
}
else
{
current_time += processes[highest_priority_index].remaining_time;
processes[highest_priority_index].remaining_time = 0;
processes[highest_priority_index].completion_time = current_time;
completed_processes++;
execution_order[num_executions++] =
processes[highest_priority_index].pid;
}
}
// Calculate turnaround time and waiting time for each process
int total_turnaround_time = 0, total_waiting_time = 0;
for (int i = 0; i < num_processes; i++)
{
processes[i].TAT= processes[i].completion_time -
processes[i].arrival_time;
processes[i].waiting_time = processes[i].TAT -
processes[i].burst_time;
total_turnaround_time += processes[i].TAT;
total_waiting_time += processes[i].waiting_time;
}
// Print the average turnaround time and waiting time
float avg_turnaround_time = (float)total_turnaround_time / num_processes;
float avg_waiting_time = (float)total_waiting_time / num_processes;
fprintf(output_file, "Average turnaround time: %.2f\n",
avg_turnaround_time);
fprintf(output_file, "Average waiting time: %.2f\n", avg_waiting_time);
// Print the Gantt Chart
fprintf(output_file, "Gantt Chart:\n");
for (int i = 0; i < num_processes; i++)
{
for (int j = 0; j < processes[i].burst_time; j++)
{
fprintf(output_file, "P%d |", processes[i].pid);
}
}
fprintf(output_file, "\n");
fclose(output_file);
return;
}

void RR()

{
    FILE *input_file, *output_file;
    input_file = fopen("RoundRobin.txt", "r");
    output_file = fopen("RoundRobinoutput.txt", "w");
// Read input from the file
    int num_processes, quantum_time;
    fscanf(input_file, "%d %d", &num_processes, &quantum_time);
// Create an array of processes and initialize them
    struct process processes[num_processes];
    for (int i = 0; i < num_processes; i++)
{
        fscanf(input_file, "%d %d %d", &processes[i].pid,
        &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
}
    int current_time = 0;
    int completed_processes = 0;
// Create an array to store the order of execution
    int execution_order[1000], num_executions = 0;
    while (completed_processes < num_processes)
{
        int flag = 0;
        for (int i = 0; i < num_processes; i++)
{
            if (processes[i].remaining_time > 0 && processes[i].arrival_time<= current_time)
{
                flag = 1;
                if (processes[i].remaining_time <= quantum_time)
{
                    current_time += processes[i].remaining_time;
                    processes[i].remaining_time = 0;
                    processes[i].completion_time = current_time;
                    completed_processes++;
                    execution_order[num_executions++]=processes[i].pid;
}
else
{
current_time += quantum_time;
processes[i].remaining_time -= quantum_time;
execution_order[num_executions++] =processes[i].pid;
}
}
}
if (flag == 0)
{
current_time++;
execution_order[num_executions++] = -1;
}
}
// Calculate turnaround time and waiting time for each process
int total_turnaround_time = 0, total_waiting_time = 0;
for (int i = 0; i < num_processes; i++)
{
processes[i].TAT = processes[i].completion_time -processes[i].arrival_time;
processes[i].waiting_time = processes[i].TAT -
processes[i].burst_time;
total_turnaround_time += processes[i].TAT;
total_waiting_time += processes[i].waiting_time;
}
// Print the average turnaround time and waiting time
float avg_turnaround_time = (float)total_turnaround_time / num_processes;
float avg_waiting_time = (float)total_waiting_time / num_processes;
fprintf(output_file, "Average turnaround time: %.2f\n",
avg_turnaround_time);
fprintf(output_file, "Average waiting time: %.2f\n", avg_waiting_time);
// Print the Gantt Chart
fprintf(output_file, "Gantt Chart:\n");
for (int i = 0; i < num_executions; i++)
{
if (execution_order[i] == -1)
{
fprintf(output_file, "--");
}
else
{
fprintf(output_file, "P%d |", execution_order[i]);
}
fprintf(output_file, " ");
}
fprintf(output_file, "\n");
fclose(output_file);

return;
}
// Structure to represent a process
void SJF(){
    int n, time = 0, smallest, completed = 0, total_turnaround_time = 0, total_waiting_time = 0;
    struct process processes[MAX_PROCESSES], temp;
    FILE *fp;

    fp = fopen("SJB.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fscanf(fp, "%d", &n);

    for (int i = 0; i < n; i++) {
        processes[i].pid = i+1;
        fscanf(fp, "%d %d", &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].is_completed = 0;
    }

    fclose(fp);
    fp = fopen("SJBoutput.txt", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    // Sort processes by arrival time
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    // Run SJF algorithm
    while (completed != n) {
        smallest = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].is_completed == 0) {
                if (smallest == -1 || processes[i].burst_time < processes[smallest].burst_time) {
                    smallest = i;
                }
            }
        }


        if (smallest == -1) {
            time++;
        } else {
            processes[smallest].completion_time = time + processes[smallest].burst_time;
            processes[smallest].TAT = processes[smallest].completion_time - processes[smallest].arrival_time;
            processes[smallest].waiting_time = processes[smallest].TAT - processes[smallest].burst_time;
            total_turnaround_time += processes[smallest].TAT;
            total_waiting_time += processes[smallest].waiting_time;
            processes[smallest].is_completed = 1;
            completed++;
            time = processes[smallest].completion_time;
        for (int j = 0; j < processes[smallest].burst_time; j++) {
            fprintf(fp,"P%d ", processes[smallest].pid);
        }
        }
    }
    fprintf(fp,"\n");
    // Print results
    fprintf(fp,"Average turn-around time: %.2f\n", (float)total_turnaround_time / n);
    fprintf(fp,"Average waiting time: %.2f\n", (float)total_waiting_time / n);

    // printf("Gantt Chart:\n");
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < processes[i].burst_time; j++) {
    //         printf("P%d ", processes[i].pid);
    //     }
    // }
    printf("\n");
    fclose(fp);
}

void SRJF(){

    int n, time = 0, smallest, completed = 0, total_turnaround_time = 0, total_waiting_time = 0;
    struct process processes[MAX_PROCESSES], temp;
    FILE *fp;

    fp = fopen("SRJF.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d %d %d",&processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time=processes[i].burst_time;
        //  printf("%d:\n",processes[i].remaining_time);
        processes[i].is_completed = 0;
    }

    fclose(fp);
    fp = fopen("SRJFoutput.txt", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int gantt_chart[MAX_PROCESSES*2];
    fprintf(fp," Gnatt Chart: \n");
    fprintf(fp," PID\tBurst Time\tArrival Time\tTAT\tWaiting Time\n");
    // Sort processes by arrival time
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    // Run SJF algorithm
    while (completed != n) {
        smallest = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].is_completed == 0) {
                if (smallest == -1 || processes[i].remaining_time < processes[smallest].remaining_time) {
                    smallest = i;
                }
            }

        }


        if (smallest == -1) {
            time++;
        } else {
            processes[smallest].remaining_time-=1;
            if(processes[smallest].remaining_time==0)
            {
                completed++;
                processes[smallest].is_completed=1;
                processes[smallest].completion_time = (time+1);
                processes[smallest].TAT = processes[smallest].completion_time - processes[smallest].arrival_time;
                processes[smallest].waiting_time = processes[smallest].TAT - processes[smallest].burst_time;
                printf("%d:%d:%d\n",processes[smallest].pid,processes[smallest].completion_time,time);
                total_turnaround_time += processes[smallest].TAT;
                total_waiting_time += processes[smallest].waiting_time;
                fprintf(fp,"%d\t\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d\n",processes[smallest].pid,processes[smallest].burst_time,processes[smallest].arrival_time,processes[smallest].TAT,processes[smallest].waiting_time);
            }
            gantt_chart[time]=processes[smallest].pid;
            time++;
        }
    }
    // fprintf(fp,"\n");
    // Print results
    fprintf(fp,"Average turn-around time: %.2f\n", (float)total_turnaround_time / n);
    fprintf(fp,"Average waiting time: %.2f\n", (float)total_waiting_time / n);

    printf("Gantt Chart:\n");

    for (int j = 0; j <time; j++) {
        fprintf(fp,"P%d \n", gantt_chart[j]);
    }

    printf("\n");
    fclose(fp);

}

void LRJF(){
    int n, time = 0, largest, completed = 0, total_turnaround_time = 0, total_waiting_time = 0;
    struct process processes[MAX_PROCESSES], temp;
    FILE *fp;

    fp = fopen("LRTF.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d %d %d",&processes[i].pid, &processes[i].arrival_time, &processes[i].burst_time);
        processes[i].remaining_time=processes[i].burst_time;
        //  printf("%d:\n",processes[i].remaining_time);
        processes[i].is_completed = 0;
    }

    fclose(fp);
    fp = fopen("LRTFoutput.txt", "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }
    int gantt_chart[MAX_PROCESSES*2];
    fprintf(fp," PID\tBurst Time\tArrival Time\tTAT\tWaiting Time\n");
    // Sort processes by arrival time
    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[i].arrival_time > processes[j].arrival_time) {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    // Run SJF algorithm
    while (completed != n) {
        largest = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].is_completed == 0) {
                if (largest == -1 || processes[i].remaining_time > processes[largest].remaining_time) {
                    largest = i;
                }
            }

        }


        if (largest == -1) {
            time++;
        } else {
            processes[largest].remaining_time-=1;
            if(processes[largest].remaining_time==0)
            {
                completed++;
                processes[largest].is_completed=1;
                processes[largest].completion_time = (time+1);
                processes[largest].TAT = processes[largest].completion_time - processes[largest].arrival_time;
                processes[largest].waiting_time = processes[largest].TAT - processes[largest].burst_time;
                printf("%d:%d:%d\n",processes[largest].pid,processes[largest].completion_time,time);
                total_turnaround_time += processes[largest].TAT;
                total_waiting_time += processes[largest].waiting_time;
                fprintf(fp,"%d\t\t\t%d\t\t\t%d\t\t\t%d\t\t\t%d\n",processes[largest].pid,processes[largest].burst_time,processes[largest].arrival_time,processes[largest].TAT,processes[largest].waiting_time);
            }
            gantt_chart[time]=processes[largest].pid;
            time++;
        }
    }
    // fprintf(fp,"\n");
    // Print results
    fprintf(fp,"Average turn-around time: %.2f\n", (float)total_turnaround_time / n);
    fprintf(fp,"Average waiting time: %.2f\n", (float)total_waiting_time / n);

    printf("Gantt Chart:\n");

    for (int j = 0; j <time; j++) {
        fprintf(fp,"P%d \n", gantt_chart[j]);
    }

    printf("\n");
    fclose(fp);
}


int main() {
    int a;
    do
    {
        printf("\nEnter the algorithm you want to perform :\n");
        printf("\t1. First Come First Serve\n");
        printf("\t2. Shortest Job First\n");
        printf("\t3. Round Robin \n");
        printf("\t4. Priority\n");
        printf("\t5. Shortest Remaining Job First\n");
        printf("\t6. Longest Remaining Job First\n");
        scanf("%d",&a);
        switch(a)
        {
            case 1:
            FCFS();
            break;
            case 2:
            SJF();
            break;
            case 3:
            RR();
            break;
            case 4:
            priority();
            break;
            case 5:
            SRJF();
            break;
            case 6:
            LRJF();
            break;
        }
    }while(a!=0);
}
