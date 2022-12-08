#include <stdio.h>

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
  int length;
  int priority;
} Process;

// Functions definitions
void print_process_list(Process *vet);
int open_and_read_file(FILE *f, Process *vet);


// Main
int main(int argc, char **argv)
{
    error_list result = OK;
    FILE *file;
    Process processes[MAX];

    result = open_and_read_file(file, processes);
    if(result != 0) return result;
  
    print_process_list(processes);

    // TODO: Sorting Algorithm
    // TODO: Implement Preemptive Scheduling
    /*
    for(int i = 0; i < MAX; i++)
    {    
        for(int j = 0; j < 3; j++)
            if(processes[i].length > 0)
                processes[i].length--;
    }
    */
    
    return result;
}

// Functions implementations
void print_process_list(Process *vet)
{
    printf("========== LIST OF PROCESSES ==========\n");
    printf("Creation Time    Length  Priority\n");
    for(int i = 0; i < MAX; i++)
        printf("      %2d           %2d       %2d\n", vet[i].creation_time, vet[i].length,
            vet[i].priority);
}

int open_and_read_file(FILE *f, Process *vet)
{
    f = fopen("processos.txt", "r");
    
    if(f == NULL)
        return ERROR_READ;

    int v;
    for(int i = 0; i < MAX; i++)
        v = fscanf(f, "%d %d %d", &vet[i].creation_time, &vet[i].length,
            &vet[i].priority);

    if(f) fclose(f);
    return OK;
}
