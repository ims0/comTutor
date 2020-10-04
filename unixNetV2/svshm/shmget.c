#include    "unpipc.h"

int
main(int argc, char **argv)
{
    int        c, id, oflag;
    void    *shmPtr;
    size_t    length;

    oflag = SVSHM_MODE | IPC_CREAT;
    while ( (c = Getopt(argc, argv, "e")) != -1) {
        switch (c) {
        case 'e':
            oflag |= IPC_EXCL;
            break;
        }
    }
    if (optind != argc - 2)
        err_quit("usage: shmget [ -e ] <pathname> <length>");
    length = atoi(argv[optind + 1]);

    id = Shmget(Ftok(argv[optind], 0), length, oflag);
    shmPtr = Shmat(id, NULL, 0);
    snprintf((char*)shmPtr, length,"%s","system V shareMemery!");

    exit(0);
}
