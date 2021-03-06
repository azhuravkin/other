#!/usr/bin/perl -w

use strict;
use Term::ANSIColor;
use POSIX "ceil";

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
} else {
    print "Необходимо скачать программу http://beltest.by/files/software/ohotmin2012.exe и извлечь из неё файл Questions.ini\n";
    exit;
}

if (0) {
    for (my $i = 1; $i <= $#DATA; $i++) {
	printf("\n%d) %s\n", $i, $DATA[$i][0]);

	for (my $j = 1; defined($DATA[$i][$j]); $j++) {
	    printf("%d - %s\n", $j, $DATA[$i][$j]);
	}

	printf("Ответ: %s\n", check($DATA[$i][10]));
    }
    exit;
}

while (!$bilet || $bilet < -1 || $bilet > ceil($#DATA / 12)) {
    printf("Введите номер билета от 1 до %d или -1 для случайного выбора: ", ceil($#DATA / 12));
    $bilet = (<STDIN>);
    chop($bilet);
}

if ($bilet == -1) {
    $bilet = int(rand(30)) + 1;
}

$offset = ($bilet - 1) * 12;

for (my $quest_num = 1; $quest_num <= 12 && defined($DATA[$offset + $quest_num][0]); $quest_num++) {
    printf("\n%d) %s\n\n", $quest_num, $DATA[$offset + $quest_num][0]);

    for (my $i = 1; defined($DATA[$offset + $quest_num][$i]); $i++) {
	printf("%d - %s\n", $i, $DATA[$offset + $quest_num][$i]);
    }

    print "> ";
    my $ans = (<STDIN>);
    chop($ans);

    if (order($ans) eq check($DATA[$offset + $quest_num][10])) {
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

sub order {
    my $ret;
    my @arr = split(/[ ,]?/, shift);

    foreach my $char (sort {$a <=> $b} @arr) {
	$ret .= $char;
    }

    return $ret;
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
