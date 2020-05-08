# Intrusive Tomato Timer
![](https://img.shields.io/github/downloads/modbrin/intrusive-tomato-qt/total)


Hypothesis of this app is that long working sessions affect productivity in a bad way but more importantly long sitting sessions can be of great harm to health. The goal is to break sessions into chunks and suggest user to stand up and do some exercises.

It is not designed to fight with you, just effectively interrupt you from working. So you can terminate it at this point (may be made more difficult in future), but do you really want to...

NOTE: This app primarily targets Windows 10; It also works under Linux losing blur/transparency effects (check out gnome-pomodoro app); It doesn't work under macos due to os limitations.

> For download visit [Releases](https://github.com/modbrin/intrusive-tomato-qt/releases)

## Features
+ Break veil covers whole screen
+ Configurable work and break intervals
+ Acrylic blur background effect for Windows 10
+ Minimize to tray
+ Free and open source forever

<p align="center">
	<img src="./misc/screenshots/app_startup.png" alt="app_startup" width="300"/>
</p>

#### When there are 20 seconds left until break, notification will appear (also, it's draggable if covers important content)
<p align="center">
	<img src="./misc/screenshots/notification.png" alt="app_startup" width="300"/>
</p>

#### Break veil covers whole screen
<p align="center">
	<img src="./misc/screenshots/fullscreen.png" alt="app_startup" width="300"/>
</p>

## Building
Use Qt Creator for building, default version is dynamically linked. If you want to link statically, refer to Qt documentation and firstly build static version of Qt Libraries.

## Future
+ "Seamless" startup
+ Long breaks
+ Activity suggestions during breaks
+ Flow mode for gaming/movies
+ Reminders and task tracking
+ Google Calendar / Microsoft Outlook integration
+ Learning user habits
+ Tests

## License
This software is licensed under [GPL v3](LICENSE)
