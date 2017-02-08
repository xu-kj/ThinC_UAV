# ThinC_UAV
Updating the Irrlicht UAV project

Currently the two version of the project are very similar to each other, almost identical. This is
to make the UAV project be consistent over time, and easier to be updated or modified in the future.

---------------------------------------------

ThinC Lab UAV Project TODO list:

Target Recognition
* [x] add indicator that will light up when uav flies near assigned target locations
* [x] add two helper buttons (yes/no) that become clickable w/ the indicator
* [x] add a button that is clickable at any time, and record when it’s clicked
* [x] add property for waypoint (bool Target, bool Indicated)
* [x] record all buttons’ reactions in one file, time indicator triggered and button clicked
* [x] change to 6 cams and add intervals between cams
* [ ] camera screen lights up with the indicator
* [ ] change camera screen frame to the same color

Package Delivery
* [x] cam_window light level auto changes up arriving target location
* [x] 3 buttons: yes, no, unsure
* [x] add event type that is associated with UAV and record response by typing on keyboard
* [ ] add event type that comes with an audio alarm
* [x] add a button for UAV cam that records user’s response to the audio alarm
* [x] add event type that comes with a visual alarm
* [x] add a button for UAV cam that records user’s response to the visual alarm, dims out after click
* [x] add configuration value that sets intensity for both types of alarms
* [ ] if the configuration value is empty, guide the user to set the value in the program before sim, to let the intensity of both alarms match (given visual to set volume, and reverse)
* [ ] cross model matching (fixed light level, change volumne)
* [ ] ~~light 5 secs in advance~~, deprecated
* [x] stop at target, wait for response, no timeout, ~~timeout = 30s~~
* [x] visual alarm ~~lasts 3 sec~~, dims after click
* [x] visual alarm and audio alarm on a time scale

General Tasks
* [ ] change UAV fly path so that it always follow the scheduled path
* [x] increase precision to millisecond
* [ ] update UAV editor and change openfile directory to absolute path
* [ ] update UAV navigation window
* [ ] update texture and UI
* [x] revert to older version, don’t use network
* [x] fix lighting functionality (for attention management)
* [x] fix button_click behavior for the local version
* [ ] change output folder naming format
* [ ] get Irrlicht model list
* [ ] rebuild project still with Irrlicht engine
* [ ] update rendering engine, rebuild with C# and unity

--------------------
More about the project: ... (to be determined)
game-like?

extra help: Markdown cheatsheet (https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
