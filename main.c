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
    sim.created_coders = 0;
    pthread_cond_init(&sim.cond, NULL);
    pthread_mutex_init(&sim.sim_mutex, NULL);
    pthread_mutex_init(&sim.run, NULL);
    pthread_mutex_init(&sim.compile, NULL);

    sim.coders = malloc(sizeof(t_coder ) * args.number_of_coders);
    if (!sim.coders)
    {
        fprintf(stderr, "Error: failed to allocate memory for coders creation.\n");
        exit(1);
    }
    sim.dongles = malloc(sizeof(t_dongle) * args.number_of_coders);
    if (!sim.dongles)
    {
        fprintf(stderr, "Error: failed to allocate memory for Dongles creation.\n");
        exit(1);
    }
    
    sim.running = 0;
    sim.begin = 0;
    
    setup_dongles(&sim, args.number_of_coders);
    status = setup_coders(&sim);
    if (status)
        return (1);

    i = 0;
    while (i < args.number_of_coders)
        pthread_join(sim.coders[i++].thread, NULL);
}