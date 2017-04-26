#!/usr/bin/python

#By Naim Ayat

#comm.py
#Accepts two inputs. Outputs three columns.
#First column contains lines unique to 1st input.
#Second column contains lines unique to 2nd input.
#Third column contains common lines.

#If files are unsorted, use command -u
#Example: comm.py -u file1.txt file2.txt

import random, sys, locale
from optparse import OptionParser
parser = OptionParser()
class comm:
    def __init__(self, input1, input2):
        f1 = open(input1, 'r')
        self.lines1 = f1.readlines()
        f1.close()
        f2 = open(input2, 'r')
        self.lines2 = f2.readlines()
        f2.close()

    def sorted(self, sort1, sort2, sort3):
        if sorted (self.lines1) != self.lines1:
            parser.error( "First input is not sorted")
        if sorted (self.lines2) != self.lines2:
            parser.error( "Second input is not sorted")
        i = j = 0
        while i < len(self.lines1) and j < len(self.lines2):
            if self.lines1[i] < self.lines2[j]:
                out = self.lines1[i].rstrip('\n')
                if not sort1:
                    print (out)
                i=i+1
            elif self.lines1[i] > self.lines2[j]:
                out = self.lines2[j].rstrip('\n')
                if not sort2:
                    if sort1:
                        print (out)
                    else:
                        print ('\t' + out)
                j=j+1
            else:
                if not sort3:
                    out = self.lines1[i].rstrip('\n')
                    if sort1 and sort2:
                        print (out)
                    elif (not sort1) and (not sort2):
                        print ('\t' + '\t' + out)
                    else:
                        print ('\t' + out)
                i=i+1
                j=j+1

        while i < len(self.lines1):
            out = self.lines1[i].rstrip('\n')
            if not sort1:
                print (out)
            i=i+1
        while j < len(self.lines2):
            out = self.lines2[j].rstrip('\n')
            if not sort2:
                if sort1:
                    print (out)
                else:
                    print ('\t' + out)
            j=j+1

    def ucomm(self, sort1, sort2, sort3):
        self.common = []
        for i in self.lines1:
            if i in self.lines2:
                out = i.rstrip('\n')
                if not sort3:
                    if (not sort2) and (not sort1):
                        print ('\t\t' + out)
                    elif sort1 and sort2:
                        print (out)
                    else:
                        print ('\t' + out)
                    self.common.append(i)
            else:
                out = i.rstrip('\n')
                if not sort1:
                    print (out)
        for k in self.lines2:
            if k not in self.common:
                out = k.rstrip('\n')
                if not sort2:
                    if sort1:
                        print (out)
                    else:
                        print ('\t' + out)

def main():
    locale.setlocale(locale.LC_COLLATE, 'C')
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE
                    Output randomly selected lines from FILE."""
    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-u", "--unsorted",
                      action="store_true", dest="unsorted", default=0,
                      help="allow input of unsorted files")
    parser.add_option("-1", action="store_true", dest="arg1", default=0,
                      help="suppress first column of output")
    parser.add_option("-2", action="store_true", dest="arg2", default=0,
                      help="suppress second column of output")
    parser.add_option("-3", action="store_true", dest="arg3", default=0,
                      help="suppress third column of output")
    options, args = parser.parse_args(sys.argv[1:])
    if len(args) != 2:
        parser.error("wrong number of operands")

    input_file1 = args[0]
    input_file2 = args[1]

    try:
        generator = comm(input_file1, input_file2)
        if options.unsorted:
            generator.ucomm(options.arg1, options.arg2, options.arg3)
        else:
            generator.sorted(options.arg1, options.arg2, options.arg3)

    except IOError as err:
        err = (errno, strerror)
        parser.error("I/O error({0}): {1}". format(errno,strerror))

if __name__ == "__main__":
    main()