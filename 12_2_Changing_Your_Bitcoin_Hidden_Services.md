# Chapter 12.2: Changing Your Bitcoin Hidden Services

In this chapter we will show you how to create or change your local Bitcoin Hidden Service.   To archieve this we need to add current user to tor or debian-tor group to guarantee that Bitcoin daemon can sets up an automatic hidden service on the first startup if it's correclty configured.    Bitcoind will create a file called onion_private_key in the data directory.  Further check if file control.authcookie exists like this:

```
~$ ll /run/tor/control.authcookie
-rw-r----- 1 debian-tor debian-tor 32 jun 26 09:44 /run/tor/control.authcookie
```

Add your user to debian-tor group like this:

```
~$ sudo usermod -a -G debian-tor [CHANGE_MY_USER]
```


 ll /run/tor/control.authcookie
-rw-r----- 1 debian-tor debian-tor 32 jun 26 09:44 /run/tor/control.authcookie
