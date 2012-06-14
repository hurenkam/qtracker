#!/usr/bin/env python
import xmlrpclib

# Make sure to redirect port 8280 to android emulator by doing the following:
# telnet localost 5554
# redir add tcp:8280:8280

proxy = xmlrpclib.ServerProxy('http://127.0.0.1:8280')
print proxy.trip()
print proxy.track()
print proxy.altitude()
print proxy.speed()
print proxy.course()
print proxy.location()
print proxy.time()
print proxy.distance()
proxy.start(1,15)
proxy.stop()
proxy.exit()

