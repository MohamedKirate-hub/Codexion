#include "codexion.h"

int main(int ac, char **av)
{
    t_args  args;
    t_sim   sim;
    int     status;
    int     i;

    if (ac != 9)
    {
        fprintf(stderr, "Error at Input.\n");
        return (1);
    }

    args = parser(av);
    sim.args = &args;
    sim.coders = malloc(sizeof(t_coder ) * args.number_of_coders);
    sim.dongles = malloc(sizeof(t_dongle) * args.number_of_coders);
    if (!sim.dongles)
    
    sim.running = 1;

    setup_dongles(&sim, args.number_of_coders);
    status = setup_coders(&sim);
    if (status)
        return (1);

    i = 0;
    while (i < args.number_of_coders)
    {
        pthread_join(sim.coders[i++].thread, NULL);
    }
}