/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include<stdbool.h>

#define BLOCK_SIZE 32



struct Triple_sequence_info{
    int triple_number;
    int triples_count;
    int current_count;
};

struct Triple_sequence_info find_triples(int block[BLOCK_SIZE], struct Triple_sequence_info sequence){
    int i;
    for(i = 0; i < BLOCK_SIZE; i++){
        if(sequence.triple_number == block[i]){
            sequence.current_count++;
        } else {
            sequence.triple_number = block[i];
            sequence.current_count = 0;
        }
        if(sequence.current_count == 2){
            sequence.triples_count++;
            sequence.triple_number = 7;
        }
    }
    return sequence;
}

struct Full_sequence_info{
    int index;
    bool in_sequence;
    int count;
};

struct Full_sequence_info find_full_sequences(int block[BLOCK_SIZE], struct Full_sequence_info sequence){
    int full_sequence[6] = {0, 1, 2, 3, 4, 5};
    int i;
    for(i = 0; i < BLOCK_SIZE; i++){
        if(sequence.in_sequence){
            sequence.index++;
            if(block[i] != full_sequence[sequence.index])
            {
                sequence.in_sequence = false;
                continue;
            }
            if(sequence.index == 5){
                sequence.count++;
                sequence.in_sequence = false;
            }
            continue;
        }
        if(block[i] == 0){
            sequence.in_sequence = true;
            sequence.index = 0;
        }
    }
    return sequence;
}

struct High_sequence_info{
    int start;
    int value;
    int size;
} High_sequence_info;

struct High_sequence_info find_highest_sequence(int block[BLOCK_SIZE], struct High_sequence_info current_sequence){
    struct High_sequence_info max_sequence;
    max_sequence.size = 0;
    bool in_sequence = true;
    int i;
    for(i = 0; i < BLOCK_SIZE; i++){
        if(in_sequence){
            if(block[i] != current_sequence.value)
            {
                if(current_sequence.size > max_sequence.size){
                    max_sequence.size = current_sequence.size;
                    max_sequence.start = current_sequence.start;
                    max_sequence.value = current_sequence.value;
                }
                in_sequence = false;
                current_sequence.value = block[i];
                current_sequence.size = 1;
                current_sequence.start = i;
                continue;
            }
            current_sequence.size++;
            continue;
        }
        in_sequence = true;
        current_sequence.start = i;
        current_sequence.size = 1;
        current_sequence.value = block[i];
    }
    if(current_sequence.size > max_sequence.size){
        return current_sequence;
    }
    return max_sequence;
}

int main()
{
    int block[BLOCK_SIZE] = {3, 0, 1, 2, 3, 1, 1, 1, 1, 2, 3, 4, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 3, 3, 3, 0, 1, 2, 3, 4, 5, 0};
    
    struct Triple_sequence_info tsi_test;
    tsi_test.triple_number = 2;
    tsi_test.triples_count = 3;
    tsi_test.current_count = 1;
    struct Triple_sequence_info triple_sequence_info = find_triples(block, tsi_test);
    printf("Quantidade de triplas: %d\n", triple_sequence_info.triples_count);
    
    struct Full_sequence_info fsi_test;
    fsi_test.index = 2;
    fsi_test.in_sequence = true;
    fsi_test.count = 2;
    struct Full_sequence_info full_sequence_info = find_full_sequences(block, fsi_test);
    printf("Quantidade de sequencias: %d\n", full_sequence_info.count);
    
    struct High_sequence_info hsi_test;
    hsi_test.value = 3;
    hsi_test.size = 10;
    hsi_test.start = 200;
    struct High_sequence_info high_sequence_info = find_highest_sequence(block, hsi_test);
    printf("Maior sequência de valores idênticos: %d %d %d\n", high_sequence_info.start, high_sequence_info.size, high_sequence_info.value);
    return 0;
}





