#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

bool check_time(int total, int new, int * arr ){
    if (0 < new > 23 ){
        printf("Error: the time must be in the [0, 23] hour range.\n");
        return false;
    }
    for (int i = 0 ; i < total; ++i ){
        int val = arr[i];
        if (val < new && new-val < 3){
            printf("Error: Cannot have mutiple meals in less than 3 hours.\n");
            return false;
        }else if (val > new && val-new < 3){
            printf("Error: Cannot have mutiple meals in less than 3 hours.\n");
            return false;
        }
    }
    return true;
}

bool check_cals(int og, int new){
    if ((og + new)> 2500){
        printf("Error: Caloric intake exceeds the 2500 calories limit.\n");
        return false;
    }
    return true;
}

int value_in_array(int val, int * arr, int size){
    for (int i = 0; i < size; ++i){
        if (arr[i] == val)
            return i;
    }
    return -1;
}
void swap(int * x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}
void selectionSort(int arr[], int n){
    int i, j, min_idx;
    for(i = 0; i < n - 1; i++){
        min_idx = i;
        for (j = i+1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        swap(&arr[min_idx], &arr[i]);
    }
}
void intro(){
    printf("Hello! Welcome to you calorie intake tracker.\n");
    printf("You may enter any of the following commands: \n");
    printf("    add <calorie_value> <time>\n");
    printf("    replace <calorie_value> <time>\n");
    printf("    printplan\n");
    printf("    quit\n");
    printf("Total calories must not exceed 2500 and times must be in [0, 23] range.\n");
}

int main(){
    char str[100];
    int total_calorie = 0;
    int time = 0;
    int times[128];
    int cals[128];
    int time_count = 0;
    intro();
    printf("$ ");
    fgets(str, sizeof(str), stdin);
    char * tok  = strtok(str, " ");
    while (strcmp(tok, "quit\n")!=0){
        if (strcmp(tok, "add") == 0){
            tok = strtok(NULL, " ");
            int current_cal = atoi(tok);
            tok = strtok(NULL, " ");
            int current_time = atoi(tok);
            if (check_time(time_count, current_time, times) && check_cals(total_calorie, current_cal)){
                time = current_time;
                times[time_count] = time;
                cals[time_count] = current_cal;
                ++time_count;
                total_calorie += current_cal;
            }
        }else if (strcmp(tok, "replace") == 0){
            tok = strtok(NULL, " ");
            int current_cal = atoi(tok);
            tok = strtok(NULL, " ");
            int current_time = atoi(tok);
            int location = value_in_array(current_time, times, time_count);

            if (location == -1)
                printf("Error: Meal does not exist in the record.\n");
            else{
                int previous = cals[location];
                if (check_cals(total_calorie-previous, current_cal)){
                    cals[location] = current_cal;
                    total_calorie -= previous;
                    total_calorie += current_cal;
                }
            }
        }else if (strcmp(tok, "printplan\n") == 0){
            selectionSort(times, time_count);
            for (int i = 0; i < time_count; ++i){
                printf("%d %d\n", times[i], cals[i]);
            }
        }else{
            printf("Error: command does not exist.\n");
        }
        printf("$ ");
        fgets(str, sizeof(str), stdin);
        tok = strtok(str, " ");
    }
}
