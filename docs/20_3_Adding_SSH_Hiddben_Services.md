# Chapter 20.3: Adding SSH Hidden Services

To date, you've used Tor with your Bitcoin services, but you can also
use it to protect other services on your machine, improving their
security and privacy. This section demonstrates how by introducing an
`ssh` hidden service to login remotely using Tor.

## Create SSH Hidden Services

New services are created by adding them to the `/etc/tor/torrc` file.

The following must be done as root:
```
sudo cat >> /etc/tor/torrc << EOF
HiddenServiceDir /var/lib/tor/hidden-service-ssh/
HiddenServicePort 22 127.0.0.1:22
EOF
```
Here's what that means:

* HiddenServiceDir: Indicates that you have a hidden service directory with the necessary configuration at this path.
* HiddenServicePort: Indicates the tor port to be used; in the case of SSH, this is usually 22.

After you add the appropriate lines to your `torrc` file, you will need to restart Tor:
```
sudo /etc/init.d/tor restart
```

After the restart, your `HiddenServiceDir` should have new files as follows:
```
sudo ls -l /var/lib/tor/hidden-service-ssh

| total 16
| drwx------ 2 debian-tor debian-tor 4096 Oct 29 15:52 authorized_clients
| -rw------- 1 debian-tor debian-tor   63 Oct 29 15:52 hostname
| -rw------- 1 debian-tor debian-tor   64 Oct 29 15:52 hs_ed25519_public_key
| -rw------- 1 debian-tor debian-tor   96 Oct 29 15:52 hs_ed25519_secret_key
```
The file `hostname` in this directory contains your new onion ID:
```
sudo cat /var/lib/tor/hidden-service-ssh/hostname

| s2i67jbe7wahg7supw4hj27chh34vekisndefaar2begudjvjefurjqd.onion
```

You can connect to the `ssh` hidden service using `torify` and that address:

```
torify ssh standup@s2i67jbe7wahg7supw4hj27chh34vekisndefaar2begudjvjefurjqd.onion

| The authenticity of host 's2i67jbe7wahg7supw4hj27chh34vekisndefaar2begudjvjefurjqd.onion (127.42.42.0)' can't be established.
| ED25519 key fingerprint is SHA256:c+RA53ttvqeJ5YOcD+rIAuBE+B2ZxE4Q7l6L4MkgHJg.
| This key is not known by any other names.
| Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
| Warning: Permanently added 's2i67jbe7wahg7supw4hj27chh34vekisndefaar2begudjvjefurjqd.onion' (ED25519) to the list of known hosts.

| standup@s2i67jbe7wahg7supw4hj27chh34vekisndefaar2begudjvjefurjqd.onion's password: 

```
## Summary: Adding SSH Hidden Services

Now that you've got Tor installed and know how to use it, you can add
other services to Tor. You just add lines to your `torrc` (on your
server), then connect with `torify` (on your client).

> 🔥 ***What's the power of Other Hidden Services?*** Every time you
access a service on your server remotely, you leave footprints on the
network. Even if the data is encrypted by something like SSH (or TLS),
lurkers on the network can see where you're connecting from, where
you're connecting to, and what service you're using. Does this matter?
This is the question you have to ask. But if the answer is "Yes", you
can protect the connection with a hidden service.

## What's Next?

For a different sort of privacy, move on to "Using i2p" with [Chapter
Twenty-One: Using i2p](21_0_Using_i2p.md).
