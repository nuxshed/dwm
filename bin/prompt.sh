#!/bin/sh

ipccommands=$(cat<<EOF
view
toggleview
tag
toggletag
tagmon
focusmon
focusstack
zoom
incnmaster
killclient
togglefloating
setattachmode
setstatus
setmfact
setlayoutsafe
quit
floatpos
defaultgaps
incrgaps
incrigaps
incrogaps
incrihgaps
incrivgaps
incrohgaps
incrovgaps
incnmaster
rioresize
togglebar
togglefullscreen
togglefakefullscreen
togglefloating
togglegaps
EOF
)

dwmc run_command $(echo "$ipccommands" | dmenu -fn "Fira Code Nerd Font:size=8" -nb "#1d2021" -nf "#ddc7a1" -sb "#89b482" -sf "#1d2021")
