# Woody's Nano 2 Trackball Custom Firmware Keymap
## Behaviors:
### Mouse1 Layer (Default)
* 1000 DPI Trackball Mode
* Button activates Mouse1
* Swift double tap primes Mouse2
* Swift triple tap primes Mouse3
* Swift quintuple (5) tap activates Scroll mode
* A regular double tap can still be done if sluggishly
### Mouse2 Layer
* 1000 DPI Trackball Mode
* Button activates Mouse2, then returns to Mouse1
### Mouse3 Layer
* 1000 DPI Trackball Mode
* Button activates Mouse3, then returns to Mouse1
### Scroll Layer
* Scroll Mode
* Button holds activate LCtrl to allow for zooming adjustment
* Single tap returns to Mouse1

## Only 3 Modified Files
I've inlcuded the compiled firmware file WoodyNano2Keymap.uf2 at the top of the repo.
If you want to edit the files you could just download these and drag them into a regular QMK install
* ...keyboards/ploopyco/nano_2/rules.mk
* ...keyboards/ploopyco/nano_2/config.h
* ...keyboards/ploopyco/nano_2/keymaps/WoodyNano2/keymap.c

## Why
I'm mostly a mouse guy, not a trackball guy, but I was curious and wanted to try the Nano 2 since I thought it would be nice for traveling and better than the trackpad when using CAD software. I really love the Ploopy mouse. But I wanted it to emulate a regular mouse as closely as I could. The scroll layer was somewhat to preserve the original functionality. I've coded before, but I'm not a strong coder and I just wanted this done, not as a learning project, so I confess it's Gemini Vibe Coded, but it seems to work pretty well and wanted to share it.
