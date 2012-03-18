#!/bin/sh
#
# Template mirror script (provided by Shaun on the centos-mirror list)
# Minor changes to reflect some sanity and caosity by GMK.
#
# This is a nice script because it utilizes a lock so it won't cause > 1
# rsync process and hammer the master.
#
# Easiest way to run this is to copy locally, chmod +x it, and copy it to
# /etc/cron.daily.

LOCAL_COPY="/usr/share/nginx/html/"
MIRRORS="ftp.mgts.by::pub/CentOS ftp.mgts.by::pub/calculate"
RSYNC="/usr/bin/rsync -aHv --delete --delete-before"
PIDFILE="/var/run/rsync-mirror.pid"

if [ ! -d "$LOCAL_COPY" ]; then
    mkdir -p $LOCAL_COPY
fi

if [ -f "$PIDFILE" ]; then
    RUNPID=`cat $PIDFILE`
    if ps -p $RUNPID; then
        echo "Mirror is already running..."
        exit 1
    else
        echo "Mirror pid found but process dead, cleaning up"
        rm -f $PIDFILE
    fi
fi

echo $$ > $PIDFILE

for SRC in $MIRRORS
do
    $RSYNC $SRC $LOCAL_COPY >/dev/null
done

rm -f $PIDFILE
