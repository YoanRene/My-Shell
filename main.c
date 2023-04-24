#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

/*
Integrantes: 
Yoan Rene Ramos Corrales - C312
Kevin Majim Ortega Alvarez - C312

Funcionalidades:
basic: funcionalidades basicas (3 puntos)
help: ayuda (1 punto)

Comandos built-in:
cd: cambia de directorios
exit: termina el shell
help: muestra la ayuda del shell
ls: muesta la lista de archivos y carpetas de un directorio
echo: muestra el texto entrado despues del comando 

Total: 4 puntos*/

#define SON_IGUALES(a, b) strcmp(a, b) == 0
#define TAMANO 30

char pegado[BUFSIZ];
char auxiliar1[BUFSIZ];

char pwd[BUFSIZ];             //
char cadena[BUFSIZ];          //
char args[BUFSIZ];            // Importantes
char splited[TAMANO][BUFSIZ]; //

char splited_aux[TAMANO][BUFSIZ];
int command_2nd_index;
int count_tuberias;
int index_command;
char cadena_aux[BUFSIZ];

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
        for (int j = 0; j < BUFSIZ && splited[i][j] != '\0'; j++)
        {
            if (splited[i][j] == '\\')
                splited[i][j] = ' ';
        }
    }
}

void recuperador_de_path_function_with_split()
{
    split("\\");
    strcpy(pegado, splited[0]);
    int i = 1;
    while (i < TAMANO && splited[i][0] != '\0')
    {
        strcat(pegado, "\\");
        for (int j = 1; j < BUFSIZ; j++)
        {
            auxiliar1[j - 1] = splited[i][j];
            if (splited[i][j] == '\0')
            {
                break;
            }
        }

        strcat(pegado, auxiliar1);
        i++;
    }

    strcpy(cadena, pegado);
    split(" \n\t\0");
    replace_blank_por_slash();
}

void echo_function()
{
    printf("%s", args);
}

void pwd_fuction()
{
    printf("%s", pwd);
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
            if (!SON_IGUALES(ent->d_name, ".") && !SON_IGUALES(ent->d_name, ".."))
                printf("%s ", ent->d_name);
    }
}

void help_function()
{
    //int fd = open("help.txt", O_RDONLY);

    //read(fd, out, BUFSIZ);
    //close(fd);
}

void cd_function()
{

    strcpy(args, splited[index_command + 1]);

    if (SON_IGUALES(args, ""))
    {
        strcpy(pwd, "\\home");
        return;
    }

    DIR *dir;
    dir = opendir(args);

    if (dir != NULL)
        strcpy(pwd, args);
    else
        printf("%s", "Directorio no encontrado"); // perror("ERROR");exit(1);
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
        int fd_r;
        int fd_w;

        printf("my-prompt $ ");
        fgets(cadena, BUFSIZ, stdin);

        limpieza_de_signo_de_numero();
        recuperador_de_path_function_with_split();
        args[0] = '\0';

        //OK
        //int std_ini = dup(STDIN_FILENO);
        int std_out = dup(STDOUT_FILENO);
        //OK
        while (count_tuberias >= 0)
        {
            int k = index_command+1;
            while (SON_IGUALES(splited[k], ">")||SON_IGUALES(splited[k], ">>")||SON_IGUALES(splited[k], "<"))
            {
                if (SON_IGUALES(splited[k], ">"))
                {
                    fd_w = open(splited[k + 1], O_WRONLY | O_CREAT | O_TRUNC);
                    if (fd_w != -1)
                        dup2(fd_w, STDOUT_FILENO);
                }
                else if (SON_IGUALES(splited[k], ">>"))
                {
                    fd_w = open(splited[k + 1], O_WRONLY | O_CREAT | O_APPEND);
                    if (fd_w != -1)
                        dup2(fd_w, STDOUT_FILENO);
                }
                else
                {
                    fd_r = open(splited[k + 1], O_RDONLY);
                    if (fd_r != -1)
                        read(fd_r,args,BUFSIZ);
                    close(fd_r);
                }
                k+=2;
            }
            //OK

            int hay_in_out=(k!=1);
            if(!hay_in_out){
                if(!SON_IGUALES(splited[k],"|"))
                    strcpy(args,splited[k]);
            }

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
                    printf("%s", "Error de sintaxis");
                else
                    printf("%s", "Comando desconocido"); // fprintf(stderr,"ERROR");exit(1);
            }
            index_command = command_2nd_index;
           // dup2(std_ini, STDIN_FILENO);
            dup2(std_out, STDOUT_FILENO);
            count_tuberias--;
        }
        printf("\n");
    }

    return 0;
}