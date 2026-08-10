#include "codexion.h"

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

int is_overflow(int digit, int res)
{
    long int_max = 0;
    long int_min = 0;
    if (res > INT_MAX / 10 || (res == INT_MAX / 10 && digit > 7))
    {
        fprintf(stderr, "ERROR: INT overflow.\n");
        return (1);
    }
    return (0);
}

int ft_atoi(char *str)
{
    int sign;
    int digit;
    int res;
    int i;

    i = 0;
    res = 0;

    if (str[i] == '+')
        i++;
    if (!str[i])
    {
        fprintf(stderr, "fffffffffffffffffffffffffff");
        return (-1);
    }
    while (str[i])
    {
        if (!is_digit(str[i]))
        {
            fprintf(stderr, "ERROR: please enter number.\n");
            return (-1);
        }
        digit = str[i] - 48;
        if (is_overflow(digit, res))
            return (-1);
        res = res * 10 + digit;
        i++;
    }
    return (res * sign);
}
