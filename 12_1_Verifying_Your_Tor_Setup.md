# Chapter 12: Verifying Your Tor Setup

In this chapter we will verify tor installation and setup.

```
~$ sudo -u debian-tor tor --verify-config
```

If tor is installed correctly you should see an output like this:

```
Jun 26 21:52:09.230 [notice] Tor 0.4.3.5 running on Linux with Libevent 2.0.21-stable, OpenSSL 1.0.2n, Zlib 1.2.11, Liblzma 5.2.2, and Libzstd N/A.
Jun 26 21:52:09.230 [notice] Tor can't help you if you use it wrong! Learn how to be safe at https://www.torproject.org/download/download#warning
Jun 26 21:52:09.230 [notice] Read configuration file "/etc/tor/torrc".
Configuration was valid

~$ 
```
