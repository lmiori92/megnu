# megnu
The reusable yet simple-to-use me"gn"u library for your bare metal uP project!

# Author's notes
I am pretty sure there are tons of similar libraries around but I still wanted to develop my own. Being that said, I *am* for software reuse and simple-yet-powerful APIs in order to develop faster, keeping the overhead in mind.

# Current development

The library is part of an ongoing project; therefore its APIs and code is subject to change and not to be considered stable. Additionally the paging support is not yet integrated in the library and will be probably part of it eventually. Furthermore it is not yet clear how to store data especially considering a typically constrained environmen, callback/page-init-time based progmem-like interface shall be added.

# Highlights
- Lightweight: no scientific data yet but works pretty well.
- Suitable for character displays
- Simple and generic API
- Event driven: easily portable and adaptable to different input sources (buttons, encoders, remote...)
- Cross-Platform due to standard C and careful coding
- Cross-Platform due to the usage of the deasplay library

# Features
- Click, long click, left, right events
- Text only or text and value
- Value is editable if instructed to be so

# MISRA
The code should (almost) follow MISRA rules with some exeptions. Please be aware that I did NOT run an analyzer tool yet, hence there is no guarantee the code actually is. The fact is the code has been compiled without warnings nor strange behavior on a 64-bit Linux machine

# Dependencies

- [deasplay](https://github.com/lmiori92/deasplay)
- [lorenzlib](https://github.com/lmiori92/lorenzlib)
