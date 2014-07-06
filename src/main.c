#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdarg.h>
#include <gd.h>

#include <strlist.h>


#define T_UNKNOWN   -1
#define T_PNG       0
#define T_GIF       1
#define T_WBMP      2
#define T_JPEG      3
#define T_WEBP      4
#define T_TIFF      5
#define T_TGA       6
#define T_BMP       7



static char * executable;
static strlist ** extensions;
static char err;



int usage(void)
{
    fprintf(stderr, "Usage: %s filename [...]\n", executable);
    return EXIT_FAILURE;
}


void init_extensions(void)
{
    extensions = (strlist **) malloc(sizeof(strlist *) * 8);

    extensions[T_PNG]   = strlist_init_args(2, "png", "PNG");
    extensions[T_GIF]   = strlist_init_args(2, "gif", "GIF");
    extensions[T_WBMP]  = strlist_init_args(2, "wbmp", "WBMP");
    extensions[T_JPEG]  = strlist_init_args(4, "jpg", "JPG", "jpeg", "JPEG");
    extensions[T_WEBP]  = strlist_init_args(2, "webp", "WEBP");
    extensions[T_TIFF]  = strlist_init_args(4, "tiff", "TIFF", "tif", "TIF");
    extensions[T_TGA]   = strlist_init_args(4, "tga", "TGA", "tpic", "TPIC");
    extensions[T_BMP]   = strlist_init_args(4, "bmp", "BMP", "dib", "DIB");
}


int get_img_type(const char * fname)
{
    char * pch = NULL;
    char * ext = NULL;
    size_t len,
           n;
    size_t * res;

    pch = strrchr(fname, '.');
    if(pch == NULL)
        return T_UNKNOWN;
    len = strlen(pch);
    if(len < 2)
        return T_UNKNOWN;

    ext = (char *) malloc(len);
    strncpy(ext, pch + 1, len - 1);
    ext[len - 1] = '\0';

    n = 0;
    while(n < sizeof(extensions)) {
        res = strlist_search(extensions[n], ext);
        if(res != NULL)
            return n;
        n++;
    }

    return T_UNKNOWN;
}


void show_img_size(const char * fname, unsigned int padding)
{
    FILE * fp;
    int type;
    gdImagePtr img;
    struct stat fstat;
    int ret;

    if((ret = stat(fname, &fstat)) != 0) {
        fprintf(stderr, "%-*s: Cannot open file\n", padding, fname);
        return;
    }
    if(S_ISDIR(fstat.st_mode)) {
        fprintf(stderr, "%-*s: Is a directory\n", padding, fname);
        return;
    }

    type = get_img_type(fname);
    if(type == T_UNKNOWN) {
        fprintf(stderr, "%-*s: Unknown type\n", padding, fname);
        return;
    }

    if((fp = fopen(fname, "rb")) == NULL) {
        fprintf(stderr, "%-*s: Cannot open file\n", padding, fname);
        return;
    }

    err = 0;
    switch(type) {
        case T_PNG:
            img = gdImageCreateFromPng(fp);
            break;
        case T_GIF:
            img = gdImageCreateFromGif(fp);
            break;
        case T_WBMP:
            img = gdImageCreateFromWBMP(fp);
            break;
        case T_JPEG:
            img = gdImageCreateFromJpeg(fp);
            break;
        case T_WEBP:
            img = gdImageCreateFromWebp(fp);
            break;
        case T_TIFF:
            img = gdImageCreateFromTiff(fp);
            break;
        case T_TGA:
            img = gdImageCreateFromTga(fp);
            break;
        case T_BMP:
            img = gdImageCreateFromBmp(fp);
    }
    fclose(fp);

    if(img == NULL || err)
        fprintf(stderr, "%-*s: wrong file\n", padding, fname);
    else
        printf("%-*s: %dx%d\n", padding, fname, gdImageSX(img), gdImageSY(img));

    if(img != NULL)
        gdImageDestroy(img);
}


void gd_errmethod(int n, const char * msg, va_list args)
{
    err = 1;
}


int main(int argc, const char * argv[])
{
    unsigned int i,
                 len,
                 padding;

    executable = (char *) malloc(strlen(argv[0]));
    strcpy(executable, argv[0]);

    if(argc < 2)
        return usage();

    init_extensions();

    i = 1;
    while(i < argc) {
        len = strlen(argv[i++]);
        if(len > padding)
            padding = len;
    }

    gdSetErrorMethod(gd_errmethod);

    i = 1;
    while(i < argc)
        show_img_size(argv[i++], padding);

    free(executable);
    i = 0;
    while(i < sizeof(extensions))
        strlist_free(&extensions[i++]);
    free(extensions);

    return err ? EXIT_FAILURE : EXIT_SUCCESS;
}

