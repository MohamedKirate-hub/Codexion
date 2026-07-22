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

    args = parser(ac, av);
    sim.args = &args;
    sim.coders = malloc(sizeof(t_coder ) * args.number_of_coders);
    sim.dongles = malloc(sizeof(t_dongle) * args.number_of_coders);
    sim.running = 1;

    setup_dongles(&sim, args.number_of_coders);
    status = setup_coders(sim.coders, sim.args);
    if (status)
        return (1);
    fill_heap(&sim);

    if (request_dongles(&sim.coders[1], &sim))
    {
        take_dongle(&sim.coders[1], &sim);
    }

    printf("Left taked: %d.\n", sim.coders[1].left_taked);
    if (sim.coders[1].left_taked)
        printf("Left dongle id: %d.\n", sim.coders[1].left->id);
    
    printf("Right taked: %d.\n", sim.coders[1].right_taked);
    if (sim.coders[1].right_taked)
        printf("Right dongle id: %d.\n", sim.coders[1].right->id);

    // simulation(&sim);
}