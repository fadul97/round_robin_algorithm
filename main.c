#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//#define defer_and_return(value) do {result = (value); goto defer; } while(0)
#define MAX 20

// Enum para erros no retorno de funcoes
typedef enum
{
    OK,
    ERROR_READ,
    ERROR_WRITE
} error_list;

// Struct para processo
typedef struct
{
    int creation_time;
    int duration;
    int priority;
    int waiting_time;
    int last_execution;
    bool completed;    
} Process;


// Linhas para escrever no arquivo de saida
char l[20][130] = {
"##  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  ##  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  ##  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  ##  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  ##  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  ##  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  ##  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  ##  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  ##  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ###  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ###  ---  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ###  ---  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ###  ---  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ###  ---  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ###  ---  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ###  ---  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ###  ---  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ###  ---  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ###  ---",
"--  --  --  --  --  --  --  --  --  ---  ---  ---  ---  ---  ---  ---  ---  ---  ---  ###"};

// Functions definitions
void print_process_list(Process *vet);
int open_and_read_file(FILE *f, Process *vet);
void bubble_sort(Process *vet);
void scheduling(Process *process);


// Main
int main(int argc, char **argv)
{
    // Definindo variaveis
    error_list result = OK;
    FILE *file;
    Process processes[MAX];

    // Ler o arquivo input.txt
    result = open_and_read_file(file, processes);
    if(result != 0) return result;

    // Sorting Algorithm - Ordenar processos na ordem de chegada
    bubble_sort(processes);

    // Escalonar os procesos
    scheduling(processes);
    return result;
}


// Imprimir lista
void print_process_list(Process *vet)
{
    printf("========== LIST OF PROCESSES ==========\n");
    printf("Creation Time    duration  Priority\n");
    for(int i = 0; i < MAX; i++)
        printf("      %2d           %2d       %2d\n", vet[i].creation_time, vet[i].duration,
            vet[i].priority);
}

// Ler arquivo de entrada e armazenar informacoes no vetor
int open_and_read_file(FILE *f, Process *vet)
{
    f = fopen("input.txt", "r");
    
    if(f == NULL)
        return ERROR_READ;

    int v;
    for(int i = 0; i < MAX; i++)
    {
        // Colocar os valores do arquivo nas variaveis do Processo
        v = fscanf(f, "%d %d %d", &vet[i].creation_time, &vet[i].duration,
            &vet[i].priority);
        vet[i].completed = false;
        vet[i].last_execution = vet[i].creation_time;
        vet[i].waiting_time = 0;
    }

    if(f) fclose(f);
    return OK;
}

// Ordenar processos
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

// Escalonar processos
void scheduling(Process *processes){
    int executed = 0;            // Processos executados/concluidos
    int quantum = 3;             // Quantum
    int count = 0;               // Contador para loop
    int switches = 0;            // Numero de trocas de contexto
    int end_time = 0;            // Tempo final na troca de contexto
    int start_time = 0;          // Tempo inicial na troca de contexto
    float life_time = 0.0f;      // Tempo medio de vida

    FILE *file;
    file = fopen("output.txt", "w");

    // Escrever no arquivo a tabela abaixo
    fprintf(file,"========== LIST OF PROCESSES ==========\n");
    fprintf(file,"Creation Time    duration  Priority\n");
    for(int i = 0; i < MAX; i++)
        fprintf(file,"      %2d           %2d       %2d\n", processes[i].creation_time, processes[i].duration,
            processes[i].priority);

    // Diagrama do tempo de execucao
    fprintf(file,"\n\n========== RUNTIME DIAGRAM ==========");
    fprintf(file,"\ntempo  P1  P2  P3  P4  P5  P6  P7  P8  P9  P10  P11  P12  P13  P14  P15  P16  P17  P18  P19  P20\n");
    while(executed < MAX)
    {
        // Reseta o tempo inicial para ser o tempo quando a ultima execucao aconteceu
        start_time = end_time;

        if(processes[count].completed == false)
            processes[count].waiting_time = start_time - processes[count].last_execution;
        
        // Se o processo nao estiver concluido
        if(processes[count].completed == false)
        {
            // Executa o quantum
            for(int i = 0; i < quantum; i++)
            {
                // Se a duracao do processo for maior que 0
                if(processes[count].duration > 0)
                    processes[count].duration--;
                else{
                    break;
                }

                // Incrementa o tempo total de execucao
                end_time++;    
                processes[count].last_execution++;
            }

            // Escrever no arquivo output os tempos de execucao
            fprintf(file,"%2d-%2d: %s\n", start_time, end_time, l[count]);

            // Se processo estiver concluido, incrementar processos concluidos
            if((processes[count].duration <= 0) && (processes[count].completed == false))
            {
                executed++;
                processes[count].completed = true;
            }
        }
        
        // Resetar contador para checar processos concluidos
        if(count >= MAX-1)
            count = 0;
        else
            count++;

        // Incrementar quantidade de trocas de contexto
        if(processes[count].completed == false) 
            switches++;
    }

    // Calcular tempo medio de espera
    float sum = 0.0f;
    for(int i = 0; i < MAX; i++)
        sum += processes[i].waiting_time;
    sum = sum / MAX;

    
    life_time = end_time / MAX;
    fprintf(file,"\nContext Switches: %d\n", ++switches);
    fprintf(file, "Life time: %.2f\n", life_time);
    fprintf(file, "Average waiting time: %.2f\n", sum);
}