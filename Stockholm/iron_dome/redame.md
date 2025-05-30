# iron_dome

iron dome is a school project that aim to implement a small daemon that that watch suspicious activities. Especialy in the fields of malwares that will try to corrupt computers's files such as stockholm the rudimantary cryptoworm coded before.
the logs are written in /var/log/iron_dome.log.

# check that the daemon is running

ps aux | grep iron_dome
pgrep -a iron_dome

# check the daemon memory use

ps -p $(pgrep iron_dome) -o pid,comm,rss,vsz
(Resident Set Size) = physical memory used (in Ko)
(Virtual Set Size) = virtaul memory used (in Ko)
to covert in Mb :

ps -p $(pgrep iron_dome) -o rss=,vsz= | awk 'BEGIN {printf "%-10s %-10s\n", "RSS(MB)", "VSZ(MB)"} {printf "%-10.2f %-10.2f\n", $1/1024, $2/1024}'

# things I monitor

## disk read abuse
I am watching read/write on the specified folders. if a procesus exeed a cartain amount of reading (10MB/s) I will add a warning in the logs.

## usage of cyptographics functions

for the moment the usages of cryptographics functions is watched in a quit simpe way : we go trough the PIDs and wtch for used crypto librairies and CPU usages of those programs. The ideal technique would be to use eBPF and work at kernel level. I don't have thoses knowledes and prefer to stay on the more basic approach.

## entropy of files

entrpy allows to measure "chaos level". most of files are stuctured and features many pattern on a binary level. The only file that will have an entropy close to 8 per bytes (100%) are going to be the compressed or the crypted files. That is why I am checkinh entropy level of files and throwing an alert when a file is sudddenly having a high entropy.
