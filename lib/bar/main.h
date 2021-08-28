typedef struct Bar Bar;
struct Bar {
	Window win;
	Monitor *mon;
	Bar *next;
	int idx;
	int topbar;
	int bx, by, bw, bh; /* bar geometry */
	int w[BARRULES]; // module width
	int x[BARRULES]; // module position
};

typedef struct {
	int x;
	int y;
	int h;
	int w;
        int max_width;
	int value;
} BarArg;

typedef struct {
	int monitor;
	int bar;
        int value;
	int alignment; // see bar alignment enum
	int (*widthfunc)(Bar *bar, BarArg *a);
	int (*drawfunc)(Bar *bar, BarArg *a);
	int (*clickfunc)(Bar *bar, Arg *arg, BarArg *a);
	char *name; // for debugging
	int x, w; // position, width for internal use
} BarRule;

