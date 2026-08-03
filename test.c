#include "codexion.h"

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

int main()
{
    char *s1 = "1234";
    char *s2 = "12c12";

    int i = 0;
    while (s1[i])
    {
        if (!is_digit(s1[i]))
        {
            printf("not digit %s.\n", s1);
            break;
        }
        i++;
    }
    i = 0;
    while (s2[i])
    {
        if (!is_digit(s2[i]))
        {
            printf("not digit %s.\n", s2);
            break;
        }
        i++;
    }
}