# include "codexion.h"

int finishing(t_sim *sim)
{
    t_coder coder;
    int     finished;
    int     i;

    finished = 1;
    i = 0;
    while (i < sim->args->number_of_coders)
    {
        coder = sim->coders[i];
        if (coder.compile_count < sim->args->number_of_compiles_required)
            finished = 0;
        i++;
    }
    return (finished);
}

void simulation(t_sim *sim)
{
    t_coder *coder;
    int     i;
    int     status;

    status = setup_coders(sim->coders, sim->args);
    if (!status)
        return (1);

    while (!finishing(sim))
    {
        i = 0;
        while (i < sim->args->number_of_coders)
        {
            coder = &sim->coders[i];
            if (request_dongles(coder, sim))
            {
                if (take_dongle(coder, sim))
                    join_coder(coder);
            }
            i++;
        }
    }
}