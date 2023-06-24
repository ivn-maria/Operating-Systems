#!/bin/bash
 
read -p "Username: " username
echo "Active sessions: $(who | grep "^${username}" | wc -l)"
