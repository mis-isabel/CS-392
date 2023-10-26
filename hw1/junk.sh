#!/bin/bash 
#Isabel Sutedjo
#I pledge my honor that I have abided by the Stevens Honor System.

mkdir -p $HOME/.junk
#usage from writeup
usage()
{
    cat << EOF
Usage: junk.sh [-hlp] [list of files]
   -h: Display help.
   -l: List junked files.
   -p: Purge all files.
   [list of files] with no other arguments to junk those files.
EOF
}

h1=0
l1=0
p1=0

#parsing command line arguments
while getopts ":hlp" option; do
    case "$option" in
        h) 
            h1=$(( h1 + 1 ))
            ;;
        l) 
            l1=$(( l1 + 1 ))
            ;;
        p) 
            p1=$(( p1 + 1 ))
            ;;
        *)
            echo "Error: Unknown Option '$1'." >&2
            usage
            exit 1
            ;;
    esac
done
 
#error for too many options
if [ $# -gt 1 ]; then 
    echo "Error: Too many options enabled." >&2
    usage
    exit 1
fi

#checking for no input
if [[ $# -eq 0 ]]; then
    usage
    exit 0
fi 

#displaying help
if [[ $h1 -gt 0 ]]; then
    usage
    exit 0
fi

#listing junked files using ls -lAF
if [[ $l1 -gt 0 ]]; then
    ls -lAF $HOME/.junk
    exit 0
fi

#purge all files
if [[ $p1 -gt 0 ]]; then
    rm -rf $HOME/.junk/.*
    exit 0
fi

for filename in "$@"; do
    if [[ -e $filename ]]; then
        mv $filename $HOME/.junk
    else    
        echo "Warning: '$filename' not found."
    fi
done
