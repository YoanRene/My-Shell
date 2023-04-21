#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define SON_IGUALES(a, b) strcmp(a, b) == 0

char cadena[BUFSIZ];
char pwd[BUFSIZ];
char args[BUFSIZ];
char splited[10][BUFSIZ/10];
int command_2nd_index;
int count_tuberias,hubo_tuberias;
int index_read;
char out[BUFSIZ];


void pwd_fuction()
{
    strcpy(out, pwd);
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
    strcpy(out,"");

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (!SON_IGUALES(ent->d_name, ".") && !SON_IGUALES(ent->d_name, ".."))
            {
                strcat(out,ent->d_name);
                strcat(out," ");
            }
        }
        strcat(out,"\n");

    }
}

void cd_function(){


    strcpy(args,splited[index_read+1]);

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
        strcpy(out,"Directorio no encontrado\n");
    }
}

void split_tuberia(){
    for (int i = 0; i < 10; i++)
    {
        if(SON_IGUALES(splited[i],"|")){
            command_2nd_index=i+1;
            count_tuberias++;
            hubo_tuberias=1;
            return;
        }
    }
    
}

int es_comando_valido(){
    return (splited[index_read]=="ls"||splited[index_read]=="exit"||splited[index_read]=="cd"||splited[index_read]=="pwd");
}

int main()
{
    getcwd(pwd, BUFSIZ);

    while (1)
    {
        index_read=0;
        count_tuberias=0;
        hubo_tuberias=0;
        printf("my-prompt $ ");
        fgets(cadena,BUFSIZ,stdin);
        split();
        split_tuberia();
        
        while (count_tuberias>=0)
        {
            
            if (SON_IGUALES(splited[index_read], "ls"))
                ls_function();
            else if (SON_IGUALES(splited[index_read], "exit"))
                exit_function();
            else if (SON_IGUALES(splited[index_read], "pwd"))
                pwd_fuction();
            else if (SON_IGUALES(splited[index_read], "cd"))
                cd_function();
            else
                strcpy(out,"Comando desconocido\n");
            
            index_read=command_2nd_index;
            if(hubo_tuberias){
                if(SON_IGUALES(splited[index_read+1],""))
                    strcpy(splited[index_read+1],out);
                out[0]='\0';
                hubo_tuberias=0;
            }
            else{
                printf("%s",out);
            }
            count_tuberias--;
        }
        
        
    }

    return 0;
}