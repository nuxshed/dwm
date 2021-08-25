int
width_wintitle(Bar *bar, BarWidthArg *a)
{
	return a->max_width;
}

int
draw_wintitle(Bar *bar, BarDrawArg *a)
{
	int boxs = drw->fonts->h / 9;
	int boxw = drw->fonts->h / 6 + 2;
	int x = a->x, w = a->w;
	Monitor *m = bar->mon;

	if (m->sel) {
            /* fix overflow when window name is bigger than window width */
			int mid = (m->ww - (int)TEXTW(m->sel->name)) / 2 - x;
			/* make sure name will not overlap on tags even when it is very long */
			mid = mid >= lrpad / 2 ? mid : lrpad / 2;
			drw_setscheme(drw, scheme[m == selmon ? SchemeNorm : SchemeNorm]);
			drw_text(drw, x, 0, w, bh, mid, m->sel->name, 0);
		if (m->sel->isfloating)
			drw_rect(drw, x + boxs, boxs, boxw, boxw, m->sel->isfixed, 0);
	} else {
		drw_setscheme(drw, scheme[SchemeNorm]);
		drw_rect(drw, x, 0, w, bh, 1, 1);
	}
	return x + w;
}

int
click_wintitle(Bar *bar, Arg *arg, BarClickArg *a)
{
	return ClkWinTitle;
}
