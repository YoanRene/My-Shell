#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>


#define SON_IGUALES(a, b) strcmp(a, b) == 0

char cadena[BUFSIZ];
char pwd[BUFSIZ];

void pwd_fuction()
{
    printf("%s\n", pwd);
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
        
    }

    return 0;
}