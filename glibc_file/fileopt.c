/*************************************************************************
	> File Name: fileopt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Tue 18 May 2021 08:05:33 AM CST
 ************************************************************************/

#include <stdio.h>


int main(int argc, char* argv[]) {

    
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("ERROR:");
        return 0;
    }
    FILE* fout = fopen(argv[2], "w+");

    char c;
    while( (c = fgetc(fp)) != EOF ) {
        printf("%c", c);
        fputc(c, fout);
    }


    fclose(fp);
    fclose(fout);
    return 0;
}

