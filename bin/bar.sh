#!/bin/bash
# shellcheck disable=SC2059,SC2086

bat() {
  CHARGE=$(cat /sys/class/power_supply/BAT0/capacity)
  STATUS=$(cat /sys/class/power_supply/BAT0/status)


  if [ "$STATUS" = "Charging" ]; then
    printf "^c#a9b665^  $CHARGE"
  elif [ "$STATUS" = "Discharging" ]; then
    if [ "$CHARGE" -lt 10 ]; then
      printf "^c#ea6962^ only  $CHARGE left, charge the damn laptop"
    elif [ "$CHARGE" -lt 20 ]; then
      printf "^c#ea6962^  $CHARGE"
    elif [ "$CHARGE" -lt 30 ]; then
      printf "^c#d8a657^  $CHARGE"
    elif [ "$CHARGE" -lt 40 ]; then
      printf "^c#d8a657^  $CHARGE"
    elif [ "$CHARGE" -lt 50 ]; then
      printf "^c#a9b665^  $CHARGE"
    elif [ "$CHARGE" -lt 60 ]; then
      printf "^c#a9b665^  $CHARGE"
    elif [ "$CHARGE" -lt 70 ]; then
      printf "^c#a9b665^  $CHARGE"
    elif [ "$CHARGE" -lt 80 ]; then
      printf "^c#a9b665^  $CHARGE"
    elif [ "$CHARGE" -lt 90 ]; then
      printf "^c#a9b665^  $CHARGE"
    elif [ "$CHARGE" -lt 100 ]; then
      printf "^c#a9b665^  $CHARGE"
    fi
  elif [ "$STATUS" = "Full" ]; then
      printf "^c#a9b665^  $CHARGE"
  fi
}

vol() {
  vol=$(amixer get Master | tail -n1 | sed -r 's/.*\[(.*)%\].*/\1/')
  printf "^c#7daea3^ 墳 $vol"
}

wlan() {
  case "$(cat /sys/class/net/w*/operstate 2>/dev/null)" in
  up) printf "^c#d3869b^直  Connected" ;;
  down) printf "^c#ea6962^睊   Disconnected" ;;
  esac
}

clock() {
  printf "^c#d8a657^ $(date '+%H:%M') "
}

# no delay at startup
dwmc run_command setstatus 0 "  $(clock)  "
dwmc run_command setstatus 1 "  $(wlan)  "
dwmc run_command setstatus 2 "  $(bat)% "
dwmc run_command setstatus 3 "  $(vol)% "
while true; do
  sleep 5
  dwmc run_command setstatus 0 "  $(clock)  "
  dwmc run_command setstatus 1 "  $(wlan)  "
  dwmc run_command setstatus 2 "  $(bat)%  "
done
