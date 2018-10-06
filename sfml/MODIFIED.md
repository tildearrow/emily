this is a custom version of SFML, designed specifically for this project.
this version of SFML adds:
- a Mesa VSync behavior workaround via OML_sync_control
- no polling when waiting for an event
- the ability to cancel waiting for an event
- scissor box functions
- fixes for some compiler warnings
- FloatColor for floating-point color values
- non-RGBA8 texture format support (for deep color and the like)
