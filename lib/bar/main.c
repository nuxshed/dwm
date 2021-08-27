static int bh; /* bar geometry */
static void drawbar(Monitor *m);
static void drawbars(void);
static void drawbarwin(Bar *bar);
static void togglebar(const Arg *arg);

void
drawbar(Monitor *m)
{
	Bar *bar;
	for (bar = m->bar; bar; bar = bar->next)
		drawbarwin(bar);
}

void
drawbarwin(Bar *bar)
{
	if (!bar->win)
		return;
	Monitor *mon;
	int r, w, mi;
	int rx, lx, rw, lw; // bar size, split between left and right if a center module is added
	const BarRule *br;
	BarArg warg = { 0 };
	BarArg darg  = { 0, 0 };

	for (mi = 0, mon = mons; mon && mon != bar->mon; mon = mon->next, mi++); // get the monitor index
	rw = lw = bar->bw;
	rx = lx = 0;

	drw_setscheme(drw, scheme[SchemeNorm]);
	drw_rect(drw, lx, 0, lw, bh, 1, 1);
	for (r = 0; r < LENGTH(barrules); r++) {
		br = &barrules[r];
		if (br->bar != bar->idx || br->drawfunc == NULL || (br->monitor == 'A' && bar->mon != selmon))
			continue;
		if (br->monitor != 'A' && br->monitor != -1 && br->monitor != mi)
			continue;
		drw_setscheme(drw, scheme[SchemeNorm]);
		warg.max_width = (br->alignment < BAR_ALIGN_RIGHT_LEFT ? lw : rw);
		warg.value = br->value;
		w = br->widthfunc(bar, &warg);
		w = MIN(warg.max_width, w);

		if (lw <= 0) { // if left is exhausted then switch to right side, and vice versa
			lw = rw;
			lx = rx;
		} else if (rw <= 0) {
			rw = lw;
			rx = lx;
		}

		switch(br->alignment) {
		default:
		case BAR_ALIGN_NONE:
		case BAR_ALIGN_LEFT_LEFT:
		case BAR_ALIGN_LEFT:
			bar->x[r] = lx;
			if (lx == rx) {
				rx += w;
				rw -= w;
			}
			lx += w;
			lw -= w;
			break;
		case BAR_ALIGN_LEFT_RIGHT:
		case BAR_ALIGN_RIGHT:
			bar->x[r] = lx + lw - w;
			if (lx == rx)
				rw -= w;
			lw -= w;
			break;
		case BAR_ALIGN_LEFT_CENTER:
		case BAR_ALIGN_CENTER:
			bar->x[r] = lx + lw / 2 - w / 2;
			if (lx == rx) {
				rw = rx + rw - bar->x[r] - w;
				rx = bar->x[r] + w;
			}
			lw = bar->x[r] - lx;
			break;
		case BAR_ALIGN_RIGHT_LEFT:
			bar->x[r] = rx;
			if (lx == rx) {
				lx += w;
				lw -= w;
			}
			rx += w;
			rw -= w;
			break;
		case BAR_ALIGN_RIGHT_RIGHT:
			bar->x[r] = rx + rw - w;
			if (lx == rx)
				lw -= w;
			rw -= w;
			break;
		case BAR_ALIGN_RIGHT_CENTER:
			bar->x[r] = rx + rw / 2 - w / 2;
			if (lx == rx) {
				lw = lx + lw - bar->x[r] + w;
				lx = bar->x[r] + w;
			}
			rw = bar->x[r] - rx;
			break;
		}
		bar->w[r] = w;
		darg.x = bar->x[r];
		darg.w = bar->w[r];
		darg.value = br->value;
		br->drawfunc(bar, &darg);
	}
	drw_map(drw, bar->win, 0, 0, bar->bw, bar->bh);
}

void
drawbars(void)
{
	Monitor *m;

	for (m = mons; m; m = m->next)
		drawbar(m);
}

void
togglebar(const Arg *arg)
{
	Bar *bar;
	selmon->showbar = !selmon->showbar;
        // if you want to toggle bar per tag, delete the above line and uncomment the one below.
	// selmon->showbar = selmon->pertag->showbars[selmon->pertag->curtag] = !selmon->showbar;
	updatebarpos(selmon);
	for (bar = selmon->bar; bar; bar = bar->next)
		XMoveResizeWindow(dpy, bar->win, bar->bx, bar->by, bar->bw, bar->bh);
	arrange(selmon);
}

