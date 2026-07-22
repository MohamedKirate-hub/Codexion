# include "codexion.h"

int request_dongles(t_coder *coder, t_sim *sim)
{
    int i;
    int left_id;
    int right_id;
    t_dongle left_dongle;
    t_dongle right_dongle;

    left_id = coder->ids[0];
    right_id = coder->ids[1];
    left_dongle = sim->dongles[left_id];
    right_dongle = sim->dongles[right_id];

    if (coder->id % 2 == 0)
    {
        if (left_dongle.available && coder->id == left_dongle.heap->data[0]->id)
            return (1);
        else
            return (0);
    }
    else
    {
        if (right_dongle.available && coder->id == right_dongle.heap->data[0]->id)
            return (1);
        else
            return (0);
    }
}
