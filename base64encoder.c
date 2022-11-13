#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

char base64[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '\\'};

static int base64encoder(unsigned char *buffer, int read, int totalread)
{
    char result[4];
    int nbbuf = 0;
    if (totalread % 57 == 0 && totalread > 0)
        if (write(1, "\n", 1) < 0)
            return (-1);
    nbbuf = nbbuf * 256 + buffer[0];
    nbbuf = (read >= 2) ? nbbuf * 256 + buffer[1] : nbbuf * 256;
    nbbuf = (read >= 3) ? nbbuf * 256 + buffer[2] : nbbuf * 256;
    result[3] = (read >= 3) ? base64[nbbuf % 64] : '=';
    nbbuf /= 64;
    result[2] = (read >= 2) ? base64[nbbuf % 64] : '=';
    nbbuf /= 64;
    result[1] = base64[nbbuf % 64];
    nbbuf /= 64;
    result[0] = base64[nbbuf % 64];
    if (write(1, result, 4) < 0)
        return (-1);
    return (0);
}

static int read_input(void)
{
    int totalread = 0;
    int readsize = 3;
    char buffer[3];

    while (readsize == 3) {
        if ((readsize = read(0, buffer, 3)) < 0)
            return (84);
        if (base64encoder(buffer, readsize, totalread) < 0)
            return (84);
        totalread += readsize;
    }
    if (write(1, "\n", 1) < 0)
        return (84);
    return (0);
}

int main (void)
{
    return (read_input());
}