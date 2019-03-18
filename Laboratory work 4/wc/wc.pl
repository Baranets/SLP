#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Std;

# Define the options
my %options = ();
getopts('lmw', \%options);

# Output the data with/without options 
sub formatPrint {
	my $numLines 	 = $_[0];
	my $numWords  	 = $_[1];
	my $numChars 	 = $_[2];
	my $namePosition = $_[3];

	
	if (defined $options{l}) {
		printf "%+8s ",  $numLines;
		printf "%+7s ",  $numWords if defined $options{w};
		printf "%+7s ",  $numChars if defined $options{m};
		printf "%-8s\n", $namePosition;
	} elsif (defined $options{w}) {
		printf "%+8s ",  $numWords;
		printf "%+7s ",  $numChars if defined $options{m};
		printf "%-8s\n", $namePosition;
	} elsif (defined $options{m}) {
		printf "%+8s ",  $numChars;
		printf "%-8s\n", $namePosition;
	} else {
		printf "%+8s %+7s %+7s %-8s\n", $numLines, $numWords, $numChars, $namePosition;
	}
}

sub countWords {
	my $countWords = 0;

	$countWords = scalar(split /\S+/, $_[0]) - 1;

	return $countWords;
}

sub countChar {
	return length($_[0]);
}

sub countLines {
	my $char = "\n";
	my $countLines = () = $_[0] =~ /\Q$char/g;
	return $countLines;
}

# Number of Arguments
my $numArgs = $#ARGV + 1;

if ($numArgs == 0) {
	
	my $string = '';

	while(my $line = <STDIN>) {
		$string = $string . $line;
	}

	# Counting number of lines
	my $countLines = countLines($string);

	# Counting number of words
	my $countWords = countWords($string);

	# Counting numbers of chars
	my $countChar = countChar($string);

	formatPrint($countLines, $countWords, $countChar, "");

} else {
	
	my $totalLines = 0;
	my $totalWords = 0;
	my $totalChar  = 0;
	my $countIterations = 0;

	foreach my $arg (@ARGV) {
		if (open(my $data, '<:', $arg)) {
		
			#String buliding from file
			my $string = '';
			while (my $line = <$data>) {
				$string = $string . $line;
			}
			
			# Counting number of words
			my $countWords = countWords($string);
			
			# Print error, if argument is a directory
			if ($countWords == -1) {
				print "wc: $arg: read: Is a direcotry\n";
				next; 
			}
			
			# Counting number of lines
			my $countLines = countLines($string);
			
			# Counting numbers of chars
			my $countChar  = countChar($string);

			# Add data to Total variables 
			$totalChar  += $countChar;
			$totalLines += $countLines;
			$totalWords += $countWords;

			# Counting interations
			$countIterations += 1;

			# Output information
			formatPrint($countLines, $countWords, $countChar, $arg);

		} else {
			# Print error, if something wrong with opening file
			print "wc: $arg: open: No such file or directory\n";
		}
	} 

	# Print the total data, if iterations more then 1
	if ($countIterations > 1) {
		formatPrint($totalLines, $totalWords, $totalChar, "total");
	}

}

