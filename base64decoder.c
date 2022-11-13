#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

char base64[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '\\'};

static int tabnb(char c)
{
    for (int i = 0; i < 64; i++)
        if (c == base64[i])
            return (i);
    return (-1);
}

static int base64decoder(unsigned char *buffer, int read, int totalread)
{
    char result[3];
    int nbbuf = 0;
    if ((totalread % 76 == 0 && totalread > 0) || buffer[0] == '\n')
        return (0);
    nbbuf = nbbuf * 64 + tabnb(buffer[0]);
    nbbuf = nbbuf * 64 + tabnb(buffer[1]);
    nbbuf = (buffer[2] != '=') ? nbbuf * 64 + tabnb(buffer[2]) : nbbuf * 64;
    nbbuf = (buffer[3]) ? nbbuf * 64 + tabnb(buffer[3]) : nbbuf * 64;
    result[2] = (buffer[3] != '=') ? nbbuf % 256 : '\0';
    nbbuf /= 256;
    result[1] = (buffer[2] != '=') ? nbbuf % 256 : '\0';
    nbbuf /= 256;
    result[0] = nbbuf % 256;
    if (write(1, result, 3) < 0)
        return (-1);
    return (0);
}

static int read_input(void)
{
    int totalread = 0;
    int readsize = 4;
    char buffer[4];

    while (readsize == 4) {
        if ((readsize = read(0, buffer, 4)) < 0)
            return (84);
        if (base64decoder(buffer, readsize, totalread) < 0)
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