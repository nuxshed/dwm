/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        // border pixel of windows
static const unsigned int snap      = 10;       // snap pixel
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 10;       // horiz inner gap between windows
static const unsigned int gappiv    = 10;       // vert inner gap between windows
static const unsigned int gappoh    = 20;       // horiz outer gap between windows and screen edge
static const unsigned int gappov    = 20;       // vert outer gap between windows and screen edge
static       int smartgaps          = 0;        // 1 means no outer gap when there is only one window
static const int showbar            = 1;        // 0 means no bar
static const int topbar             = 1;        // 0 means bottom bar
static const char slopspawnstyle[]  = "-t 0 -c 0.92,0.85,0.69,0.3 -o"; /* do NOT define -f (format) here */
static const char slopresizestyle[] = "-t 0 -c 0.92,0.85,0.69,0.3"; /* do NOT define -f (format) here */
static const int riodraw_borders    = 0;        /* 0 or 1, indicates whether the area drawn using slop includes the window borders */
static const int riodraw_matchpid   = 1;        /* 0 or 1, indicates whether to match the PID of the client that was spawned with riospawn */
static const int riodraw_spawnasync = 0;        /* 0 means that the application is only spawned after a successful selection while
                                                 * 1 means that the application is being initialised in the background while the selection is made */
