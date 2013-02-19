#!/usr/bin/perl

use strict ;

my $cFile = $ARGV[0] ; shift @ARGV ;
my @mFile = @ARGV ;

foreach my $cat (@mFile)
{
  print SRC  #include \"$cat\"\n ";
}

my %cat ;

my $baseDef = "_______" ;
my $id ;

foreach my $cat (@ARGV)
{
  open CAT, $cat ;

  foreach my $line (<CAT>) 
  {
    chomp $line ;
                         
    next unless $line =~ /^\s*\#define\s+(\S+)\s+(.+)\s*$/ ;
    my $define = $1 ;   
    my $value  = $2 ;
 
    unless( $define =~ /${baseDef}$/ )
    {
      $baseDef = $define ;
      $id = $value ;
      $cat{$id}{define} = $define ; 
      next ; 
    }
    $define =~ /^([^_]+)_([^_]+)_(.+)/ ;
    my $definePrefix =  $1 ;
    my $class =  $2 ;
    $cat{$id}{$definePrefix} = $value ;  
    $cat{$id}{class} = $class ;  
  }
  close CAT ;
}

open SRC, ">$cFile" ;
print SRC  "#include <catalog.h>\n";

foreach my $id( keys %cat )
{

}

close SRC ;
