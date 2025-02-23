
//first read if the command contains "-records"
//then whether or not the -h is valid or wtv, then get that field
//then the third option, print the rows that correspond to the 3rd value

void record_check(char *file, int idx, char *val){

    FILE *file = fopen(file, "r"); //pointer to open and read file
    char line[MAX_LINE_LENGTH]; //buffer that stores the header row
    char *value = val;
    int curr_col = 0;


    //      int h_function = h(file);


    //    for (i=1; i<argc; i++) {
    //        if(strcmp(argv[i], "-records") == 0){
    //            value = argv[i+2];
    //        }
    //    }
        //the next argument has the feild whether it is the name or the index
        //open the file and then read through the lines


    while (fgets(line, sizeof(line), file)){
        char *field;
        field = strtok(line, ",\n"); //split each field

        while (field != NULL){
            char *process_field = process_quoted_field(field);
            //processes the field and gets rid of the quotations

            if (curr_col == idx){
                if(strcmp(field, value) == 0){
                    print("%s", line); //print line if it matches
                }
            }

            if(process_field != field){
                free(process_field);
            }

            curr_col ++;
            field = strtok(NULL, ",\n");


        }


        //return EXIT_FAILURE;
        //scan the row to look for the value in the column, then print that row

    }

    fclose(file);
    
}

