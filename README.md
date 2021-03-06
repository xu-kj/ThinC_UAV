# ThinC_UAV
Updating the Irrlicht UAV project

---------------------------------------------

ThinC Lab UAV Project TODO list:

Target Recognition
* [x] add indicator that will light up when uav flies near assigned target locations
* [x] add two helper buttons (yes/no) that become clickable w/ the indicator
* [x] add a button that is clickable at any time, and record when it’s clicked
* [x] add property for waypoint (bool Target, bool Indicated)
* [x] record all buttons’ reactions in one file, time indicator triggered and button clicked
* [x] change to 6 cams and add intervals between cams
* [x] camera screen lights up with the indicator
* [x] change camera screen frame to the same color
* [x] change the indicator button to be a trigger that lights up the screen
* [x] the indicator button should only light up the screen for 4-5 seconds after button is pressed
* [x] pause simulation after one minute of time and request user feedback on "trustworthiness" of uav

Package Delivery
* [x] cam_window light level auto changes up arriving target location
* [x] 3 buttons: yes, no, unsure
* [x] add event type that is associated with UAV and record response by typing on keyboard
* [x] add event type that comes with an audio alarm
* [x] add a button for UAV cam that records user’s response to the audio alarm
* [x] add event type that comes with a visual alarm
* [x] add a button for UAV cam that records user’s response to the visual alarm, dims out after click
* [x] add configuration value that sets intensity for both types of alarms
* [ ] if the configuration value is empty, guide the user to set the value in the program before sim, to let the intensity of both alarms match (given visual to set volume, and reverse)
* [x] cross model matching (fixed light level, change volumne), repeat until enter is hit
* [x] light up and enable buttons only when the UAV stops
* [x] stop at target, wait for response, no timeout, ~~timeout = 30s~~
* [x] visual alarm ~~lasts 3 sec~~, dims after click
* [x] visual alarm and audio alarm on a time scale
* [x] lower altitude after clicking yes
* [x] create audio files for each uav alert
* [x] change all camera windows' borders to the same blue color
* [ ] TBD | change video/audio response buttons into one single reaction button
* [x] ~~VA_1: add a border with numbers as video alarm, display numbers~~
* [ ] TBD | VA_2: change video alarm button to show number in all 9 camera boxes, use keyboard as input
* [x] show uav number/index in each camera box
* [x] texture: change target model into delivery pad
* [x] texture: update map, to make map look more like urban streets
* [ ] change beginning prompt for x-model matching
* [x] revert VA_1, remove window 5 and make it the video alert box
* [x] user could only react after the uav stops
* [ ] run in fullscreen
* [ ] fix data recording and calculation

General Tasks
* [ ] support for global speed change, for testing
* [ ] record events in time sequence
* [ ] calculate correctness and record response time
* [ ] change UAV fly path so that it always follow the scheduled path
* [ ] fix camera box shape, 3,6,9's shape changes strangely
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
* [ ] back up UAV project, with current and older versions

--------------------
More about the project: ... (to be determined)
game-like?

libraries being used in the project:
Irrlicht, irrKlang

extra help: Markdown cheatsheet (https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet)
