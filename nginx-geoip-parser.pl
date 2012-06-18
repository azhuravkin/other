#!/usr/bin/perl

# cat access1.log access2.log | ./parser.pl
# cat access1.log:
# 192.168.1.1 - - [18/May/2012:06:28:29 +0700] "GET / HTTP/1.1" 200 45378 "-" "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.0.19) Gecko/2010031422 Firefox/3.0.19"

use Geo::IP;
use strict;

my %IP;
my @DATE;
my %COUNTRY;

my $gi = Geo::IP->new(GEOIP_MEMORY_CACHE);

foreach my $line (<STDIN>) {
    $line =~ m/^(\d+\.\d+\.\d+\.\d+) - - \[(\d+\/\w+)\/\d+:.*$/;

    unless (defined($IP{$2})) {
	push (@DATE, $2);
    }

    $IP{$2}{$1}++;
}

for (my $i = 0; $i <= $#DATE; $i++) {
    foreach my $ip (keys %{$IP{$DATE[$i]}}) {
	my $country = $gi->country_name_by_addr($ip) || "Unknown";
	$COUNTRY{$country}{$DATE[$i]}++;
    }
}

printf "%25s ", " ";

for (my $i = 0; $i <= $#DATE; $i++) {
    print "$DATE[$i] ";
}

foreach my $country (sort keys %COUNTRY) {
    printf "\n%-25s ", $country;

    for (my $i = 0; $i <= $#DATE; $i++) {
	printf "%6d ", $COUNTRY{$country}{$DATE[$i]};
    }
}

print "\n";
