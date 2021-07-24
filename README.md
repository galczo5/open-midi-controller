# Open Midi Controller
Open source firmware for 6 button midi controller based on arduino. It's fully configurable and don't need computer connection. Build you own midi controller and save a lot of money. 

# Features

Most important features:

* 6 buttons, each can be assigned to two actions. First action is triggered on footswitch click, second one is triggered when switch is pressed for more than 1 second.
* LCD status screen
* Configurable without computer connection
* Supported midi commands: Note, CC - send one value, CC - toggle two values
* Five "pages" of configuration. You can assign action to footswitch and toggle between five different configurations

It's easy to calculate that with 6 footswitches, 2 actions for each footswitch and 5 possible configuration pages allows you to assing 60 different commands! Thats a lot.

# Build cost
Cost of parts if you decide to order them in popular Chinese store.

| Part | Cost |
| ---- | ---- |
| Arduino Nano | 2 - 3 USD |
| Arduino Nano Shield (if you don't want to solder) | 1 USD |
| LCD with IC2 module | 2 - 2.5 USD |
| Momentary Footswitches | 6 x 1 - 1.5 USD |
| DIN 5 and other stuff | 1 USD |
| TOTAL | 12 - 16.5 USD  |

*You have to add the cost of the case and soldering supplies. I'm using a case described as T25. It costs about 8 USD. You can make the case of course of almost anything.*

# Build instruction

FAQ:

* **Do I need to know how to solder?** - A little bit. If you can solder two wires together, you have the necessary skills. 
* **Do I need to know C++?** - No
* **Is it hard to create my own controller?** - Relatively easy

See [BUILD](./BUILD.md).

*Remember to follow the build instructions. I cannot get the responsibility if you mess something up. Everything you do, you do on your own responsibility. I just provided a free firmware for your device.*

# Manual

See [MANUAL](./MANUAL.md).

# Feature requests

Leave me an issue on github. It's highly possible that I will implement it.

# Build photos

Send me your build photos, I will post it here :)

My build:

![photo_1](./photos/galczo5_1.jpg)

![photo_2](./photos/galczo5_2.jpg)