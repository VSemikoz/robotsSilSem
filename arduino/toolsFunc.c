char* split(char *string, char sepSymbol){
    char *command[10];
    int i = 0;
    command[i] = strtok(NULL, sepSymbol);

    while(command[i] != NULL) {
        command[++i] = strtok(NULL, sepSymbol);
    }
    return command;
}

char* removeFirst(char *array){
    char *result[10];
    int arrayLen = (int) sizeof(array);
    for(int i = 1; i < arrayLen; i++){
        result[i-1] = array[i];
    }
    return result;
}