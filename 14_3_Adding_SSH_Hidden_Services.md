# Chapter 14.3: Adding SSH Hidden Services

> :information_source: **NOTE:** This section has been recently added to the course and is an early draft that may still be awaiting review. Caveat reader.

To date, you've used Tor with your Bitcoin services, but you can also use it to protect other services on your machine, improving their security and privacy. This section demonstrates how by introducing an `ssh` hidden service to login remotely using Tor. 

## Create SSH Hidden Services

New services are created by adding them to the `/etc/tor/torrc` file:
```
$ su 
# cat >> /etc/tor/torrc << EOF
HiddenServiceDir /var/lib/tor/hidden-service-ssh/
HiddenServicePort 22 127.0.0.1:22
EOF
# exit
```
Here's what that means:

* HiddenServiceDir: Indicates that you have a hidden service directory with the necessary configuration at this path.
* HiddenServicePort: Indicates the tor port to be used; in the case of SSH, this is usually 22.

After you add the appropriate lines to your `torrc` file, you will need to restart Tor:
```
$ sudo /etc/init.d/tor restart
```

After the restart, your `HiddenServiceDir` should have new files as follows:
```
$ sudo ls -l /var/lib/tor/hidden-service-ssh
total 16
drwx--S--- 2 debian-tor debian-tor 4096 Jul 22 14:55 authorized_clients
-rw------- 1 debian-tor debian-tor   63 Jul 22 14:56 hostname
-rw------- 1 debian-tor debian-tor   64 Jul 22 14:55 hs_ed25519_public_key
-rw------- 1 debian-tor debian-tor   96 Jul 22 14:55 hs_ed25519_secret_key
```
The file `hostname` in this directory contains your new onion ID:
```
$ sudo cat /var/lib/tor/hidden-service-ssh/hostname
qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
```
You can connect to the `ssh` hidden service using `torify` and that address:
```
$ torify ssh standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion
The authenticity of host 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion (127.42.42.0)' can't be established.
ECDSA key fingerprint is SHA256:LQiWMtM8qD4Nv7eYT1XwBPDq8fztQafEJ5nfpNdDtCU.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion' (ECDSA) to the list of known hosts.
standup@qwkemc3vusd73glx22t3sglf7izs75hqodxsgjqgqlujemv73j73qpid.onion's password: 
```
## Summary: Adding SSH Hidden Services

Now that you've got Tor installed and know how to use it, you can add other services to Tor. You just add lines to your `torrc` (on your server), then connect with `torify` (on your client).

> :fire: ***What's the power of Other Hidden Services?*** Every time you access a service on your server remotely, you leave footprints on the network. Even if the data is encrypted by something like SSH (or TLS), lurkers on the network can see where you're connecting from, where you're connecting to, and what service you're using. Does this matter? This is the question you have to ask. But if the answer is "Yes", you can protect the connection with a hidden service.

## What's Next?

For a different sort of privacy, move on to "Using i2p" with [Chapter Fifteen: Using i2p](15_0_Using_i2p.md).
