#include "minishell.h"

int ft_iter_char(const char *str, int c)
{
    int count;
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            count++;
        i++;
    }
    return (count);
}
