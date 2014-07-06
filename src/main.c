#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <gd.h>

#include <strlist.h>
#include <intlist.h>


#define T_UNKNOWN   -1
#define T_PNG       0
#define T_GIF       1
#define T_WBMP      2
#define T_JPEG      3
#define T_WEBP      4
#define T_TIFF      5
#define T_TGA       6
#define T_BMP       7



char * executable;
strlist ** extensions;

char f_nonl = 0;
char f_onlysize = 0;
char f_noerr = 0;
char f_sb = 0;
char f_sa = 0;
int f_padding = 0;

char err = 0;

int opterr = 0;
int optind = 0;


int usage(void)
{
    fprintf(stderr, "Usage: %s [OPTION] filename [...]\n", executable);
    return EXIT_FAILURE;
}


void help(void)
{
    printf( "Usage: %s [OPTION] filename [...]\n\n"
            "Options:\n"
            "    -o  Print only size, without filename\n"
            "    -n  Not print newlines\n"
            "    -s  Not display errors\n"
            "    -b  Space before\n"
            "    -a  Space after\n"
            "    -h  Display this help and exit\n",
            executable);
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


void print_img_err(const char * msg, const char * fname)
{
    if(f_noerr)
        return;
    if(f_sb)
        printf(" ");
    if(f_onlysize)
        printf("%s: %s", msg, fname);
    else
        printf("%-*s: %s", f_padding, fname, msg);
    if(f_sa)
        printf(" ");
    if(!f_nonl)
        printf("\n");
}


void print_img_size(const char * fname, int w, int h)
{
    if(f_sb)
        printf(" ");
    if(!f_onlysize)
        printf("%-*s: ", f_padding, fname);
    printf("%dx%d", w, h);
    if(f_sa)
        printf(" ");
    if(!f_nonl)
        printf("\n");
}


void show_img_size(const char * fname)
{
    FILE * fp;
    int type;
    gdImagePtr img;
    struct stat fstat;
    int ret;

    if((ret = stat(fname, &fstat)) != 0) {
        /* fprintf(stderr, "%-*s: Cannot open file\n", padding, fname); */
        print_img_err("Cannot open file", fname);
        return;
    }
    if(S_ISDIR(fstat.st_mode)) {
        print_img_err("Is a directory", fname);
        return;
    }

    type = get_img_type(fname);
    if(type == T_UNKNOWN) {
        print_img_err("Unknown type", fname);
        return;
    }

    if((fp = fopen(fname, "rb")) == NULL) {
        print_img_err("Cannot open file", fname);
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
        print_img_err("Wrong file", fname);
    else
        print_img_size(fname, gdImageSX(img), gdImageSY(img));

    if(img != NULL)
        gdImageDestroy(img);
}


void gd_errmethod(int n, const char * msg, va_list args)
{
    err = 1;
}


int main(int argc, char * argv[])
{
    int i,
        len,
        opt,
        cnt;

    intlist * opts = intlist_init();
    strlist * files = strlist_init();

    executable = (char *) malloc(strlen(argv[0]));
    strcpy(executable, argv[0]);

    if(argc < 2)
        return usage();

    while((opt = getopt(argc, argv, "onsbah")) != -1) {
        switch(opt) {
            case 'o':
                f_onlysize = 1;
                break;
            case 'n':
                f_nonl = 1;
                break;
            case 's':
                f_noerr = 1;
                break;
            case 'b':
                f_sb = 1;
                break;
            case 'a':
                f_sa = 1;
                break;
            case 'h':
                help();
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "Invalid option: %s\n", argv[optind - 1]);
                return EXIT_FAILURE;
        }
        intlist_push(opts, optind - 1);
    }

    i = 1;
    while(i < argc) {
        if(intlist_search(opts, i) == NULL) {
            len = strlen(argv[i]);
            if(len > f_padding)
                f_padding = len;
            strlist_push(files, argv[i]);
        }
        i++;
    }

    if(!files->size)
        return usage();

    init_extensions();
    gdSetErrorMethod(gd_errmethod);

    i = 0;
    while(i < files->size)
        show_img_size(strlist_get(files, i++));

    free(executable);
    i = 0;
    while(i < sizeof(extensions))
        strlist_free(&extensions[i++]);
    free(extensions);
    strlist_free(&files);
    intlist_free(&opts);

    return err ? EXIT_FAILURE : EXIT_SUCCESS;
}

