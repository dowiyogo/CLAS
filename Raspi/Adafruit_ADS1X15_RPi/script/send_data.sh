#!/bin/bash
while :
do
	fn=`ls DATA/* | tail -1`
	echo $fn
	rsync -Pav -e "ssh -p 40000" --min-size=1 $fn osoto@localhost:/lustre/expphy/volatile/clas/claseg2/osoto/RENE/.
	sleep 3
done
