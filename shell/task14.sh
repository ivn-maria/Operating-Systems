#!/bin/bash

echo "<table>"
echo "  <tr>"
echo "    <th>Username</th>"
echo "    <th>group</th>"
echo "    <th>login shell</th>"
echo "    <th>GECOS</th>"
echo "  </tr>"

while IFS=: read -r user pass uid gid gecos home shell; do
  echo "  <tr>"
  echo "    <td>${user}</td>"
  echo "    <td>${gid}</td>"
  echo "    <td>${shell}</td>"
  echo "    <td>${gecos}</td>"
  echo "  </tr>"
done < /etc/passwd

echo "</table>"
