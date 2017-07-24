# emily tech paper

init, render/process procedure, and the like.

## initialization

1. application loads engine:
  - engine loads backend
  - engine pre-inits variables

2. application loads frame(s)
  - this can be done after initialization as well, but i'd prefer to load them at the beginning

3. application inserts widgets into frame(s)
  - engine manages skinning and textures

4. application sets other parameters
  - some of them can't be set on the fly

5. application lets the engine run
  - the engine allows for detached running too

## engine loop

1. graphics card swaps buffers

2. start wait timer (which will fire an event)

3. while timer is running, engine thread waits for next event

4. process next event
  - go back to 3 if not a timer event

5. render. measure render time

6. present. graphics card will schedule a swap.

7. estimate wait timer value.

8. clear. this forces the engine to wait for the swap to happen.
  - go back to 1

the reason why we are using a wait timer is because:

- if we render just after clearing, there will be input lag.

- if we render, then present and clear, it won't allow (or make difficult) not rendering when no changes to save battery.

this has only been tested on Linux with Mesa on Intel.
OS X, Windows and non-Mesa are unconfirmed. maybe they don't schedule swaps but rather wait for vblank and then present.

## rendering

1. draw stuff in the "possible change" queue.

2. if this queue is blank we may have a chance to sleep the engine until something happens.

## mouse event

1. check if we collide with any widgets in the region where the pointer is located at.
  - regions to save calculation time

2. if so, then process hover/up/down functions on widgets.
  - add them to "possible change" queue.
  - and add any other plugin specified regions there.

## keyboard/text input event

1. send event to widgets that want to catch all keyboard events.

2. send event to the currently selected widget.
