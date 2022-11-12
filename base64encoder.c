#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

char base64[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '\\'};

static int read_input(unsigned char **buffer)
{
    int readsize = 0;
    int bufsize = 256;
    unsigned char *save;

    if ((*buffer = malloc(bufsize + 1)) == ((void *)0))
        return (-1);
    if ((readsize = read(0, *buffer, bufsize)) < 0)
        return (-1);
    if (readsize < bufsize) {
        (*buffer)[readsize] = '\0';
        return (readsize);
    }
    readsize /= 2;
    while (readsize == bufsize / 2) {
        save = *buffer;
        if ((*buffer = malloc((bufsize *= 2) + 1)) == ((void *)0))
            return (-1);
        for (int i = 0; i < bufsize / 2; i++)
            (*buffer)[i] = save[i];
        free(save);
        if ((readsize = read(0, &(*buffer)[bufsize / 2], bufsize / 2)) < 0)
            return (-1);
    }
    (*buffer)[bufsize / 2 + readsize] = '\0';
    return (bufsize / 2 + readsize);
}

static int base64encoder(unsigned char *buffer, int bufsize)
{
    char *result = malloc(bufsize * 2);
    int resultpos = 0;
    int nbbuf = 0;

    for (int i = 0; i < bufsize; i += 3, nbbuf = 0, resultpos += 4) {
        if (i % 57 == 0 && i > 0)
            result[resultpos++] = '\n';
        nbbuf = nbbuf * 256 + buffer[i];
        nbbuf = (bufsize - i >= 2) ? nbbuf * 256 + buffer[i + 1] : nbbuf * 256;
        nbbuf = (bufsize - i >= 3) ? nbbuf * 256 + buffer[i + 2] : nbbuf * 256;
        result[resultpos + 3] = (bufsize - i >= 3) ? base64[nbbuf % 64] : '=';
        nbbuf /= 64;
        result[resultpos + 2] = (bufsize - i >= 2) ? base64[nbbuf % 64] : '=';
        nbbuf /= 64;
        result[resultpos + 1] = base64[nbbuf % 64];
        nbbuf /= 64;
        result[resultpos] = base64[nbbuf % 64];
    }
    free(buffer);
    result[resultpos++] = '\n';
    if (write(1, result, resultpos) < 0)
        return (-1);
    free(result);
    return (0);
}

int main (void) 
{
    unsigned char *buffer;
    int bufsize = 0;
    if ((bufsize = read_input(&buffer)) < 0)
        return (84);
    return (base64encoder(buffer, bufsize));
}