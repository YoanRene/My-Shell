#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#define SON_IGUALES(a, b) strcmp(a, b) == 0
#define TAMANO 30

char cadena[BUFSIZ];
char pegado[BUFSIZ];
char auxiliar1[BUFSIZ];
char pwd[BUFSIZ];
char args[BUFSIZ];
char splited[TAMANO][BUFSIZ];
char splited_aux[TAMANO][BUFSIZ];
int command_2nd_index;
int count_tuberias;
int index_command;
char cadena_aux[BUFSIZ];
char out[BUFSIZ];

void reset_splited()
{
    for (int i = 0; i < TAMANO; i++)
    {
        splited[i][0] = '\0';
    }
}

void split(char no_necesary[])
{
    reset_splited();
    char *elemento = strtok(cadena, no_necesary);

    int i = 0;
    while (elemento != NULL)
    {
        for (int j = 0; j < strlen(elemento); j++)
        {
            splited[i][j] = elemento[j];
        }
        splited[i][strlen(elemento)] = '\0';
        elemento = strtok(NULL, no_necesary);
        i++;
    }
}

void replace_blank_por_slash()
{
    for (int i = 0; i < TAMANO; i++)
    {
        for (int j = 0; j < BUFSIZ && splited[i][j]!='\0'; j++)
        {
            if(splited[i][j]=='\\')
                splited[i][j]=' ';
        }
        
    }
}

void recuperador_de_path_function_with_split()
{
    split("\\");
    strcpy(pegado,splited[0]);
    int i=1;
    while (i<TAMANO&&splited[i][0]!='\0')
    {
        strcat(pegado,"\\");
        for (int j = 1; j < BUFSIZ; j++)
        {
            auxiliar1[j-1]=splited[i][j];
            if(splited[i][j]=='\0'){
                break;
            }
        }  
        
        strcat(pegado,auxiliar1);
        i++;
    }
    
    strcpy(cadena,pegado);
    split(" \n\t\0");
    replace_blank_por_slash();
}



void echo_function()
{
    strcpy(out, splited[index_command + 1]);
}

void pwd_fuction()
{
    strcpy(out, pwd);
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
    strcpy(out, "");

    if (dir != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            if (!SON_IGUALES(ent->d_name, ".") && !SON_IGUALES(ent->d_name, ".."))
            {
                strcat(out, ent->d_name);
                strcat(out, " ");
            }
        }
    }
}

void help_function()
{
    int fd = open("help.txt", O_RDONLY);

    read(fd, out, BUFSIZ);
    close(fd);
}

void cd_function()
{

    strcpy(args, splited[index_command + 1]);

    if (SON_IGUALES(args, ""))
    {
        strcpy(pwd, "\\home");
        return;
    }
    strcpy(args,"/mnt/d/My Projects");

    DIR *dir;
    dir = opendir(args);

    if (dir != NULL)
    {
        strcpy(pwd, args);
        out[0] = '\0';
    }
    else
    {
        strcpy(out, "Directorio no encontrado"); // perror("ERROR");exit(1);
    }
}

void split_tuberia()
{
    for (int i = 0; i < TAMANO; i++)
    {
        if (SON_IGUALES(splited[i], "|"))
        {
            command_2nd_index = i + 1;
            count_tuberias++;
            return;
        }
    }
}

int es_comando_valido()
{
    return (splited[index_command] == "ls" || splited[index_command] == "exit" || splited[index_command] == "cd" || splited[index_command] == "pwd");
}

void limpieza_de_signo_de_numero()
{
    for (int i = 0; i < BUFSIZ && cadena[i] != '\0'; i++)
    {
        if (cadena[i] == '#')
        {
            cadena[i] = '\0';
            return;
        }
    }
}

int main()
{
    getcwd(pwd, BUFSIZ);

    while (1)
    {
        index_command = 0;
        count_tuberias = 0;
        printf("my-prompt $ ");
        fgets(cadena, BUFSIZ, stdin);

        limpieza_de_signo_de_numero();
        recuperador_de_path_function_with_split();
        split_tuberia();
        out[0] = '\0';

        while (count_tuberias >= 0)
        {
            if (SON_IGUALES(splited[index_command], "ls"))
                ls_function();
            else if (SON_IGUALES(splited[index_command], "exit"))
                exit_function();
            else if (SON_IGUALES(splited[index_command], "pwd"))
                pwd_fuction();
            else if (SON_IGUALES(splited[index_command], "cd"))
                cd_function();
            else if (SON_IGUALES(splited[index_command], "help"))
                help_function();
            else if (SON_IGUALES(splited[index_command], "echo"))
                echo_function();
            else
            {
                if (isalnum(splited[index_command][0]) == 0)
                {
                    strcpy(out, "Error de sintaxis");
                }
                else
                    strcpy(out, "Comando desconocido"); // fprintf(stderr,"ERROR");exit(1);
            }
            index_command = command_2nd_index;
            if (count_tuberias)
            {
                if (SON_IGUALES(splited[index_command + 1], ""))
                    strcpy(splited[index_command + 1], out);
                out[0] = '\0';
            }
            else
            {
                if (out[0] != '\0')
                    printf("%s\n", out);
            }
            count_tuberias--;
        }
    }

    return 0;
}