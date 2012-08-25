#!/usr/bin/perl -w

use strict;
use Term::ANSIColor;

my @DATA;
my $id;
my $bilet = 0;
my $ok = 0;
my $offset;

open(STDERR, ">/dev/null");

if (open(IN, "<:encoding(utf-16)", "Questions.ini")) {
    foreach my $line (<IN>) {
	chomp($line);

	if ($line =~ m/^\[(\d+)\]$/) {
	    $id = $1;
	} elsif ($line =~ m/^que=(.*)$/) {
	    $DATA[$id][0] = $1;
	} elsif ($line =~ m/^(\d)=(.*)$/) {
	    $DATA[$id][$1] = $2;
	} elsif ($line =~ m/^res=(.*)$/) {
	    $DATA[$id][10] = $1;
	}
    }

    close(IN);
}

while (!$bilet || $bilet < -1 || $bilet > $#DATA / 12) {
    printf("Введите номер билета от 1 до %d или -1 для случайного выбора: ", $#DATA / 12);
    $bilet = (<STDIN>);
    chop($bilet);
}

if ($bilet == -1) {
    $bilet = rand(time()) % 30 + 1;
}

$offset = ($bilet - 1) * 12;

for (my $quest_num = 1; $quest_num <= 12; $quest_num++) {
    printf("\n%d) %s\n\n", $quest_num, $DATA[$offset + $quest_num][0]);

    for (my $i = 1; defined($DATA[$offset + $quest_num][$i]); $i++) {
	printf("%d - %s\n", $i, $DATA[$offset + $quest_num][$i]);
    }

    print "> ";
    my $ans = (<STDIN>);
    chop($ans);

    if ($ans eq check($DATA[$offset + $quest_num][10])) {
	$ok++;
	print color("green"), "\nПравильно! ($ok из $quest_num)", color("reset"), "\n";
    } else {
	print color("red"), "\nНе правильно! ($ok из $quest_num)", color("blue"), "\nПравильный ответ: ";
	print check($DATA[$offset + $quest_num][10]), color("reset"), "\n";
    }
}

if ($ok >= 10) {
    print color("green"), "\n\nБилет $bilet сдан!!! ($ok из 12)", color("reset"), "\n";
} else {
    print color("red"), "\n\nБилет $bilet не сдан!!! ($ok из 12)", color("reset"), "\n";
}

sub check {
    my $ret;
    my @str = split(/ /, shift);

    for (my $i = 0; $i <= $#str; $i++) {
	if ($str[$i] == 1) {
	    $ret .= $i + 1;
	}
    }
    return $ret;
}
