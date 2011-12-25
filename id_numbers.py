import sys

# Create a variable called 'state'
# This will be used to partition the while loop below
state = "init"
print "the state is", state

# Create a variable called 'atEndOfFile'
# This is a 'Boolean' value
atEndOfFile = False

#
# A while loop will keep executing the contained code over
# and over and over and over until the condition evaluates
# to false.
# This loop is the main state machine for the program.  Each
# input character is processed one at a time until end-of-file.
# If any character is encountered that does not match the
# expected pattern of a valid number than the state machine
# will enter the 'error' state.
#
# For future reference, the pattern matched is:
#     ^\s*[-+]?[0-9]*(.[0-9]+)?\s*$
#
while (state != "error" and not atEndOfFile):
    nextCharRaw = sys.stdin.read(1)
    if (0 == len(nextCharRaw)):
        atEndOfFile = True
    else:
        nextChar = nextCharRaw[0]
        print "next char is", nextChar
        
        if (state == "init"):
            if (nextChar == '-' or nextChar == '+'):
                state = "sign"
            elif (nextChar.isdigit()):
                state = "whole digits"
            elif (nextChar == '.'):
                state = "decimal"
            elif (nextChar.isspace()):
                state = "init"
            else:
                state = "error"
        elif (state == "sign"):
            if (nextChar.isdigit()):
                state = "whole digits"
            elif (nextChar == '.'):
                state = "decimal"
            else:
                state = "error"
        elif (state == "whole digits"):
            if (nextChar.isdigit()):
                state = "whole digits"
            elif (nextChar == '.'):
                state = "decimal"
            elif (nextChar.isspace()):
                state = "whitespace"
            else:
                state = "error"
        elif (state == "decimal"):
            if (nextChar.isdigit()):
                state = "partial digits"
            else:
                state = "error"
        elif (state == "partial digits"):
            if (nextChar.isdigit()):
                state = "partial digits"
            elif (nextChar.isspace()):
                state = "whitespace"
            else:
                state = "error"
        elif (state == "whitespace"):
            if (not nextChar.isspace()):
                state = "error"
        print "the state is", state

# The following if will detect if the last state was an 'accepting'
# state or not, and report the result to the user.
if (state == "whole digits" or state == "partial digits" or state == "whitespace"):
    print "yes, it is a number"
else:
    print "no, it is not a number"
