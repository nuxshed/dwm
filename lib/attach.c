static void attach(Client *c);
static void attachstack(Client *c);
static void attachx(Client *c);

void
attach(Client *c)
{
	c->next = c->mon->clients;
	c->mon->clients = c;
}

void
attachstack(Client *c)
{
	c->snext = c->mon->stack;
	c->mon->stack = c;
}

void
attachx(Client *c)
{
	Client *at;
	unsigned int n;

	switch (attachmode) {
		case 1: // above
			if (c->mon->sel == NULL || c->mon->sel == c->mon->clients || c->mon->sel->isfloating)
				break;

			for (at = c->mon->clients; at->next != c->mon->sel; at = at->next);
			c->next = at->next;
			at->next = c;
			return;

		case 2: // aside
			for (at = c->mon->clients, n = 0; at; at = at->next)
				if (!at->isfloating && ISVISIBLEONTAG(at, c->tags))
					if (++n >= c->mon->nmaster)
						break;

			if (!at || !c->mon->nmaster)
				break;

			c->next = at->next;
			at->next = c;
			return;

		case 3: // below
			if (c->mon->sel == NULL || c->mon->sel->isfloating)
				break;

			c->next = c->mon->sel->next;
			c->mon->sel->next = c;
			return;

		case 4: // bottom
			for (at = c->mon->clients; at && at->next; at = at->next);
			if (!at)
				break;

			at->next = c;
			c->next = NULL;
			return;
	}

	/* master (default) */
	attach(c);
}
