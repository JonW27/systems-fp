# s i l e t t o
A systems final project by Jonathan Wong and Jeremy Sharapov.

## M A N I F E S T O

Siletto is a blazing-fast modern browser without shitty bloatware that doesn't hog up all your RAM (looking at you Electron and Google Chrome)<sup>1</sup>.

All those things that browsers throw at you: profiles, extensions, history, find & replace, hamburger menus; we don't need any of those.

We get you. You want a C L E A N browsing experience.

You don't want just that. You want a F A S T browsing experience. You want light speed; you got it. With our barebones backbone of webkit-gtk (not chromium), we got your back.

You don't want M E M O R Y _ L E A K S<sup>[2](https://github.com/electron/electron/issues/2575) [3](https://github.com/electron/electron/issues/9823) [4](https://twitter.com/admiraljonjon/status/593505756768444416?lang=en)</sup>.

And best of all you want it F R E E. And not free as in free beer, but free as in truly F R E E.

As in F R E E software. As in software that'll never be proprietary. That people can always contribute to, that people can always modify. Software that won't be like that startup here<sup>[5](https://venturebeat.com/2016/02/15/palantir-acquires-kimono-labs-will-shutter-data-collection-service-on-february-29/)</sup> or that startup there<sup>[6](https://techcrunch.com/2018/01/23/facebook-confirm-io/)</sup> or ... the list goes on and on and on...<sup>[7](https://ourincrediblejourney.tumblr.com/)</sup>

We hold all of these V A L U E S dear, crafting a perfectly exquisite browser experience for you.

## F E A T U R E S
* Actually is a browser
* Actually runs
* Back button, forward button, stop button, uri bar all work
* uri button works with any IP address or hostname, including 127.0.0.1 ports, and will read raw data files on the computer
* the layout doesn't look too terrible
* it has multiprocess architecture
* it looks beautiful

## I N S T A L L I N G
We're too lazy to create a .deb file.

## C O M P I L I N G
```sh
  make
```
or, if you want to compile it yourself, cause why not:
```sh
  gcc main.c -o siletto `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-3.0`
```

## R U N N I N G
```sh
make run
```

## O U R _ J O U R N E Y

I guarantee you this was more than just a systems final project. This was the result of days of hard work thinking about every little exquisite detail, about what not to have rather than what to have.

Designers say that the text is defined by whitespace. I agree. We hereby present our entire journey to make this beautiful product.

TBA.

## W H A T _ W E _ L E A R N E D

* Building a browser for an Ubuntu system when both people don't run Ubuntu is generally a bad idea, especially when the library you're using to build it, flatly states "building it on macos and windows is really hard, and we haven't tried doing so in like, 5 years"

* Installing Ubuntu on an old Windows computer can be harder than you think because partitioning and MBR systems.

* Initially trying to compile webkit-gtk is hard, because it's not very easy to find the compile statement.

* There is no .pc package-config file for the (deleted) homebrew version of webkit-gtk so I couldn't install it/find it.

* If you try to query webkitgtk packages you're gonna hit a lotta 404s.

* There are literally no webkitgtk examples on the Internet and if so, they're outdated, using deprecated stuff.

* The default answer when asking anyone about gtk layout is a referral to a RAD tool called glade, instead of actually trying to code it.

* Gtk layout is all nice and good and easy, but when trying to embed a webview in there, it's like "gotcha, ain't gonna display"(the url bar literally took 2 days)

* Ubuntu wifi connections can be really *really* **weak**. In which case, you're not sure if your test cases succeeded.

* Browsers are way more complex than they seem, especially with profiles and tab management.

* X-Servers can be slow

* C++ and Python are close enough to C when thinking about things

* Some people actually just put code in a video and never speak.

* Documentation for many projects just plain sucks.

<sup>1</sup>We have no benchmarks yet, but it's our gut feeling. And that's what matters.
