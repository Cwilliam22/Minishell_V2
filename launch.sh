valgrind \
        -s \
        --leak-check=full \
        --track-origins=yes \
        --show-leak-kinds=all \
        --suppressions=./supp.supp \
        --track-fds=yes \
        ./minishell