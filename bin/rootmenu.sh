#!/bin/sh

cat <<EOF | xmenu | sh &
Applications
	Terminal					kitty
	Browser						brave
dwm
	Resize Window 		dwmc run_command rioresize
	Toggle Bar 		dwmc run_command togglebar
	Toggle Gaps 		dwmc run_command togglegaps
	Fullscreen
		Toggle fullscreen 	dwmc run_command togglefullscreen
		Toggle fakefullscreen 	dwmc run_command togglefakefullscreen
System
	Exit dwm 	dwmc run_command quit
	Shutdown 	poweroff
	Restart 	reboot
EOF
