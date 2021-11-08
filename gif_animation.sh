#!/bin/bash

# settings
Delay=10
Loop=0

if [ $# -ne 1 ]; then
 echo "Usage : $0 image_directory"
 exit
fi

if [ ! -d $1 ]; then
 echo "$1 does not exist or is not a directory."
 exit
fi

convert -delay $Delay -loop $Loop $1/xy_p_*.png xy_p.gif &
convert -delay $Delay -loop $Loop $1/yz_p_*.png yz_p.gif &
convert -delay $Delay -loop $Loop $1/xz_p_*.png xz_p.gif &

convert -delay $Delay -loop $Loop $1/xy_vx_*.png xy_vx.gif &
convert -delay $Delay -loop $Loop $1/xy_vy_*.png xy_vy.gif &
convert -delay $Delay -loop $Loop $1/xy_vz_*.png xy_vz.gif &

wait

convert -delay $Delay -loop $Loop $1/yz_vx_*.png yz_vx.gif &
convert -delay $Delay -loop $Loop $1/yz_vy_*.png yz_vy.gif &
convert -delay $Delay -loop $Loop $1/yz_vz_*.png yz_vz.gif &

convert -delay $Delay -loop $Loop $1/xz_vx_*.png xz_vx.gif &
convert -delay $Delay -loop $Loop $1/xz_vy_*.png xz_vy.gif &
convert -delay $Delay -loop $Loop $1/xz_vz_*.png xz_vz.gif &

wait