static int floatposgrid_x           = 5;        /* float grid columns */
static int floatposgrid_y           = 5;        /* float grid rows */
static const char *fonts[]          = { "Fira Code Nerd Font:size=8" };
static const char dmenufont[]       = "Fira Code Nerd Font:size=8";
static const char col_gray1[]       = "#1d2021";
static const char col_gray2[]       = "#282828";
static const char col_gray3[]       = "#3f3f3f";
static const char col_fg[]          = "#ddc7a1";
static const char col_magenta[]     = "#d3869b";
static const char col_red[]         = "#ea6962";
static const char col_green[]       = "#a9b665";
static const char col_blue[]        = "#7daea3";
static const char col_yellow[]      = "#d8a657";
static const char col_aqua[]        = "#89b482";
static const char *colors[][3]      = {
	//                      fg            bg              border
	[SchemeNorm] =        { col_fg,       col_gray1,      col_gray3 },
	[SchemeSel]  =        { col_gray2,    col_aqua,    col_aqua},
	[SchemeScratchSel]  = { col_gray2,    col_aqua,    col_red },
	[SchemeScratchNorm] = { col_gray2,    col_aqua,    col_gray2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

// TODO: actually use this
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance         title       tags mask     isfloating   isterminal   noswallow   floatpos  monitor    scratch key */
	{ "Gimp",     NULL,            NULL,       0,            1,           0,           0,           NULL,     -1,         0  },
	{ "firefox",  NULL,            NULL,       0,            0,           0,          -1,           NULL,     -1,         0  },
        { "kitty",    NULL,            NULL,       0,            0,           1,           0,           NULL,     -1,         0  },
	{ NULL,       NULL,       "scratchpad",    0,            1,           0,           0,           NULL,     -1,        's' },
	{ NULL,       NULL,       "Event Tester",  0,            0,           0,           1,           NULL,     -1,         0  }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "lib/layouts.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define Shift ShiftMask
#define Ctrl ControlMask
#define Alt Mod1Mask
#define AltGr Mod3Mask
#define ShiftGr Mod5Mask

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define SCRATCHKEYS(KEY,CMD) \
	{ MODKEY,                      KEY,      togglescratch,     {.v = CMD } }, \
	{ MODKEY|Ctrl,                 KEY,      setscratch,        {.v = CMD } }, \
	{ MODKEY|Ctrl|Shift,           KEY,      removescratch,     {.v = CMD } }, \

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_fg, "-sb", col_red, "-sf", col_gray1, NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *layoutmenu_cmd = "~/.dwm/layoutmenu.sh";

/*First arg only serves to match against key in rules*/
static const char *scratchcmd1[] = {"s", "kitty", NULL};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,           XK_Return, riospawn,       {.v = termcmd } },
	{ MODKEY,                       XK_s,      rioresize,      {0} },
        SCRATCHKEYS(                    XK_g,                      scratchcmd1)
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Left,   placedir,       {.i = 0 } },
	{ MODKEY|ControlMask,           XK_Right,  placedir,       {.i = 1 } },
	{ MODKEY|ControlMask,           XK_Up,     placedir,       {.i = 2 } },
	{ MODKEY|ControlMask,           XK_Down,   placedir,       {.i = 3 } },
	{ MODKEY|ControlMask,           XK_h,      placedir,       {.i = 0 } },
	{ MODKEY|ControlMask,           XK_l,      placedir,       {.i = 1 } },
	{ MODKEY|ControlMask,           XK_k,      placedir,       {.i = 2 } },
	{ MODKEY|ControlMask,           XK_j,      placedir,       {.i = 3 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask|ControlMask, XK_r,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_d,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_w,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY|ShiftMask|ControlMask, XK_g,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[8]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY,                       XK_e,      setlayout,      {.v = &layouts[10]} },

	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefakefullscreen, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
        // TODO: add keybindings for diagonal movement/resizing
	/* Client position is limited to monitor window area */
	{ Mod4Mask,                     XK_Up,     floatpos,       {.v = "  0x -26y" } }, // ↑
	{ Mod4Mask,                     XK_Left,   floatpos,       {.v = "-26x   0y" } }, // ←
	{ Mod4Mask,                     XK_Right,  floatpos,       {.v = " 26x   0y" } }, // →
	{ Mod4Mask,                     XK_Down,   floatpos,       {.v = "  0x  26y" } }, // ↓
	// { Mod4Mask,                     XK_m,      floatpos,       {.v = "-26x  26y" } }, // ↙
	// { Mod4Mask,                     XK_o,      floatpos,       {.v = " 26x -26y" } }, // ↗
	// { Mod4Mask,                     XK_u,      floatpos,       {.v = "-26x -26y" } }, // ↖
	// { Mod4Mask,                     XK_period, floatpos,       {.v = " 26x  26y" } }, // ↘
	/* Absolute positioning (allows moving windows between monitors) */
	{ Mod4Mask|ControlMask,         XK_Up,     floatpos,       {.v = "  0a -26a" } }, // ↑
	{ Mod4Mask|ControlMask,         XK_Left,   floatpos,       {.v = "-26a   0a" } }, // ←
	{ Mod4Mask|ControlMask,         XK_Right,  floatpos,       {.v = " 26a   0a" } }, // →
	{ Mod4Mask|ControlMask,         XK_Down,   floatpos,       {.v = "  0a  26a" } }, // ↓
	// { Mod4Mask|ControlMask,         XK_m,      floatpos,       {.v = "-26a  26a" } }, // ↙
	// { Mod4Mask|ControlMask,         XK_o,      floatpos,       {.v = " 26a -26a" } }, // ↗
	// { Mod4Mask|ControlMask,         XK_u,      floatpos,       {.v = "-26a -26a" } }, // ↖
	// { Mod4Mask|ControlMask,         XK_period, floatpos,       {.v = " 26a  26a" } }, // ↘
	/* Resize client, client center position is fixed which means that client expands in all directions */
	{ Mod4Mask|ShiftMask,           XK_Up,      floatpos,       {.v = "  0w -26h" } }, // ↑
	{ Mod4Mask|ShiftMask,           XK_Left,    floatpos,       {.v = "-26w   0h" } }, // ←
	{ Mod4Mask|ShiftMask,           XK_plus,    floatpos,       {.v = "800W 800H" } }, // +
	{ Mod4Mask|ShiftMask,           XK_minus,   floatpos,       {.v="-800W -800H" } }, // -
	{ Mod4Mask|ShiftMask,           XK_Right,   floatpos,       {.v = " 26w   0h" } }, // →
	{ Mod4Mask|ShiftMask,           XK_Down,    floatpos,       {.v = "  0w  26h" } }, // ↓
	// { Mod4Mask|ShiftMask,           XK_m,      floatpos,       {.v = "-26w  26h" } }, // ↙
	// { Mod4Mask|ShiftMask,           XK_o,      floatpos,       {.v = " 26w -26h" } }, // ↗
	// { Mod4Mask|ShiftMask,           XK_u,      floatpos,       {.v = "-26w -26h" } }, // ↖
	// { Mod4Mask|ShiftMask,           XK_period, floatpos,       {.v = " 26w  26h" } }, // ↘
	/* Client is positioned in a floating grid, movement is relative to client's current position */
	{ Mod4Mask|Mod1Mask,            XK_Up,      floatpos,       {.v = " 0p -1p" } }, // ↑
	{ Mod4Mask|Mod1Mask,            XK_Left,    floatpos,       {.v = "-1p  0p" } }, // ←
	{ Mod4Mask|Mod1Mask,            XK_Right,   floatpos,       {.v = " 1p  0p" } }, // →
	{ Mod4Mask|Mod1Mask,            XK_Down,    floatpos,       {.v = " 0p  1p" } }, // ↓
	// { Mod4Mask|Mod1Mask,            XK_k,      floatpos,       {.v = " 0p  0p" } }, // ·
	// { Mod4Mask|Mod1Mask,            XK_m,      floatpos,       {.v = "-1p  1p" } }, // ↙
	// { Mod4Mask|Mod1Mask,            XK_o,      floatpos,       {.v = " 1p -1p" } }, // ↗
	// { Mod4Mask|Mod1Mask,            XK_u,      floatpos,       {.v = "-1p -1p" } }, // ↖
	// { Mod4Mask|Mod1Mask,            XK_period, floatpos,       {.v = " 1p  1p" } }, // ↘

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
        { 0,                            0x1008FF02,spawn,          SHCMD("brightnessctl s +5%")  },
        { 0,                            0x1008FF03,spawn,          SHCMD("brightnessctl s 5%-")  },
        { ControlMask,                  0x1008FF02,spawn,          SHCMD("brightnessctl s +1%")  },
        { ControlMask,                  0x1008FF03,spawn,          SHCMD("brightnessctl s 1%-")  },
        { 0,                            0x1008FF11,spawn,          SHCMD("vol -5%")  },
        { 0,                            0x1008FF13,spawn,          SHCMD("vol +5%")  },
        { ControlMask,                  0x1008FF11,spawn,          SHCMD("vol -1%")  },
        { ControlMask,                  0x1008FF13,spawn,          SHCMD("vol +1%")  },
        { 0,                            XK_Print,  spawn,          SHCMD("scr screen")},
        { ControlMask,                  XK_Print,  spawn,          SHCMD("scr window")},
        { ShiftMask,                    XK_Print,  spawn,          SHCMD("scr selection")},
        { MODKEY,                       XK_Print,  spawn,          SHCMD("scr screentoclip")},
        { MODKEY|ControlMask,           XK_Print,  spawn,          SHCMD("scr windowtoclip")},
        { MODKEY|ShiftMask,             XK_Print,  spawn,          SHCMD("scr selectiontoclip")},
        { MODKEY,                       XK_r,      spawn,          SHCMD("rofi -show drun")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ ClkClientWin,         MODKEY,         Button1,        moveorplace,    {.i = 1} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button1,      dragmfact,      {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,      dragcfact,      {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};
