#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define SON_IGUALES(a, b) strcmp(a, b) == 0

char cadena[BUFSIZ];
char pwd[BUFSIZ];
char args[BUFSIZ];
char splited[10][BUFSIZ/10];
int command_2nd_index;
int count_tuberias,hubo_tuberias;
int index_command;
char out[BUFSIZ];


void pwd_fuction()
{
    strcpy(out, pwd);
}

void reset_splited(){
    for (int i = 0; i < 10; i++)
    {
        splited[i][0]='\0';
    }
    
}

void split()
{
    reset_splited();

    char no_necesary[]=" \n\t\0";
    char *elemento=strtok(cadena,no_necesary);

    int i=0;
    while (elemento!=NULL)
    {
        for (int j = 0; j < strlen(elemento); j++)
        {
            splited[i][j]=elemento[j];
        }
        splited[i][strlen(elemento)]='\0';
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
    }
}

void help_function(){
    int fd=open("help.txt",O_RDONLY);
    
    read(fd,out,BUFSIZ);
    close(fd);
}

void cd_function(){


    strcpy(args,splited[index_command+1]);

    if(SON_IGUALES(args,"")){
        strcpy(pwd,"\\home");
        return;
    }

    DIR *dir;
    dir = opendir(args);

    if(dir!=NULL){
        strcpy(pwd,args);
        out[0]='\0';
    }
    else{
        strcpy(out,"Directorio no encontrado");
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
    return (splited[index_command]=="ls"||splited[index_command]=="exit"||splited[index_command]=="cd"||splited[index_command]=="pwd");
}

void limpieza_de_signo_de_numero(){
    for (int i = 0; i < BUFSIZ && cadena[i]!='\0'; i++)
    {
        if(cadena[i]=='#'){
            cadena[i]='\0';
            return;
        }
    }
    
}


int main()
{
    getcwd(pwd, BUFSIZ);

    while (1)
    {
        index_command=0;
        count_tuberias=0;
        hubo_tuberias=0;

        printf("my-prompt $ ");
        fgets(cadena,BUFSIZ,stdin);
        
        limpieza_de_signo_de_numero();
        split();
        split_tuberia();
        out[0]='\0';
        
        while (count_tuberias>=0)
        {
            
            if (SON_IGUALES(splited[index_command], "ls"))
                ls_function();
            else if (SON_IGUALES(splited[index_command], "exit"))
                exit_function();
            else if (SON_IGUALES(splited[index_command], "pwd"))
                pwd_fuction();
            else if (SON_IGUALES(splited[index_command], "cd"))
                cd_function();
            else if (SON_IGUALES(splited[index_command],"help"))
                help_function();
            else
                strcpy(out,"Comando desconocido");
            
            index_command=command_2nd_index;
            if(hubo_tuberias){
                if(SON_IGUALES(splited[index_command+1],""))
                    strcpy(splited[index_command+1],out);
                out[0]='\0';
                hubo_tuberias=0;
            }
            else{
                if(out[0]!='\0')
                    printf("%s\n",out);
            }
            count_tuberias--;
        }
        
    }

    return 0;
}