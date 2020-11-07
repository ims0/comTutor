#include    "unpipc.h"

int main(int argc, char **argv)
{
    int c;
    int oflag = SVSHM_MODE | IPC_CREAT;
    while ( (c = Getopt(argc, argv, "e")) != -1) {
        switch (c) {
        case 'e':
            oflag |= IPC_EXCL;
            break;
        }
    }

    if (optind != argc - 2)
        err_quit("usage: shmget [ -e ] <pathname> <length>");
    size_t length = atoi(argv[optind + 1]);

    int id = Shmget(Ftok(argv[optind], 0), length, oflag);
    void *shmPtr = Shmat(id, NULL, 0);
    snprintf((char*)shmPtr, length,"%s","system V shareMemery!");
}
