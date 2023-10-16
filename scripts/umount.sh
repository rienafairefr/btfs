#!/usr/bin/env bash 
until umount /tmp/Sintel
do
    sleep 1
done
rm -rf /tmp/Sintel
