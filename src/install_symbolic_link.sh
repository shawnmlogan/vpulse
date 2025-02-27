#!/bin/bash

if test -d $HOME/bin; then
	if test -L $HOME/bin/vpulse; then
		# echo "File vpulse exists...deleting file to replace it with new version."
		rm $HOME/bin/vpulse
	fi
	ln -s $PWD/vpulse $HOME/bin/vpulse
	# echo "File vpulse created..."
	# ls -l $HOME/bin/vpulse
fi
