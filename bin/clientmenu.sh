#!/bin/sh

cat <<EOF | xmenu | zsh
Close Window 				dwmc run_command killclient
Resize Window 				dwmc run_command rioresize
Zoom 					dwmc run_command zoom
Fullscreen
	Toggle Fullscreen 		dwmc run_command togglefullscreen
	Toggle FakeFullscreen 		dwmc run_command togglefakefullscreen
EOF
