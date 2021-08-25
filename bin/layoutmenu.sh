#!/bin/sh

cat <<EOF | xmenu | zsh
[]= tile				dwmc run_command setlayoutex 0
[M] monocle				dwmc run_command setlayoutex 1
[@] spiral				dwmc run_command setlayoutex 2
[\\ dwindle	        		dwmc run_command setlayoutex 3
H[] deck				dwmc run_command setlayoutex 4
TTT bstack				dwmc run_command setlayoutex 5
HHH grid				dwmc run_command setlayoutex 6
|M| centeredmaster			dwmc run_command setlayoutex 7
>M> centeredfloatingmaster   		dwmc run_command setlayoutex 8
|+| tatami 				dwmc run_command setlayoutex 9
><> floating     			dwmc run_command setlayoutex 10
EOF
