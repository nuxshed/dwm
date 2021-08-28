int
width_ltsymbol(Bar *bar, BarArg *a)
{
	return TEXTW(bar->mon->ltsymbol);
}

int
draw_ltsymbol(Bar *bar, BarArg *a)
{
	return drw_text(drw, a->x, 0, a->w, bh, lrpad / 2, bar->mon->ltsymbol, 0);
}

int
click_ltsymbol(Bar *bar, Arg *arg, BarArg *a)
{
	return ClkLtSymbol;
}
