#!/usr/bin/perl

use Net::Telnet;
#$debug=1;
$menu_prompt='/Number:/';
$console_prompt='/>/';

&main();

sub get_DSL_state(){
    my($t, $verbose, $cmd)=@_;

    if ($t->waitfor(Match => $menu_prompt) == 1) {
        print "Sending 24\n" if ($debug);
        $t->print('24');
    } else { 
        print "Error: accessing menu option '24'.\n" if ($debug);
        $t->close;
        return 2;
    }
    if ($t->waitfor(Match => $menu_prompt) == 1) {
        print "Going console mode\n" if ($debug);
        $t->print('8');
        if ($t->waitfor(Match => $console_prompt) == 1) {
    	print "Getting xDSL status\n" if ($debug);
    	@list=$t->cmd(String => 'xdsl state', Prompt => $console_prompt);
    	print "Exiting from console\n" if ($debug);
    	$t->print('exit');
        }
    } else { 
        print "Error: accessing menu option '8'.\n" if ($debug);
        return 2;
    }
    foreach $line (@list){
        chop($line);
        $line =~ /xDSL state/ && ($xDSL_state=(split(/:/, $line))[1]);
        $line =~ /^Operation state/ && ($Oper_state=(split(/:/, $line))[1]);
        $line =~ /^Operation rate/ && ($Oper_rate=(split(/: */, $line))[1]);
        $line =~ /^Framer Sync/ && ($Framer_state=(split(/:/, $line))[1]);
#	print "$line\n";
    }

    print "Framer      : $Framer_state\n" if ($debug || $verbose);
    print "Rate        : $Oper_rate\n" if ($debug || $verbose);
    print "Oper. state : $Oper_state\n" if ($debug || $verbose);
    print "xDSL state  : $xDSL_state\n" if ($debug || $verbose);
    if (($xDSL_state !~ /DSL_ACTIVE/) ||
        ($Oper_state !~ /Data/) ||
        ($Framer_state !~ /Framer in SYNC/)){
	if (!$verbose && $cmd == 1) {
	    print "0\n";
	}
    	return (1);
    }
    if (!$verbose && $cmd == 1) {
	$rate = (split(/ /, $Oper_rate))[0];
	print "$rate\n";
    }
    return (0);
}

sub reset_dsl(){
    my($t)=@_;
	
    if ($telnet->waitfor(Match => $menu_prompt) == 1) {
        print "Sending 24\n" if ($debug);
        $telnet->print('24');
    } else { 
        print "Error: acessing menu option '24'.\n" if ($debug);
        return 2;
    }
    if ($telnet->waitfor(Match => $menu_prompt) == 1) {
        print "Going Diagnostic\n" if ($debug);
        $telnet->print('4');
    } else { 
        print "Error: acessing menu option '4'.\n" if ($debug);
        return 2;
    }
    if ($telnet->waitfor(Match => $menu_prompt) == 1) {
        print "Going DSL restart\n" if ($debug);
        $telnet->print('12');
    } else { 
        print "Error: acessing menu option '12'.\n" if ($debug);
        return 2;
    }
    return (0);
}

sub restart_bridge(){
    my($t)=@_;
	
    if ($telnet->waitfor(Match => $menu_prompt) == 1) {
        print "Sending 24\n" if ($debug);
        $telnet->print('24');
    } else { 
        print "Error: acessing menu option '24'.\n" if ($debug);
        return 2;
    }
    if ($telnet->waitfor(Match => $menu_prompt) == 1) {
        print "Going Diagnostic\n" if ($debug);
        $telnet->print('4');
    } else { 
        print "Error: acessing menu option '4'.\n" if ($debug);
        return 2;
    }
    if ($telnet->waitfor(Match => $menu_prompt) == 1) {
        print "Going Reboot\n" if ($debug);
        $telnet->print('21');
    } else { 
        print "Error: acessing menu option '21'.\n" if ($debug);
        return 2;
    }
    return (0);
}

#exit(0);

sub _connect(){
    my($host, $pass)=@_;
    my $t = new Net::Telnet (Timeout => 2,
				   Prompt => '/:/');
#	$telnet->dump_log("tt");
    $t->open($host);
    if ($t->waitfor(Match => '/Password:/')==1){
	print "Sending password\n" if ($debug);
	$t->print($pass);
    } else { 
	print "Error: sending password\n" if ($debug);
	$t->close;
	return 0;	
    }
    return $t;
    
}

sub _disconnect(){
    my($t)=@_;
    if ($t->waitfor(Match => $menu_prompt) == 1) {
        print "Sending 99\n" if ($debug);
        $t->print('99');
    } else { 
        print "Error: acessing menu option '99'.\n" if ($debug);
        $t->close;
        return 2;
    }
    $t->close;
}

sub usage(){
    print "Usage: prestige_ctl <command> <host> <password>\n";
    print "       <command> is one of following:\n";
    print "       get_speed\n";
    print "       check_dsl\n";
    print "       reset_dsl\n";
    print "       restart\n";
    print "       condrestart\n";
}

sub main(){
    my $err;
    if (! defined $ARGV[2]){
	&usage();
	exit(1);
    }
    $command = $ARGV[0];
    $host    = $ARGV[1];
    $pass    = $ARGV[2];
    
    if (($command ne "check_dsl") &&
	($command ne "get_speed") &&
	($command ne "reset_dsl") &&
	($command ne "restart") &&
	($command ne "condrestart")){
	&usage();
	exit(1);
    }

    $telnet=&_connect($host, $pass);
    if ($command eq "check_dsl"){
	if (($err = &get_DSL_state($telnet, 1, 0))==0){
	    print "Status OK\n";
	}elsif($err == 1){
	    print "Status NOK\n";
	}else{
	    print "Error getting status\n";
	}
    }elsif ($command eq "get_speed"){
	&get_DSL_state($telnet, 0, 1);
    }elsif ($command eq "reset_dsl"){
	$err=&reset_dsl($telnet);
    }elsif ($command eq "restart"){
	$err=&restart_bridge($telnet);
        exit($err);
    }elsif ($command eq "condrestart"){
	if (($err = &get_DSL_state($telnet, 1))==0){
	    $err=&restart_bridge($telnet);
	    exit($err);
	}
    }
    &_disconnect($telnet);
    exit($err);
}
