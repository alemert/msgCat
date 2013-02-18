#!/usr/bin/perl

use strict ;

my $cFile = $ARGV[0] ; shift @ARGV ;

open SRC, ">$cFile" ;

print SRC "
#include <stdio.h>



foreach my $cat (@ARGV)
{
  open CAT, $cat ;

  foreach my $line (<CAT>) 
  {
    chomp $line ;
    next unless $line =~ /^\s*#define\s+/ ;
    print SRC "$line\n" ;
  }

  close CAT ;
}

close SRC ;
