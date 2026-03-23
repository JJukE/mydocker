#!/bin/bash

# 1. Create the privilege separation directory (often required by sshd in Ubuntu-based containers)
mkdir -p /run/sshd

# 2. Place any other initialization scripts, environment variable exports, or custom setups here.
# ...

# 3. Start the SSH daemon in the foreground. 
# 'exec' replaces the shell process with sshd, making it the PID 1 process.
# '-D' prevents sshd from detaching and becoming a background daemon.
exec /usr/sbin/sshd -D