#include "codexion.h"

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);
}

void is_overflow(int digit, int res, int sign)
{
    long int_max = 0;
    long int_min = 0;
    if (sign == 1 && (res > INT_MAX / 10
		|| (res == INT_MAX / 10 && digit > 7)))
    {
        fprintf(stderr, "ERROR: INT overflow.\n");
        exit(1);
    }
}

int ft_atoi(char *str)
{
    int sign;
    int digit;
    int res;
    int i;

    i = 0;
    res = 0;
    sign = 1;

    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign *= -1;
        i++;
    }
    // if (!str[i])
    // {
    //     fprintf(stderr, "fffffffffffffffffffffffffff");
    //     exit(0);
    // }

    while (str[i])
    {
        if (!is_digit(str[i]))
        {
            fprintf(stderr, "ERROR: please enter number.\n");
            exit(1);
        }
        digit = str[i] - 48;
        is_overflow(digit, res, sign);
        res = res * 10 + digit;
        i++;
    }
    return (res * sign);
}
