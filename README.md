# ThinC_UAV
Updating the Irrlicht UAV project

Currently the two version of the project are very similar to each other, almost identical. This is
to make the UAV project be consistent over time, and easier to be updated or modified in the future.
Here are the unique features of each version:

-----------------------------------------------------

1. make scenario
3. modify events
4. trigger events at certain time/by trigger
   highlight button/ make sound
5. check response upon events

-------------------------------------------

1. light up 5 secs in advance
2. stop at target, with timeout = 30s
3. visual alarm lasts 3 secs, dims after click
4. delete not sure button, set no_response action

---------------------------------------------

ThinC Lab UAV Project TODO list:

Target Recognition
* [x] add indicator that will light up when uav flies near assigned target locations
* [x] add two helper buttons (yes/no) that become clickable w/ the indicator
* [x] add a button that is clickable at any time, and record when it’s clicked
* [ ] increase interval between cam_windows
* [ ] change textures
* [ ] add property for waypoint (record time, doesn’t trigger indicator)
* [ ] fix distance detection
* [ ] record all buttons’ reactions in one file, time indicator triggered and button clicked
* [ ] figure out the “Failed to open scenario file” error
* [ ] change to 6 cams and add intervals between cams

Package Delivery
* [x] cam_window light level auto changes up arriving target location
* [x] 3 buttons: yes, no, unsure
* [ ] record hit rate (accuracy) and response time
* [x] add event type that is associated with UAV and record response by typing on keyboard
* [ ] add event type that comes with an audio alarm
* [ ] add a button for UAV cam that records user’s response to the audio alarm
* [ ] add event type that comes with a visual alarm
* [ ] add a button for UAV cam that records user’s response to the visual alarm, dims out after click
* [x] add configuration value that sets intensity for both types of alarms
* [ ] if the configuration value is empty, guide the user to set the value in the program before sim, to let the intensity of both alarms match (given visual to set volume, and reverse)
* [ ] cross model matching (fixed light level, change volumne)
* [ ] light 5 secs in advance
* [ ] stop at target, wait for response, timeout = 30s
* [ ] visual alarm lasts 3 sec, dims after click
* [ ] visual alarm and audio alarm on a time scale

General Tasks
* [ ] update texture and UI
* [x] revert to older version, don’t use network
* [ ] fix lighting functionality (for attention management)
* [ ] fix editor to edit flight path and scenario
* [ ] change the number of UAVs
* [x] fix button_click behavior for the local version
* [ ] change output folder naming format

[ ] get Irrlicht model list
