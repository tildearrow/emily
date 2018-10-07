# emily environment variables

## EMILY_SCALE

- real number.

- if set, force a scale factor

- note that fractional scaling is broken ATM

## EMILY_SCALE_METHOD (unimplemented)

- string, containing one or many of the following, separated by colons:

  - `config`: emily.conf
  - `x11dpi`: DPI from X server
  - `cocoabsf`: backingScaleFactor from mainScreen
  - `winfontdpi`: font DPI from Windows
  - `winlogpix`: LOGPIXELSX/Y from Windows
  - `gtk`: GDK_SCALE
  - `qt`: QT_SCALE_FACTOR
  - `gnomesf`: org.gnome.desktop.interface scaling-factor
  - `efl`: ELM_SCALE
  - `auto`: auto-detect based on display resolution

## EMILY_POWER_SAVING

- 0 or 1.

- enable or disable stopping render when unneeded

## EMILY_WAIT_MSC

- 0 or 1.

- use glXWaitForMscOML for VSync

  - part of a Mesa driver bug workaround

## EMILY_LATE_POLL

- real number.

- use late polling to reduce latency.

## EMILY_UPDATE_RATE

- real number.

- update rate if VSync is off/unavailable

## EMILY_CLIENT_BORDER

- 0 or 1.

- controls whether to display a border on video outs (for GNOME Wayland, where CSD is mandatory)

## DEJAVU_OBESITY

- set to over 9000 to force emily to use DejaVu if it is the default font in your system.

- setting to "31337" or higher will also scale the UI to fit this horrible font, too.
