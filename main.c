#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define SON_IGUALES(a, b) strcmp(a, b) == 0

char cadena[BUFSIZ];
char pwd[BUFSIZ];
char args[BUFSIZ];


void pwd_fuction()
{
    printf("%s\n", pwd);
}

void split()
{
    char no_necesary[]=" \n\t\0";
    char *elemento=strtok(cadena,no_necesary);

    int i=0;
    while (elemento!=NULL)
    {
        for (int j = 0; j < strlen(elemento); j++)
        {
            splited[i][j]=elemento[j];
        }
        elemento=strtok(NULL,no_necesary);
        i++;
    }

}



void exit_function()
{
    exit(EXIT_SUCCESS);
}

void ls_function()
{
    DIR *dir;
    dir = opendir(pwd); // abrir direccion actual
    struct dirent *ent;

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (!SON_IGUALES(ent->d_name, ".") && !SON_IGUALES(ent->d_name, ".."))
            {
                printf("%s ", ent->d_name);
            }
        }
        printf("\n");
    }
}

int es_cd(){
    if(cadena[0]=='c'&&cadena[1]=='d'&&(cadena[2]==' '||cadena[2]=='\0')){
        return 1;
    }
    return 0;
}

void cd_function(){

    for (int i = 3; i < BUFSIZ; i++)
    {
        args[i-3]=cadena[i];
    }

    if(SON_IGUALES(args,"")){
        strcpy(pwd,"\\home");
        return;
    }

    DIR *dir;
    dir = opendir(args);

    if(dir!=NULL){
        strcpy(pwd,args);
    }
    else{
        printf("Directorio no encontrado\n");
    }
}

void split_tuberia(){
    array[0]=0;
    for (int i = 0; i < 10; i++)
    {
        if(SON_IGUALES(splited[i],"|")){
            array[1]=i;
        }
    }
    
}

int main()
{
    getcwd(pwd, BUFSIZ);

    while (1)
    {
        printf("my-prompt $ ");
        gets(cadena);

        if (SON_IGUALES(cadena, "ls"))
            ls_function();
        else if (SON_IGUALES(cadena, "exit"))
            exit_function();
        else if (SON_IGUALES(cadena, "pwd"))
            pwd_fuction();
        else if (es_cd()){
            cd_function();
        }
        else {
            printf("Comando desconocido\n");
        }
    }

    return 0;
}