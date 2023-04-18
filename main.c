#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <windows.h>
#include <dirent.h>

#define SON_IGUALES(a, b) strcmp(a, b) == 0

enum Colors
{
    BLACK = 0,
    BLUE = 1,
    MAGENTA = 5,
    WHITE = 15
};

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



void Color(int Background, int Text)
{ // Función para cambiar el color del fondo y/o pantalla

    HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE); // Tomamos la consola.

    // Para cambiar el color, se utilizan números desde el 0 hasta el 255.
    // Pero, para convertir los colores a un valor adecuado, se realiza el siguiente cálculo.
    int New_Color = Text + (Background * 16);

    SetConsoleTextAttribute(Console, New_Color); // Guardamos los cambios en la Consola.
}

int main()
{
    getcwd(pwd, BUFSIZ);

    while (1)
    {
        Color(BLACK, MAGENTA);
        printf("my-prompt $ ");
        Color(BLACK, WHITE);
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