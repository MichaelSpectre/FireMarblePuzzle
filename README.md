# Fire Marble Dome Slider Puzzle
- Author: Michael Wion
- Date:   07/08/2015
- Time taken: About 8 hours

Just a shotty mini-game replica of the dome puzzles
found within Riven: Sequel to Myst, created in less
than 24 hours for The Starry Expanse Project.
Have fun!

Controls:
=========
* Escape........ Give up (exits application)
* Space......... Create a new combination
* Return........ Check if you got the right combination
* Left Mouse.... Drag slider nodes
* Right Mouse... Cheater!

Contents:
======
* main.......... Win32 entry point, main message pump
* CGame......... Centralization of logic and rendering
* CD2D.......... Renderer, uses Direct2D to draw everything
* CInput........ Input manager, uses DirectInput8 & Win32
* CGameLogic.... Responsible for the raw logic of the game
* CSprite....... Just a quick & dirty sprite/image manager

Bugs:
=====
I ran out of time to figure this out, but there seems
to be an issue with the offsets of some of the sliders
(when you try to select them). If you have trouble using
a slider, try clicking slightly to the right of it before
dragging it to a new position. Besides that, I can't
guarantee that no other bugs exist, but I've done my best
to iron out what I could find within the allotted time.

Exercises:
==========
- Try adding the D'ni numbering system into the game as
a "hint system" for the player!
- Add a button under the slider, as opposed to just having
the player press the enter key to check the combination.
- Instead of ugly message boxes, add font rendering to
the application
- Optimizations! (There's plenty of unused code, for example)
