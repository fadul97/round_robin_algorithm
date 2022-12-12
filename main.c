#include <stdio.h>
#include <stdbool.h>

//#define defer_and_return(value) do {result = (value); goto defer; } while(0)
#define MAX 20

typedef enum
{
    OK,
    ERROR_READ,
    ERROR_WRITE
} error_list;

typedef struct
{
    int creation_time;
    int duration;
    int priority;
    bool completed;    
} Process;

// Functions definitions
void print_process_list(Process *vet);
int open_and_read_file(FILE *f, Process *vet);
void bubble_sort(Process *vet);


// Main
int main(int argc, char **argv)
{
    error_list result = OK;
    FILE *file;
    Process processes[MAX];

    result = open_and_read_file(file, processes);
    if(result != 0) return result;
  
    //print_process_list(processes);

    // Sorting Algorithm
    bubble_sort(processes);
    print_process_list(processes);

    // TODO: Implement Preemptive Scheduling
    int executed = 0;
    int quantum = 3;
    int count = 0;
    int end_time = 0;
    int start_time = 0;

    while(executed < MAX)
    {
        start_time = end_time;
        if(processes[count].completed == false)
        {
            for(int i = 0; i < quantum; i++)
            {
                if(processes[count].duration > 0)
                    processes[count].duration--;
                else{
                    //processes[count].completed = true;
                    break;
                }

                end_time++;    
            }
            printf("%d-%d: processo %d\n", start_time, end_time, count);

            if((processes[count].duration<=0)&&(processes[count].completed==false))
            {
                printf("Processo concluido: %d\n", count);
                executed++;
                processes[count].completed = true;
            }
        }
        
        if(count >= MAX-1)
            count = 0;
        else
            count++;
    }

    printf("Completed = %d\n", executed);
    print_process_list(processes);
    
    return result;
}

// Functions implementations
void print_process_list(Process *vet)
{
    printf("========== LIST OF PROCESSES ==========\n");
    printf("Creation Time    duration  Priority\n");
    for(int i = 0; i < MAX; i++)
        printf("      %2d           %2d       %2d\n", vet[i].creation_time, vet[i].duration,
            vet[i].priority);
}

int open_and_read_file(FILE *f, Process *vet)
{
    f = fopen("processos.txt", "r");
    
    if(f == NULL)
        return ERROR_READ;

    int v;
    for(int i = 0; i < MAX; i++)
    {
        v = fscanf(f, "%d %d %d", &vet[i].creation_time, &vet[i].duration,
            &vet[i].priority);
        vet[i].completed = false;
    }
    if(f) fclose(f);
    return OK;
}

void bubble_sort(Process *vet)
{
    Process aux;
    for(int i = 0; i < MAX; i++)
    {
        for(int j = 0; j < MAX-1; j++)
        {
            if(vet[j].creation_time > vet[j+1].creation_time)
            {
                aux = vet[j];
                vet[j] = vet[j+1];
                vet[j+1] = aux;
            } 
        }
    }
}
