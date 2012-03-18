PATH=/sbin:/bin:/usr/sbin:/usr/bin
DAY=$(date --date "today" +%d.%m.%Y-%H:%M)
SRC="/var/www/html"
DST="/var/backups"

mysqldump -pewrthgq --all-databases --skip-extended-insert | bzip2 -c >$DST/${DAY}.sql.bz2
tar cjpf $DST/${DAY}.tar.bz2 $SRC 2>/dev/null

find $DST -mtime +3 -exec rm -f {} \;
