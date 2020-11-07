#include    "unpipc.h"

int
main(int argc, char **argv)
{
    if (argc != 2)
        err_quit("usage: shmrmid <pathname>");

    int id = Shmget(Ftok(argv[1], 0), 0, SVSHM_MODE);
    Shmctl(id, IPC_RMID, NULL);
}
